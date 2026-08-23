#pragma once

/**
 * JwtUtils.hpp
 *
 * Self-contained JWT (HS256) utilities using OpenSSL HMAC-SHA256.
 * No external jwt library required — OpenSSL is already a project dependency.
 *
 * Token format (RFC 7519):  <base64url(header)>.<base64url(payload)>.<base64url(sig)>
 * Algorithm:                HMAC-SHA256
 * Secret:                   Read from JWT_SECRET environment variable at runtime.
 *
 * Supported claims:
 *   iss  — "NewsAggregator" (issuer check on verify)
 *   iat  — issued-at Unix timestamp
 *   exp  — expiry Unix timestamp (default: 24 h)
 *   user_id — integer user id (stored as JSON number)
 *   role    — "user" | "admin"
 */

#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <chrono>
#include <stdexcept>
#include <iostream>

#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <nlohmann/json.hpp>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/http_header.h>

namespace JwtUtils {

    // ── Constants ─────────────────────────────────────────────────────────────

    constexpr int  TOKEN_EXPIRY_HOURS = 24;
    constexpr const char* ISSUER      = "NewsAggregator";
    constexpr const char* SECRET_ENV  = "JWT_SECRET";

    // ── Internal helpers ──────────────────────────────────────────────────────

    namespace detail {

        inline std::string base64UrlEncode(const unsigned char* data, size_t len) {
            static const char tbl[] =
                "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
            std::string out;
            out.reserve(((len + 2) / 3) * 4);

            for (size_t i = 0; i < len; i += 3) {
                uint32_t b = static_cast<uint32_t>(data[i]) << 16;
                if (i + 1 < len) b |= static_cast<uint32_t>(data[i + 1]) << 8;
                if (i + 2 < len) b |= static_cast<uint32_t>(data[i + 2]);

                out += tbl[(b >> 18) & 0x3F];
                out += tbl[(b >> 12) & 0x3F];
                if (i + 1 < len) out += tbl[(b >>  6) & 0x3F];
                if (i + 2 < len) out += tbl[(b      ) & 0x3F];
            }
            return out;
        }

        inline std::string base64UrlEncode(const std::string& s) {
            return base64UrlEncode(
                reinterpret_cast<const unsigned char*>(s.data()), s.size());
        }

        // Decode base64url (no padding).  Returns empty string on error.
        inline std::string base64UrlDecode(const std::string& input) {
            // Build decode table once (C++17: IIFE on std::array is well-formed)
            static const std::array<int8_t, 256> decTbl = [] {
                std::array<int8_t, 256> t;
                t.fill(-1);
                for (int i = 0; i < 26; i++) { t['A' + i] = i; t['a' + i] = 26 + i; }
                for (int i = 0; i < 10; i++) { t['0' + i] = 52 + i; }
                t['-'] = 62;  t['_'] = 63;
                return t;
            }();

            std::string out;
            out.reserve((input.size() * 3) / 4);

            uint32_t buf  = 0;
            int      bits = 0;
            for (unsigned char c : input) {
                int v = decTbl[c];
                if (v < 0) return "";   // invalid character
                buf = (buf << 6) | static_cast<uint32_t>(v);
                bits += 6;
                if (bits >= 8) {
                    bits -= 8;
                    out += static_cast<char>((buf >> bits) & 0xFF);
                }
            }
            return out;
        }

        // HMAC-SHA256 raw binary digest
        inline std::string hmacSha256(const std::string& data, const std::string& key) {
            unsigned char digest[EVP_MAX_MD_SIZE];
            unsigned int  digestLen = 0;
            HMAC(EVP_sha256(),
                 key.data(),  static_cast<int>(key.size()),
                 reinterpret_cast<const unsigned char*>(data.data()),
                 static_cast<int>(data.size()),
                 digest, &digestLen);
            return std::string(reinterpret_cast<char*>(digest), digestLen);
        }

    } // namespace detail

    // ── Public API ────────────────────────────────────────────────────────────

    /**
     * Read JWT_SECRET environment variable.
     * Throws std::runtime_error if not set or empty.
     */
    inline std::string getSecret() {
        const char* s = std::getenv(SECRET_ENV);
        if (!s || s[0] == '\0') {
            throw std::runtime_error(
                "[JwtUtils] JWT_SECRET environment variable is not set. "
                "Set it before starting the server.");
        }
        return std::string(s);
    }

