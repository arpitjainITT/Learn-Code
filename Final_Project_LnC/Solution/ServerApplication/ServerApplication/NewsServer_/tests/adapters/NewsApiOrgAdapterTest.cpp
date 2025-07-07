#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../adapters/NewsApiOrgAdapter.hpp"
#include "../../utils/HttpClient.hpp"
#include <nlohmann/json.hpp>

// Mock for HttpClient to avoid real API calls
class MockHttpClient {
public:
    static std::string get(const std::string& url) {
        if (shouldThrow) {
            throw std::runtime_error("Network error");
        }
        return mockResponse;
    }

    static void setMockResponse(const std::string& response) {
        mockResponse = response;
    }

    static void setShouldThrow(bool throw_) {
        shouldThrow = throw_;
    }

private:
    static std::string mockResponse;
    static bool shouldThrow;
};

std::string MockHttpClient::mockResponse = "{}";
bool MockHttpClient::shouldThrow = false;

// Test fixture for NewsApiOrgAdapter
class NewsApiOrgAdapterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset mock behavior before each test
        MockHttpClient::setShouldThrow(false);
        MockHttpClient::setMockResponse("{}");
    }

    void TearDown() override {
        // Clean up after each test
    }

    // Helper to create a sample NewsAPI.org response
    std::string createSampleResponse() {
        nlohmann::json response = {
            {"status", "ok"},
            {"totalResults", 2},
            {"articles", nlohmann::json::array({
                {
                    {"source", {{"id", "techcrunch"}, {"name", "TechCrunch"}}},
                    {"author", "John Doe"},
                    {"title", "Test Tech Article"},
                    {"description", "This is a test tech article description"},
                    {"url", "https://example.com/tech-article"},
                    {"urlToImage", "https://example.com/tech-article/image.jpg"},
                    {"publishedAt", "2023-07-05T08:30:00Z"},
                    {"content", "This is the content of the tech article..."}
                },
                {
                    {"source", {{"id", "business-insider"}, {"name", "Business Insider"}}},
                    {"author", "Jane Smith"},
                    {"title", "Test Business Article"},
                    {"description", "This is a test business article description"},
                    {"url", "https://example.com/business-article"},
                    {"urlToImage", "https://example.com/business-article/image.jpg"},
                    {"publishedAt", "2023-07-06T10:15:00Z"},
                    {"content", "This is the content of the business article..."}
                }
            })}
        };
        return response.dump();
    }

    // Helper to create a sample NewsAPI.org error response
    std::string createErrorResponse() {
        nlohmann::json response = {
            {"status", "error"},
            {"code", "apiKeyInvalid"},
            {"message", "Your API key is invalid or incorrect."}
        };
        return response.dump();
    }

    // Create an adapter instance for testing
    NewsApiOrgAdapter adapter;
};

// Test successful fetching of articles
TEST_F(NewsApiOrgAdapterTest, FetchArticlesSuccess) {
    // Override HttpClient::get with our mock
    auto originalGet = HttpClient::get;
    HttpClient::get = MockHttpClient::get;

    // Set up mock response
    MockHttpClient::setMockResponse(createSampleResponse());

    // Call method under test
    auto articles = adapter.fetchArticles();

    // Verify results
    ASSERT_EQ(articles.size(), 2);
    EXPECT_EQ(articles[0]["title"], "Test Tech Article");
    EXPECT_EQ(articles[0]["source"], "TechCrunch");
    EXPECT_EQ(articles[1]["title"], "Test Business Article");
    EXPECT_EQ(articles[1]["source"], "Business Insider");

    // Restore original HttpClient::get
    HttpClient::get = originalGet;
}

// Test handling of empty articles array
TEST_F(NewsApiOrgAdapterTest, FetchArticlesEmptyArray) {
    // Override HttpClient::get with our mock
    auto originalGet = HttpClient::get;
    HttpClient::get = MockHttpClient::get;

    // Set up mock response with empty articles array
    nlohmann::json response = {
        {"status", "ok"},
        {"totalResults", 0},
        {"articles", nlohmann::json::array()}
    };
    MockHttpClient::setMockResponse(response.dump());

    // Call method under test
    auto articles = adapter.fetchArticles();

    // Verify results
    EXPECT_TRUE(articles.empty());

    // Restore original HttpClient::get
    HttpClient::get = originalGet;
}

// Test handling of API error
TEST_F(NewsApiOrgAdapterTest, FetchArticlesApiError) {
    // Override HttpClient::get with our mock
    auto originalGet = HttpClient::get;
    HttpClient::get = MockHttpClient::get;

    // Set up mock error response
    MockHttpClient::setMockResponse(createErrorResponse());

    // Call method under test - should handle error gracefully
    auto articles = adapter.fetchArticles();

    // Verify results (should be an empty array)
    EXPECT_TRUE(articles.empty());

    // Restore original HttpClient::get
    HttpClient::get = originalGet;
}

// Test handling of network error
TEST_F(NewsApiOrgAdapterTest, FetchArticlesNetworkError) {
    // Override HttpClient::get with our mock
    auto originalGet = HttpClient::get;
    HttpClient::get = MockHttpClient::get;

    // Set up mock to throw exception
    MockHttpClient::setShouldThrow(true);

    // Call method under test - should catch exception
    EXPECT_NO_THROW({
        auto articles = adapter.fetchArticles();
        EXPECT_TRUE(articles.empty());
    });

    // Restore original HttpClient::get
    HttpClient::get = originalGet;
}

// Test handling of malformed JSON
TEST_F(NewsApiOrgAdapterTest, FetchArticlesMalformedJson) {
    // Override HttpClient::get with our mock
    auto originalGet = HttpClient::get;
    HttpClient::get = MockHttpClient::get;

    // Set up mock with malformed JSON response
    MockHttpClient::setMockResponse("This is not valid JSON");

    // Call method under test - should handle error gracefully
    EXPECT_NO_THROW({
        auto articles = adapter.fetchArticles();
        EXPECT_TRUE(articles.empty());
    });

    // Restore original HttpClient::get
    HttpClient::get = originalGet;
}

// Test source name retrieval
TEST_F(NewsApiOrgAdapterTest, GetSourceName) {
    std::string sourceName = adapter.getSourceName();
    EXPECT_EQ(sourceName, "newsapi.org");
}

// Test article format conversion
TEST_F(NewsApiOrgAdapterTest, ArticleFormatConversion) {
    // Override HttpClient::get with our mock
    auto originalGet = HttpClient::get;
    HttpClient::get = MockHttpClient::get;

    // Set up mock response
    MockHttpClient::setMockResponse(createSampleResponse());

    // Call method under test
    auto articles = adapter.fetchArticles();

    // Verify that each article has the expected fields
    for (const auto& article : articles) {
        EXPECT_TRUE(article.contains("title"));
        EXPECT_TRUE(article.contains("description"));
        EXPECT_TRUE(article.contains("url"));
        EXPECT_TRUE(article.contains("image_url"));
        EXPECT_TRUE(article.contains("source"));
        EXPECT_TRUE(article.contains("content"));
        EXPECT_TRUE(article.contains("language"));
        EXPECT_TRUE(article.contains("locale"));
    }

    // Restore original HttpClient::get
    HttpClient::get = originalGet;
}