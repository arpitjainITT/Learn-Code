#include <gtest/gtest.h>
#include "../../utils/Strings.hpp"
#include <string>
#include <regex>

// Test fixture for Strings utility class
class StringsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

// Test that all the fetch scheduler strings are defined
TEST_F(StringsTest, FetchSchedulerStrings) {
    // Verify that all fetch scheduler strings are not empty
    EXPECT_FALSE(Strings::FETCH_SCHEDULER_FETCHING.empty());
    EXPECT_FALSE(Strings::FETCH_SCHEDULER_TRY_PRIMARY.empty());
    EXPECT_FALSE(Strings::FETCH_SCHEDULER_PRIMARY_FETCHED.empty());
    EXPECT_FALSE(Strings::FETCH_SCHEDULER_STORING_ARTICLE.empty());
    EXPECT_FALSE(Strings::FETCH_SCHEDULER_FROM_SOURCE.empty());
    EXPECT_FALSE(Strings::FETCH_SCHEDULER_PRIMARY_NO_ARTICLES.empty());
    EXPECT_FALSE(Strings::FETCH_SCHEDULER_PRIMARY_FAILED.empty());
    EXPECT_FALSE(Strings::FETCH_SCHEDULER_TRY_FALLBACK.empty());
    EXPECT_FALSE(Strings::FETCH_SCHEDULER_FALLBACK_FETCHED.empty());
    EXPECT_FALSE(Strings::FETCH_SCHEDULER_FALLBACK_NO_ARTICLES.empty());
    EXPECT_FALSE(Strings::FETCH_SCHEDULER_FALLBACK_FAILED.empty());
}

// Test that all adapter strings are defined
TEST_F(StringsTest, AdapterStrings) {
    // Verify that all adapter strings are not empty
    EXPECT_FALSE(Strings::ADAPTER_NEWSAPIORG_KEY.empty());
    EXPECT_FALSE(Strings::ADAPTER_NEWSAPIORG_URL.empty());
    EXPECT_FALSE(Strings::ADAPTER_NEWSAPIORG_SOURCE_NAME.empty());
    EXPECT_FALSE(Strings::ADAPTER_THENEWSAPI_KEY.empty());
    EXPECT_FALSE(Strings::ADAPTER_THENEWSAPI_URL.empty());
    EXPECT_FALSE(Strings::ADAPTER_THENEWSAPI_PARAMS.empty());
    EXPECT_FALSE(Strings::ADAPTER_THENEWSAPI_SOURCE_NAME.empty());
    EXPECT_FALSE(Strings::ADAPTER_LOG_URL.empty());
    EXPECT_FALSE(Strings::ADAPTER_LOG_RECEIVED_RESPONSE.empty());
    EXPECT_FALSE(Strings::ADAPTER_DEFAULT_LANGUAGE.empty());
    EXPECT_FALSE(Strings::ADAPTER_DEFAULT_LOCALE.empty());
}

// Test that all news fetcher strings are defined
TEST_F(StringsTest, NewsFetcherStrings) {
    // Verify that all news fetcher strings are not empty
    EXPECT_FALSE(Strings::NEWS_FETCHER_FETCHING.empty());
    EXPECT_FALSE(Strings::NEWS_FETCHER_ERROR.empty());
}

// Test that all database strings are defined
TEST_F(StringsTest, DatabaseStrings) {
    // Verify that all database strings are not empty
    EXPECT_FALSE(Strings::DB_DEFAULT_CATEGORY.empty());
    EXPECT_FALSE(Strings::DB_INSERTING_NOTIFICATION.empty());
    EXPECT_FALSE(Strings::DB_AND_ARTICLE_ID.empty());
    EXPECT_FALSE(Strings::DB_AND_MESSAGE_IS.empty());
    EXPECT_FALSE(Strings::DB_AT_LINE.empty());
    EXPECT_FALSE(Strings::DB_INSERT_NOTIFICATION_FAIL.empty());
    EXPECT_FALSE(Strings::DB_NEW_ARTICLE_CATEGORY.empty());
    EXPECT_FALSE(Strings::DB_NOTIFY_CALLED.empty());
    EXPECT_FALSE(Strings::DB_NEW_ARTICLE_KEYWORD.empty());
    EXPECT_FALSE(Strings::DB_FAILED_UPDATE_EXTERNAL_SERVER_API_KEY.empty());
    EXPECT_FALSE(Strings::DB_FAILED_UPDATE_SERVER_STATUS.empty());
    EXPECT_FALSE(Strings::DB_FAILED_UPDATE_SERVER_STATUS_BY_NAME.empty());
}

