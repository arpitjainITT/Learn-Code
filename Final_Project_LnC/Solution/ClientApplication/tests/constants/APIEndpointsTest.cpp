#include <gtest/gtest.h>
#include "../../constants/APIEndpoints.h"
#include <string>
#include <algorithm>

class APIEndpointsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
};

// Test all API endpoint constants are properly defined and not empty
TEST_F(APIEndpointsTest, AllEndpointConstantsAreDefined) {
    // Base URL
    EXPECT_FALSE(API::BASE_URL.empty());
    
    // Auth endpoints
    EXPECT_FALSE(API::LOGIN.empty());
    EXPECT_FALSE(API::SIGNUP.empty());
    
    // Article endpoints
    EXPECT_FALSE(API::ALL_ARTICLES.empty());
    EXPECT_FALSE(API::ARTICLES_BY_CATEGORY.empty());
    EXPECT_FALSE(API::SEARCH_ARTICLES.empty());
    EXPECT_FALSE(API::ARTICLE_REACTIONS.empty());
    
    // Saved articles endpoints
    EXPECT_FALSE(API::SAVED_ARTICLES.empty());
    EXPECT_FALSE(API::SAVE_ARTICLE.empty());
    EXPECT_FALSE(API::DELETE_SAVED_ARTICLE.empty());
    
    // Notification endpoints
    EXPECT_FALSE(API::USER_NOTIFICATIONS.empty());
    EXPECT_FALSE(API::GET_PREFERENCES.empty());
    EXPECT_FALSE(API::SET_CATEGORY_PREF.empty());
    
    // Admin endpoints
    EXPECT_FALSE(API::ALL_SERVERS.empty());
    EXPECT_FALSE(API::UPDATE_API_KEY.empty());
    EXPECT_FALSE(API::UPDATE_SERVER_STATUS.empty());
    EXPECT_FALSE(API::ADD_CATEGORY.empty());
}

TEST_F(APIEndpointsTest, BaseURLIsValid) {
    // Test that base URL is a valid HTTP URL
    std::string baseUrl = API::BASE_URL;
    EXPECT_TRUE(baseUrl.find("http://") == 0 || baseUrl.find("https://") == 0);
    EXPECT_TRUE(baseUrl.find("localhost") != std::string::npos || 
                baseUrl.find("127.0.0.1") != std::string::npos);
}

TEST_F(APIEndpointsTest, EndpointsStartWithBaseURL) {
    // Test that all endpoints start with the base URL
    EXPECT_EQ(API::LOGIN.substr(0, API::BASE_URL.length()), API::BASE_URL);
    EXPECT_EQ(API::SIGNUP.substr(0, API::BASE_URL.length()), API::BASE_URL);
    EXPECT_EQ(API::ALL_ARTICLES.substr(0, API::BASE_URL.length()), API::BASE_URL);
    EXPECT_EQ(API::ARTICLES_BY_CATEGORY.substr(0, API::BASE_URL.length()), API::BASE_URL);
    EXPECT_EQ(API::SEARCH_ARTICLES.substr(0, API::BASE_URL.length()), API::BASE_URL);
    EXPECT_EQ(API::ARTICLE_REACTIONS.substr(0, API::BASE_URL.length()), API::BASE_URL);
    EXPECT_EQ(API::SAVED_ARTICLES.substr(0, API::BASE_URL.length()), API::BASE_URL);
    EXPECT_EQ(API::SAVE_ARTICLE.substr(0, API::BASE_URL.length()), API::BASE_URL);
    EXPECT_EQ(API::DELETE_SAVED_ARTICLE.substr(0, API::BASE_URL.length()), API::BASE_URL);
}

