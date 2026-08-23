/**
 * HttpClientTest.cpp
 *
 * Tests HttpClient against the real libcurl implementation.
 * We use malformed / unreachable URLs that fail immediately so tests are fast
 * and deterministic without requiring a running server.
 *
 * All HTTP methods return std::string — empty string on failure.
 */
#include <gtest/gtest.h>
#include "../../utils/HttpClient.h"
#include <string>

// A URL that curl rejects immediately (CURLE_URL_MALFORMAT)
static const std::string BAD_URL = "not-a-url";

// Localhost port that is almost certainly not listening
// (port 1 is privileged; connection refused is instantaneous)
static const std::string REFUSED_URL = "http://127.0.0.1:1/path";

TEST(HttpClientTest, SetAuthToken_DoesNotThrow) {
    EXPECT_NO_THROW(HttpClient::setAuthToken("test-token-abc123"));
}

TEST(HttpClientTest, SetAuthToken_EmptyString_DoesNotThrow) {
    EXPECT_NO_THROW(HttpClient::setAuthToken(""));
}

TEST(HttpClientTest, Get_MalformedUrl_ReturnsString) {
    // Must return a string (empty) and not crash
    std::string result = HttpClient::get(BAD_URL);
    EXPECT_TRUE(result.empty());
}

TEST(HttpClientTest, Post_MalformedUrl_ReturnsEmptyString) {
    std::string result = HttpClient::post(BAD_URL, "{}");
    EXPECT_TRUE(result.empty());
}

TEST(HttpClientTest, Put_MalformedUrl_ReturnsEmptyString) {
    std::string result = HttpClient::put(BAD_URL, "{}");
    EXPECT_TRUE(result.empty());
}

TEST(HttpClientTest, Delete_MalformedUrl_ReturnsEmptyString) {
    std::string result = HttpClient::deleteRequest(BAD_URL);
    EXPECT_TRUE(result.empty());
}

TEST(HttpClientTest, Get_UnreachableHost_ReturnsEmptyString) {
    // Connection refused → curl fails fast, returns ""
    std::string result = HttpClient::get(REFUSED_URL);
    EXPECT_TRUE(result.empty());
}

TEST(HttpClientTest, Post_UnreachableHost_ReturnsEmptyString) {
    std::string result = HttpClient::post(REFUSED_URL, "{\"key\":\"value\"}");
    EXPECT_TRUE(result.empty());
}

TEST(HttpClientTest, ReturnType_IsStdString) {
    // Return type must be std::string regardless of failure
    auto r1 = HttpClient::get(BAD_URL);
    auto r2 = HttpClient::post(BAD_URL, "");
    auto r3 = HttpClient::put(BAD_URL, "");
    auto r4 = HttpClient::deleteRequest(BAD_URL);
    static_assert(std::is_same<decltype(r1), std::string>::value, "get() must return std::string");
    static_assert(std::is_same<decltype(r2), std::string>::value, "post() must return std::string");
    static_assert(std::is_same<decltype(r3), std::string>::value, "put() must return std::string");
    static_assert(std::is_same<decltype(r4), std::string>::value, "deleteRequest() must return std::string");
}
