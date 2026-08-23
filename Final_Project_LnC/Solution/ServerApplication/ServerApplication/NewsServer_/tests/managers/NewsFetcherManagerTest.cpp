#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../managers/NewsFetcherManager.hpp"
#include "../../adapters/INewsApiAdapter.hpp"
#include "../../services/ArticleService.hpp"
#include <nlohmann/json.hpp>

// Mock implementation of INewsApiAdapter
class MockNewsApiAdapter : public INewsApiAdapter {
public:
    MOCK_METHOD(nlohmann::json, fetchArticles, (), (override));
    MOCK_METHOD(std::string, getSourceName, (), (const, override));
};

// Mock for ArticleService
class MockArticleService {
public:
    static bool storeArticleCalled;
    static nlohmann::json lastArticle;
    
    static void storeArticle(const nlohmann::json& article) {
        storeArticleCalled = true;
        lastArticle = article;
    }
    
    static void reset() {
        storeArticleCalled = false;
        lastArticle = nlohmann::json();
    }
};

bool MockArticleService::storeArticleCalled = false;
nlohmann::json MockArticleService::lastArticle = nlohmann::json();

// Test fixture for NewsFetcherManager
class NewsFetcherManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset mocks before each test
        MockArticleService::reset();
    }

    void TearDown() override {
        // Clean up after each test
    }

    // Helper to create sample article data
    nlohmann::json createSampleArticles() {
        nlohmann::json articles = nlohmann::json::array();
        
        articles.push_back({
            {"title", "Sample Tech Article"},
            {"description", "This is a sample tech article description"},
            {"content", "Full content of the tech article..."},
            {"url", "https://example.com/tech-article"},
            {"image_url", "https://example.com/tech-article/image.jpg"},
            {"source", "Tech News"},
            {"category", "technology"},
            {"language", "en"},
            {"locale", "us"}
        });
        
        articles.push_back({
            {"title", "Sample Business Article"},
            {"description", "This is a sample business article description"},
            {"content", "Full content of the business article..."},
            {"url", "https://example.com/business-article"},
            {"image_url", "https://example.com/business-article/image.jpg"},
            {"source", "Business Daily"},
            {"category", "business"},
            {"language", "en"},
            {"locale", "us"}
        });
        
        return articles;
    }

    // Create manager for testing
    NewsFetcherManager manager;
};

// Test registering adapters
TEST_F(NewsFetcherManagerTest, RegisterAdapter) {
    // Create mock adapter
    auto adapter = std::make_shared<testing::NiceMock<MockNewsApiAdapter>>();
    
    // Verify that registering adapter doesn't throw
    EXPECT_NO_THROW(manager.registerAdapter(adapter));
}

// Test fetching articles from single adapter
TEST_F(NewsFetcherManagerTest, FetchFromSingleAdapter) {
    // Override ArticleService::storeArticle with our mock
    auto originalStoreArticle = ArticleService::storeArticle;
    ArticleService::storeArticle = MockArticleService::storeArticle;
    
    // Create mock adapter
    auto adapter = std::make_shared<testing::NiceMock<MockNewsApiAdapter>>();
    
    // Configure mock behavior
    auto sampleArticles = createSampleArticles();
    ON_CALL(*adapter, fetchArticles()).WillByDefault(testing::Return(sampleArticles));
    ON_CALL(*adapter, getSourceName()).WillByDefault(testing::Return("Test Source"));
    
    // Register adapter with manager
    manager.registerAdapter(adapter);
    
    // Call method under test
    manager.fetchAll();
    
    // Verify that ArticleService::storeArticle was called
    EXPECT_TRUE(MockArticleService::storeArticleCalled);
    
    // Restore original ArticleService::storeArticle
    ArticleService::storeArticle = originalStoreArticle;
}

