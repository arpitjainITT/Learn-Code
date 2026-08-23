/**
 * JwtUtilsTest.cpp
 *
 * Tests JwtUtils::generateToken() and JwtUtils::verifyToken().
 * These tests require the JWT_SECRET environment variable to be set.
 * If not set, all tests are skipped with a clear message.
 */
#include <gtest/gtest.h>
#include "../../utils/JwtUtils.hpp"
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>

// ── Fixture: set a known secret for all tests ─────────────────────────────────
class JwtUtilsTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        // Use a fixed test secret — safe because this is an in-process test
        setenv("JWT_SECRET", "test-secret-key-for-unit-tests-only", 1);
    }

    static void TearDownTestSuite() {
        unsetenv("JWT_SECRET");
    }
};

// ── generateToken ─────────────────────────────────────────────────────────────

TEST_F(JwtUtilsTest, GenerateToken_ReturnsNonEmptyString) {
    std::string token = JwtUtils::generateToken(42, "user");
    EXPECT_FALSE(token.empty());
}

TEST_F(JwtUtilsTest, GenerateToken_HasThreeDotSeparatedParts) {
    std::string token = JwtUtils::generateToken(1, "admin");
    auto dot1 = token.find('.');
    ASSERT_NE(dot1, std::string::npos) << "Token must have at least one '.'";
    auto dot2 = token.find('.', dot1 + 1);
    ASSERT_NE(dot2, std::string::npos) << "Token must have at least two '.'";
    // No third dot (3-part structure: header.payload.signature)
    EXPECT_EQ(token.find('.', dot2 + 1), std::string::npos)
        << "Token must have exactly two '.' separators";
}

TEST_F(JwtUtilsTest, GenerateToken_DifferentUsersProduceDifferentTokens) {
    std::string t1 = JwtUtils::generateToken(1, "user");
    std::string t2 = JwtUtils::generateToken(2, "user");
    EXPECT_NE(t1, t2);
}

TEST_F(JwtUtilsTest, GenerateToken_AdminAndUserProduceDifferentTokens) {
    std::string t1 = JwtUtils::generateToken(1, "user");
    std::string t2 = JwtUtils::generateToken(1, "admin");
    EXPECT_NE(t1, t2);
}

// ── verifyToken ───────────────────────────────────────────────────────────────

TEST_F(JwtUtilsTest, VerifyToken_ValidToken_Succeeds) {
    std::string token = JwtUtils::generateToken(7, "user");

    int         userId = -1;
    std::string role;
    EXPECT_TRUE(JwtUtils::verifyToken(token, userId, role));
    EXPECT_EQ(userId, 7);
    EXPECT_EQ(role,   "user");
}

TEST_F(JwtUtilsTest, VerifyToken_AdminRole_ExtractedCorrectly) {
    std::string token = JwtUtils::generateToken(99, "admin");

    int         userId = -1;
    std::string role;
    EXPECT_TRUE(JwtUtils::verifyToken(token, userId, role));
    EXPECT_EQ(userId, 99);
    EXPECT_EQ(role,   "admin");
}

TEST_F(JwtUtilsTest, VerifyToken_EmptyString_Fails) {
    int uid; std::string role;
    EXPECT_FALSE(JwtUtils::verifyToken("", uid, role));
}

TEST_F(JwtUtilsTest, VerifyToken_MalformedNoDots_Fails) {
    int uid; std::string role;
    EXPECT_FALSE(JwtUtils::verifyToken("notavalidtoken", uid, role));
}

TEST_F(JwtUtilsTest, VerifyToken_TamperedSignature_Fails) {
    std::string token = JwtUtils::generateToken(5, "user");
    // Flip the last character of the signature to simulate tampering
    token.back() = (token.back() == 'a') ? 'b' : 'a';

    int uid; std::string role;
    EXPECT_FALSE(JwtUtils::verifyToken(token, uid, role))
        << "Tampered token must be rejected";
}

TEST_F(JwtUtilsTest, VerifyToken_WrongSecret_Fails) {
    // Generate with current secret
    std::string token = JwtUtils::generateToken(3, "user");

    // Change secret
    setenv("JWT_SECRET", "completely-different-secret", 1);

    int uid; std::string role;
    EXPECT_FALSE(JwtUtils::verifyToken(token, uid, role))
        << "Token signed with different secret must be rejected";

    // Restore secret for subsequent tests
    setenv("JWT_SECRET", "test-secret-key-for-unit-tests-only", 1);
}

TEST_F(JwtUtilsTest, VerifyToken_TamperedPayload_Fails) {
    std::string token = JwtUtils::generateToken(10, "user");

    // Corrupt the payload section (middle part)
    auto dot1 = token.find('.');
    auto dot2 = token.find('.', dot1 + 1);
    std::string payload = token.substr(dot1 + 1, dot2 - dot1 - 1);
    payload[0] = (payload[0] == 'A') ? 'B' : 'A';   // flip one char

    std::string tamperedToken =
        token.substr(0, dot1 + 1) + payload + token.substr(dot2);

    int uid; std::string role;
    EXPECT_FALSE(JwtUtils::verifyToken(tamperedToken, uid, role))
        << "Token with modified payload must be rejected";
}

// ── generateToken round-trip ──────────────────────────────────────────────────

TEST_F(JwtUtilsTest, GenerateAndVerify_RoundTrip_LargeUserId) {
    std::string token = JwtUtils::generateToken(999999, "admin");
    int uid; std::string role;
    ASSERT_TRUE(JwtUtils::verifyToken(token, uid, role));
    EXPECT_EQ(uid,  999999);
    EXPECT_EQ(role, "admin");
}

TEST_F(JwtUtilsTest, GenerateAndVerify_RoundTrip_UserId1) {
    std::string token = JwtUtils::generateToken(1, "user");
    int uid; std::string role;
    ASSERT_TRUE(JwtUtils::verifyToken(token, uid, role));
    EXPECT_EQ(uid,  1);
    EXPECT_EQ(role, "user");
}

// ── Missing secret ────────────────────────────────────────────────────────────

TEST_F(JwtUtilsTest, GetSecret_WhenNotSet_Throws) {
    unsetenv("JWT_SECRET");
    EXPECT_THROW(JwtUtils::getSecret(), std::runtime_error);
    // Restore
    setenv("JWT_SECRET", "test-secret-key-for-unit-tests-only", 1);
}
