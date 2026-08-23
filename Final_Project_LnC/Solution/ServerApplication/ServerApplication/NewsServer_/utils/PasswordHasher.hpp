#pragma once

#include <string>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

namespace PasswordHasher {
    // Returns the SHA-256 hex digest of the given plain-text string.
    inline std::string hash(const std::string& plaintext) {
        unsigned char digest[SHA256_DIGEST_LENGTH];
        SHA256(
            reinterpret_cast<const unsigned char*>(plaintext.c_str()),
            plaintext.size(),
            digest
        );
        std::ostringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            ss << std::hex << std::setw(2) << std::setfill('0')
               << static_cast<int>(digest[i]);
        }
        return ss.str();
    }
}