// Test fetching articles from multiple adapters
TEST_F(NewsFetcherManagerTest, FetchFromMultipleAdapters) {
    // Override ArticleService::storeArticle with our mock
    auto originalStoreArticle = ArticleService::storeArticle;
    ArticleService::storeArticle = MockArticleService::storeArticle;
    
    // Create mock adapters
    auto adapter1 = std::make_shared<testing::NiceMock<MockNewsApiAdapter>>();
    auto adapter2 = std::make_shared<testing::NiceMock<MockNewsApiAdapter>>();
    
    // Configure mock behavior for first adapter
    nlohmann::json articles1 = nlohmann::json::array();
    articles1.push_back({
        {"title", "Article from Source 1"},
        {"source", "Source 1"}
    });
    ON_CALL(*adapter1, fetchArticles()).WillByDefault(testing::Return(articles1));
    ON_CALL(*adapter1, getSourceName()).WillByDefault(testing::Return("Source 1"));
    
    // Configure mock behavior for second adapter
    nlohmann::json articles2 = nlohmann::json::array();
    articles2.push_back({
        {"title", "Article from Source 2"},
        {"source", "Source 2"}
    });
    ON_CALL(*adapter2, fetchArticles()).WillByDefault(testing::Return(articles2));
    ON_CALL(*adapter2, getSourceName()).WillByDefault(testing::Return("Source 2"));
    
    // Register adapters with manager
    manager.registerAdapter(adapter1);
    manager.registerAdapter(adapter2);
    
    // Call method under test
    manager.fetchAll();
    
    // Verify that ArticleService::storeArticle was called
    EXPECT_TRUE(MockArticleService::storeArticleCalled);
    
    // Restore original ArticleService::storeArticle
    ArticleService::storeArticle = originalStoreArticle;
}

// Test handling adapter exceptions
TEST_F(NewsFetcherManagerTest, HandleAdapterException) {
    // Override ArticleService::storeArticle with our mock
    auto originalStoreArticle = ArticleService::storeArticle;
    ArticleService::storeArticle = MockArticleService::storeArticle;
    
    // Create mock adapters
    auto adapter1 = std::make_shared<testing::NiceMock<MockNewsApiAdapter>>();
    auto adapter2 = std::make_shared<testing::NiceMock<MockNewsApiAdapter>>();
    
    // Configure first adapter to throw exception
    ON_CALL(*adapter1, fetchArticles()).WillByDefault(testing::Throw(std::runtime_error("Network error")));
    ON_CALL(*adapter1, getSourceName()).WillByDefault(testing::Return("Source 1"));
    
    // Configure second adapter to return valid data
    nlohmann::json articles2 = nlohmann::json::array();
    articles2.push_back({
        {"title", "Article from Source 2"},
        {"source", "Source 2"}
    });
    ON_CALL(*adapter2, fetchArticles()).WillByDefault(testing::Return(articles2));
    ON_CALL(*adapter2, getSourceName()).WillByDefault(testing::Return("Source 2"));
    
    // Register adapters with manager
    manager.registerAdapter(adapter1);
    manager.registerAdapter(adapter2);
    
    // Call method under test - should not throw exception
    EXPECT_NO_THROW(manager.fetchAll());
    
    // Verify that ArticleService::storeArticle was still called for second adapter
    EXPECT_TRUE(MockArticleService::storeArticleCalled);
    
    // Restore original ArticleService::storeArticle
    ArticleService::storeArticle = originalStoreArticle;
}

// Test handling empty articles array
TEST_F(NewsFetcherManagerTest, HandleEmptyArticles) {
    // Override ArticleService::storeArticle with our mock
    auto originalStoreArticle = ArticleService::storeArticle;
    ArticleService::storeArticle = MockArticleService::storeArticle;
    
    // Create mock adapter
    auto adapter = std::make_shared<testing::NiceMock<MockNewsApiAdapter>>();
    
    // Configure adapter to return empty array
    ON_CALL(*adapter, fetchArticles()).WillByDefault(testing::Return(nlohmann::json::array()));
    ON_CALL(*adapter, getSourceName()).WillByDefault(testing::Return("Test Source"));
    
    // Register adapter with manager
    manager.registerAdapter(adapter);
    
    // Call method under test
    manager.fetchAll();
    
    // Verify that ArticleService::storeArticle was not called
    EXPECT_FALSE(MockArticleService::storeArticleCalled);
    
    // Restore original ArticleService::storeArticle
    ArticleService::storeArticle = originalStoreArticle;
}

// Test with no adapters registered
TEST_F(NewsFetcherManagerTest, NoAdaptersRegistered) {
    // Override ArticleService::storeArticle with our mock
    auto originalStoreArticle = ArticleService::storeArticle;
    ArticleService::storeArticle = MockArticleService::storeArticle;
    
    // Call method under test with no adapters registered
    manager.fetchAll();
    
    // Verify that ArticleService::storeArticle was not called
    EXPECT_FALSE(MockArticleService::storeArticleCalled);
    
    // Restore original ArticleService::storeArticle
    ArticleService::storeArticle = originalStoreArticle;
}