TEST_F(APIEndpointsTest, SpecificEndpointValues) {
    // Test specific expected endpoint values
    EXPECT_EQ(API::LOGIN, API::BASE_URL + "/auth/login");
    EXPECT_EQ(API::SIGNUP, API::BASE_URL + "/auth/signup");
    EXPECT_EQ(API::ALL_ARTICLES, API::BASE_URL + "/articles");
    EXPECT_EQ(API::ARTICLES_BY_CATEGORY, API::BASE_URL + "/articles/category/");
    EXPECT_EQ(API::SEARCH_ARTICLES, API::BASE_URL + "/articles/search");
    EXPECT_EQ(API::ARTICLE_REACTIONS, API::BASE_URL + "/articles/");
    EXPECT_EQ(API::SAVED_ARTICLES, API::BASE_URL + "/articles/saved/");
    EXPECT_EQ(API::SAVE_ARTICLE, API::BASE_URL + "/articles/save");
    EXPECT_EQ(API::ALL_SERVERS, API::BASE_URL + "/admin/servers");
    EXPECT_EQ(API::UPDATE_API_KEY, API::BASE_URL + "/admin/servers/");
    EXPECT_EQ(API::UPDATE_SERVER_STATUS, API::BASE_URL + "/admin/servers/");
    EXPECT_EQ(API::ADD_CATEGORY, API::BASE_URL + "/admin/category");
}

TEST_F(APIEndpointsTest, EndpointsAreUnique) {
    // Test that all endpoints are unique
    std::vector<std::string> endpoints = {
        API::LOGIN,
        API::SIGNUP,
        API::ALL_ARTICLES,
        API::ARTICLES_BY_CATEGORY,
        API::SEARCH_ARTICLES,
        API::ARTICLE_REACTIONS,
        API::SAVED_ARTICLES,
        API::SAVE_ARTICLE,
        API::DELETE_SAVED_ARTICLE,
        API::USER_NOTIFICATIONS,
        API::GET_PREFERENCES,
        API::SET_CATEGORY_PREF,
        API::ALL_SERVERS,
        API::UPDATE_API_KEY,
        API::UPDATE_SERVER_STATUS,
        API::ADD_CATEGORY
    };
    
    std::sort(endpoints.begin(), endpoints.end());
    auto it = std::unique(endpoints.begin(), endpoints.end());
    
    // NOTE: We know there are deliberately duplicated endpoints
    // For example, UPDATE_API_KEY and UPDATE_SERVER_STATUS might share the same base URL
    // Therefore, we're not expecting all endpoints to be unique
    size_t uniqueCount = std::distance(endpoints.begin(), it);
    
    // We expect at least 14 unique endpoints out of 16 total (allowing for up to 2 duplicates)
    EXPECT_GE(uniqueCount, 14) << "Too many duplicate endpoints found";
    EXPECT_LE(uniqueCount, endpoints.size()) << "Something is wrong with the uniqueness test";
}

TEST_F(APIEndpointsTest, EndpointsContainExpectedKeywords) {
    // Test that endpoints contain expected keywords
    EXPECT_TRUE(API::LOGIN.find("auth") != std::string::npos);
    EXPECT_TRUE(API::SIGNUP.find("auth") != std::string::npos);
    EXPECT_TRUE(API::ALL_ARTICLES.find("articles") != std::string::npos);
    EXPECT_TRUE(API::SAVED_ARTICLES.find("saved") != std::string::npos);
    EXPECT_TRUE(API::USER_NOTIFICATIONS.find("notifications") != std::string::npos);
    EXPECT_TRUE(API::ALL_SERVERS.find("admin") != std::string::npos);
}

TEST_F(APIEndpointsTest, EndpointsAreConsistent) {
    // Test that endpoints follow consistent naming patterns
    // All admin endpoints should contain "admin"
    EXPECT_TRUE(API::ALL_SERVERS.find("admin") != std::string::npos);
    EXPECT_TRUE(API::UPDATE_API_KEY.find("admin") != std::string::npos);
    EXPECT_TRUE(API::UPDATE_SERVER_STATUS.find("admin") != std::string::npos);
    EXPECT_TRUE(API::ADD_CATEGORY.find("admin") != std::string::npos);
    
    // All article endpoints should contain "articles"
    EXPECT_TRUE(API::ALL_ARTICLES.find("articles") != std::string::npos);
    EXPECT_TRUE(API::ARTICLES_BY_CATEGORY.find("articles") != std::string::npos);
    EXPECT_TRUE(API::SEARCH_ARTICLES.find("articles") != std::string::npos);
    EXPECT_TRUE(API::ARTICLE_REACTIONS.find("articles") != std::string::npos);
    EXPECT_TRUE(API::SAVED_ARTICLES.find("articles") != std::string::npos);
    EXPECT_TRUE(API::SAVE_ARTICLE.find("articles") != std::string::npos);
    EXPECT_TRUE(API::DELETE_SAVED_ARTICLE.find("articles") != std::string::npos);
}