    /**
     * Generate a signed HS256 JWT for the given user.
     * Returns the full token string: header.payload.signature
     */
    inline std::string generateToken(int userId, const std::string& role) {
        // Fixed header (pre-encoded for efficiency)
        static const std::string ENCODED_HEADER =
            detail::base64UrlEncode(R"({"alg":"HS256","typ":"JWT"})");

        auto nowSec = static_cast<long long>(
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count());

        nlohmann::json payload = {
            {"iss",     ISSUER},
            {"iat",     nowSec},
            {"exp",     nowSec + TOKEN_EXPIRY_HOURS * 3600LL},
            {"user_id", userId},
            {"role",    role}
        };

        std::string encodedPayload   = detail::base64UrlEncode(payload.dump());
        std::string signingInput     = ENCODED_HEADER + "." + encodedPayload;
        std::string encodedSignature = detail::base64UrlEncode(
            detail::hmacSha256(signingInput, getSecret()));

        return signingInput + "." + encodedSignature;
    }

    /**
     * Verify a JWT token string and extract its claims.
     * Returns true on success; populates userId and role.
     * Returns false if the token is missing, malformed, expired, or has a bad signature.
     */
    inline bool verifyToken(const std::string& token, int& userId, std::string& role) {
        // Split into three segments
        auto dot1 = token.find('.');
        if (dot1 == std::string::npos) return false;
        auto dot2 = token.find('.', dot1 + 1);
        if (dot2 == std::string::npos) return false;

        const std::string header    = token.substr(0, dot1);
        const std::string payload   = token.substr(dot1 + 1, dot2 - dot1 - 1);
        const std::string signature = token.substr(dot2 + 1);

        // Verify signature
        std::string signingInput = header + "." + payload;
        std::string expectedSig;
        try {
            expectedSig = detail::base64UrlEncode(
                detail::hmacSha256(signingInput, getSecret()));
        } catch (const std::exception& e) {
            std::cerr << e.what() << "\n";
            return false;
        }

        if (signature != expectedSig) return false;  // tampered / wrong secret

        // Decode and parse payload
        try {
            std::string payloadJson = detail::base64UrlDecode(payload);
            if (payloadJson.empty()) return false;

            auto claims = nlohmann::json::parse(payloadJson);

            // Check issuer
            if (!claims.contains("iss") || claims["iss"].get<std::string>() != ISSUER)
                return false;

            // Check expiry
            auto nowSec = static_cast<long long>(
                std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count());
            if (!claims.contains("exp") || claims["exp"].get<long long>() < nowSec)
                return false;  // expired

            // Extract claims
            if (!claims.contains("user_id") || !claims.contains("role")) return false;
            userId = claims["user_id"].get<int>();
            role   = claims["role"].get<std::string>();
            return true;

        } catch (...) {
            return false;
        }
    }

    /**
     * Extract Bearer token from a Pistache request's Authorization header.
     * Returns empty string if not present or not Bearer.
     */
    inline std::string extractToken(const Pistache::Rest::Request& req) {
        auto authHeader = req.headers().tryGet<Pistache::Http::Header::Authorization>();
        if (!authHeader) return "";

        if (!authHeader->hasMethod<Pistache::Http::Header::Authorization::Method::Bearer>())
            return "";

        const std::string val    = authHeader->value();   // "Bearer <token>"
        const std::string prefix = "Bearer ";
        if (val.size() <= prefix.size()) return "";
        return val.substr(prefix.size());
    }

    /**
     * Validate the request's Bearer token.
     * Returns true and populates userId/role on success.
     */
    inline bool validateRequest(const Pistache::Rest::Request& req,
                                int& userId, std::string& role) {
        const std::string token = extractToken(req);
        if (token.empty()) return false;
        return verifyToken(token, userId, role);
    }

    /**
     * Guard helper for protected routes.
     * Sends HTTP 401 and returns false if token is invalid/missing.
     * Usage at the top of a route handler:
     *   int uid; std::string r;
     *   if (!JwtUtils::requireAuth(req, response, uid, r)) return ...;
     */
    inline bool requireAuth(const Pistache::Rest::Request& req,
                             Pistache::Http::ResponseWriter& response,
                             int& userId, std::string& role) {
        if (!validateRequest(req, userId, role)) {
            response.send(Pistache::Http::Code::Unauthorized,
                          R"({"status":"unauthorized","message":"Valid Bearer token required"})");
            return false;
        }
        return true;
    }

    /**
     * Guard helper for admin-only routes.
     * Sends HTTP 401 if no valid token, HTTP 403 if role != "admin".
     */
    inline bool requireAdmin(const Pistache::Rest::Request& req,
                              Pistache::Http::ResponseWriter& response,
                              int& userId, std::string& role) {
        if (!requireAuth(req, response, userId, role)) return false;
        if (role != "admin") {
            response.send(Pistache::Http::Code::Forbidden,
                          R"({"status":"forbidden","message":"Admin access required"})");
            return false;
        }
        return true;
    }

} // namespace JwtUtils
