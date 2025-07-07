#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../adapters/TheNewsApiAdapter.hpp"
#include "../../utils/HttpClient.hpp"
#include <nlohmann/json.hpp>

// Mock for HttpClient to avoid real API calls - same as in NewsApiOrgAdapterTest
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

// Test fixture for TheNewsApiAdapter
class TheNewsApiAdapterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset mock behavior before each test
        MockHttpClient::setShouldThrow(false);
        MockHttpClient::setMockResponse("{}");
    }

    void TearDown() override {
        // Clean up after each test
    }

    // Helper to create a sample TheNewsAPI.com response
    std::string createSampleResponse() {
        nlohmann::json response = {
            {"meta", {
                {"found", 2},
                {"returned", 2},
                {"limit", 10},
                {"page", 1}
            }},
            {"data", nlohmann::json::array({
                {
                    {"uuid", "abc123"},
                    {"title", "Test Tech Article"},
                    {"description", "This is a test tech article description"},
                    {"keywords", "tech,test,article"},
                    {"snippet", "This is a snippet of the tech article..."},
                    {"url", "https://example.com/tech-article"},
                    {"image_url", "https://example.com/tech-article/image.jpg"},
                    {"language", "en"},
                    {"published_at", "2023-07-05T08:30:00Z"},
                    {"source", "Tech News"},
                    {"categories", nlohmann::json::array({"technology", "innovation"})},
                    {"relevance_score", 22.43}
                },
                {
                    {"uuid", "def456"},
                    {"title", "Test Business Article"},
                    {"description", "This is a test business article description"},
                    {"keywords", "business,test,article"},
                    {"snippet", "This is a snippet of the business article..."},
                    {"url", "https://example.com/business-article"},
                    {"image_url", "https://example.com/business-article/image.jpg"},
                    {"language", "en"},
                    {"published_at", "2023-07-06T10:15:00Z"},
                    {"source", "Business Daily"},
                    {"categories", nlohmann::json::array({"business", "finance"})},
                    {"relevance_score", 19.87}
                }
            })}
        };
        return response.dump();
    }

    // Helper to create a sample TheNewsAPI.com error response
    std::string createErrorResponse() {
        nlohmann::json response = {
            {"error", {
                {"code", 401},
                {"message", "Invalid API token"}
            }}
        };
        return response.dump();
    }

    // Create an adapter instance for testing
    TheNewsApiAdapter adapter;
};

// Test successful fetching of articles
TEST_F(TheNewsApiAdapterTest, FetchArticlesSuccess) {
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
    EXPECT_EQ(articles[0]["source"], "Tech News");
    EXPECT_EQ(articles[1]["title"], "Test Business Article");
    EXPECT_EQ(articles[1]["source"], "Business Daily");

    // Restore original HttpClient::get
    HttpClient::get = originalGet;
}

// Test handling of empty data array
TEST_F(TheNewsApiAdapterTest, FetchArticlesEmptyArray) {
    // Override HttpClient::get with our mock
    auto originalGet = HttpClient::get;
    HttpClient::get = MockHttpClient::get;

    // Set up mock response with empty data array
    nlohmann::json response = {
        {"meta", {
            {"found", 0},
            {"returned", 0},
            {"limit", 10},
            {"page", 1}
        }},
        {"data", nlohmann::json::array()}
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
TEST_F(TheNewsApiAdapterTest, FetchArticlesApiError) {
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
TEST_F(TheNewsApiAdapterTest, FetchArticlesNetworkError) {
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
TEST_F(TheNewsApiAdapterTest, FetchArticlesMalformedJson) {
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
TEST_F(TheNewsApiAdapterTest, GetSourceName) {
    std::string sourceName = adapter.getSourceName();
    EXPECT_EQ(sourceName, "thenewsapi.com");
}

// Test article format conversion
TEST_F(TheNewsApiAdapterTest, ArticleFormatConversion) {
    // Override HttpClient::get with our mock
    auto originalGet = HttpClient::get;
    HttpClient::get = MockHttpClient::get;

    // Set up mock response
    MockHttpClient::setMockResponse(createSampleResponse());

    // Call method under test
    auto articles = adapter.fetchArticles();

    // Verify that each article has the expected fields
    for (const auto& article : articles) {
        EXPECT_TRUE(article.contains("uuid"));
        EXPECT_TRUE(article.contains("title"));
        EXPECT_TRUE(article.contains("description"));
        EXPECT_TRUE(article.contains("url"));
        EXPECT_TRUE(article.contains("image_url"));
        EXPECT_TRUE(article.contains("source"));
        EXPECT_TRUE(article.contains("content"));
        EXPECT_TRUE(article.contains("language"));
        EXPECT_TRUE(article.contains("locale"));
        EXPECT_TRUE(article.contains("category"));
    }

    // Restore original HttpClient::get
    HttpClient::get = originalGet;
}

// Test category extraction from response
TEST_F(TheNewsApiAdapterTest, CategoryExtraction) {
    // Override HttpClient::get with our mock
    auto originalGet = HttpClient::get;
    HttpClient::get = MockHttpClient::get;

    // Set up mock response
    MockHttpClient::setMockResponse(createSampleResponse());

    // Call method under test
    auto articles = adapter.fetchArticles();

    // Verify that categories were properly extracted
    EXPECT_EQ(articles[0]["category"], "technology");
    EXPECT_EQ(articles[1]["category"], "business");

    // Restore original HttpClient::get
    HttpClient::get = originalGet;
}