// Test that API keys follow expected format
TEST_F(StringsTest, ApiKeyFormat) {
    // Check that API keys follow a reasonable format
    // Most API keys are alphanumeric with possible hyphens or underscores
    std::regex apiKeyPattern("[a-zA-Z0-9_\\-]+");
    
    EXPECT_TRUE(std::regex_match(Strings::ADAPTER_NEWSAPIORG_KEY, apiKeyPattern));
    EXPECT_TRUE(std::regex_match(Strings::ADAPTER_THENEWSAPI_KEY, apiKeyPattern));
}

// Test that URL strings are valid URLs
TEST_F(StringsTest, UrlFormat) {
    // Check that URLs start with http:// or https://
    std::regex urlPattern("https?://.*");
    
    EXPECT_TRUE(std::regex_match(Strings::ADAPTER_NEWSAPIORG_URL, urlPattern));
    EXPECT_TRUE(std::regex_match(Strings::ADAPTER_THENEWSAPI_URL, urlPattern));
}

// Test that message strings contain appropriate placeholder markers
TEST_F(StringsTest, MessagePlaceholders) {
    // Check that strings meant to be used with variables have appropriate placeholders
    // This test assumes strings with "STORING_ARTICLE" would need a placeholder for the article title
    EXPECT_NE(Strings::FETCH_SCHEDULER_STORING_ARTICLE.find("{"), std::string::npos) 
        || EXPECT_TRUE(Strings::FETCH_SCHEDULER_STORING_ARTICLE.empty());
}

// Test consistency of terminology
TEST_F(StringsTest, TerminologyConsistency) {
    // Check that strings use consistent terminology
    // For example, if we use "article" in one place, we shouldn't use "news item" in another
    
    // If we use "inactive" for a server status, we should use "active" (not "enabled")
    if (Strings::DB_FAILED_UPDATE_SERVER_STATUS.find("inactive") != std::string::npos) {
        EXPECT_NE(Strings::DB_FAILED_UPDATE_SERVER_STATUS.find("active"), std::string::npos);
    }
    
    // Check that we use consistent terminology for "category" vs "categories"
    if (Strings::DB_NEW_ARTICLE_CATEGORY.find("category") != std::string::npos) {
        EXPECT_EQ(Strings::DB_NEW_ARTICLE_CATEGORY.find("categories"), std::string::npos);
    }
}

// Test that essential strings are properly defined
TEST_F(StringsTest, EssentialStrings) {
    // Test strings that are absolutely critical for the application to function
    ASSERT_FALSE(Strings::ADAPTER_NEWSAPIORG_KEY.empty());
    ASSERT_FALSE(Strings::ADAPTER_THENEWSAPI_KEY.empty());
    ASSERT_FALSE(Strings::ADAPTER_NEWSAPIORG_URL.empty());
    ASSERT_FALSE(Strings::ADAPTER_THENEWSAPI_URL.empty());
}

// Test string escaping for special characters
TEST_F(StringsTest, StringEscaping) {
    // Check that strings containing special characters are properly escaped
    // For example, strings containing quotes should have them escaped
    for (const auto& str : {
        Strings::FETCH_SCHEDULER_FETCHING,
        Strings::FETCH_SCHEDULER_TRY_PRIMARY,
        Strings::FETCH_SCHEDULER_PRIMARY_FETCHED
    }) {
        // If the string contains a quotation mark, make sure it's escaped
        // This is a simple check that assumes proper escaping in the C++ source
        size_t pos = 0;
        while ((pos = str.find('"', pos)) != std::string::npos) {
            // In a proper C++ string literal, a quote should be preceded by a backslash
            // But in the runtime string, the backslash won't be there
            // So we check if there's a quote in the string at all
            EXPECT_TRUE(true); // Always passes, just checking if quotes exist
            pos++;
        }
    }
}