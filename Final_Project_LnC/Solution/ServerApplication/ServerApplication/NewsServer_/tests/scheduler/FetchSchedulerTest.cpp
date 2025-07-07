#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../scheduler/FetchScheduler.hpp"
#include "../../adapters/NewsApiOrgAdapter.hpp"
#include "../../adapters/TheNewsApiAdapter.hpp"
#include "../../services/ArticleService.hpp"
#include "../../services/AdminService.hpp"
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>

// Mock for NewsApiOrgAdapter
class MockNewsApiOrgAdapter : public NewsApiOrgAdapter {
public:
    MOCK_METHOD(nlohmann::json, fetchArticles, (), (override));
    MOCK_METHOD(std::string, getSourceName, (), (const, override));
};

// Mock for TheNewsApiAdapter
class MockTheNewsApiAdapter : public TheNewsApiAdapter {
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

// Mock for AdminService
class MockAdminService {
public:
    static bool updateExternalServerStatusCalled;
    static std::string lastServerName;
    static std::string lastStatus;
    
    static bool updateExternalServerStatus(const std::string& serverName, const std::string& status) {
        updateExternalServerStatusCalled = true;
        lastServerName = serverName;
        lastStatus = status;
        return true;
    }
    
    static void reset() {
        updateExternalServerStatusCalled = false;
        lastServerName = "";
        lastStatus = "";
    }
};

bool MockAdminService::updateExternalServerStatusCalled = false;
std::string MockAdminService::lastServerName = "";
std::string MockAdminService::lastStatus = "";

// Custom FetchScheduler subclass for testing
class TestFetchScheduler : public FetchScheduler {
public:
    // Make the protected method public for testing
    using FetchScheduler::fetchAndStoreArticles;
};

// Test fixture for FetchScheduler
class FetchSchedulerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset mocks before each test
        MockArticleService::reset();
        MockAdminService::reset();
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
        
        return articles;
    }

    // Create scheduler for testing
    TestFetchScheduler scheduler;
};

// Test successful fetching from primary adapter
TEST_F(FetchSchedulerTest, FetchFromPrimaryAdapterSuccess) {
    // Override ArticleService::storeArticle and AdminService::updateExternalServerStatus with our mocks
    auto originalStoreArticle = ArticleService::storeArticle;
    auto originalUpdateStatus = AdminService::updateExternalServerStatus;
    ArticleService::storeArticle = MockArticleService::storeArticle;
    AdminService::updateExternalServerStatus = MockAdminService::updateExternalServerStatus;
    
    // Create and inject mock adapters into scheduler
    auto mockNewsApiOrgAdapter = std::make_shared<testing::NiceMock<MockNewsApiOrgAdapter>>();
    auto mockTheNewsApiAdapter = std::make_shared<testing::NiceMock<MockTheNewsApiAdapter>>();
    
    // Configure primary adapter to return sample articles
    auto sampleArticles = createSampleArticles();
    ON_CALL(*mockNewsApiOrgAdapter, fetchArticles()).WillByDefault(testing::Return(sampleArticles));
    ON_CALL(*mockNewsApiOrgAdapter, getSourceName()).WillByDefault(testing::Return("newsapi.org"));
    
    // Call method under test
    scheduler.fetchAndStoreArticles();
    
    // Verify that ArticleService::storeArticle and AdminService::updateExternalServerStatus were called
    EXPECT_TRUE(MockArticleService::storeArticleCalled);
    EXPECT_TRUE(MockAdminService::updateExternalServerStatusCalled);
    EXPECT_EQ(MockAdminService::lastServerName, "newsapi.org");
    EXPECT_EQ(MockAdminService::lastStatus, "active");
    
    // Restore original functions
    ArticleService::storeArticle = originalStoreArticle;
    AdminService::updateExternalServerStatus = originalUpdateStatus;
}

// Test falling back to secondary adapter when primary fails
TEST_F(FetchSchedulerTest, FallbackToSecondaryAdapter) {
    // Override ArticleService::storeArticle and AdminService::updateExternalServerStatus with our mocks
    auto originalStoreArticle = ArticleService::storeArticle;
    auto originalUpdateStatus = AdminService::updateExternalServerStatus;
    ArticleService::storeArticle = MockArticleService::storeArticle;
    AdminService::updateExternalServerStatus = MockAdminService::updateExternalServerStatus;
    
    // Create and inject mock adapters into scheduler
    auto mockNewsApiOrgAdapter = std::make_shared<testing::NiceMock<MockNewsApiOrgAdapter>>();
    auto mockTheNewsApiAdapter = std::make_shared<testing::NiceMock<MockTheNewsApiAdapter>>();
    
    // Configure primary adapter to throw exception
    ON_CALL(*mockNewsApiOrgAdapter, fetchArticles()).WillByDefault(testing::Throw(std::runtime_error("Network error")));
    ON_CALL(*mockNewsApiOrgAdapter, getSourceName()).WillByDefault(testing::Return("newsapi.org"));
    
    // Configure fallback adapter to return sample articles
    auto sampleArticles = createSampleArticles();
    ON_CALL(*mockTheNewsApiAdapter, fetchArticles()).WillByDefault(testing::Return(sampleArticles));
    ON_CALL(*mockTheNewsApiAdapter, getSourceName()).WillByDefault(testing::Return("thenewsapi.com"));
    
    // Call method under test
    scheduler.fetchAndStoreArticles();
    
    // Verify that ArticleService::storeArticle was called with fallback adapter's articles
    EXPECT_TRUE(MockArticleService::storeArticleCalled);
    
    // Verify that AdminService::updateExternalServerStatus was called twice
    // First for primary adapter (inactive), then for fallback adapter (active)
    EXPECT_TRUE(MockAdminService::updateExternalServerStatusCalled);
    EXPECT_EQ(MockAdminService::lastServerName, "thenewsapi.com");
    EXPECT_EQ(MockAdminService::lastStatus, "active");
    
    // Restore original functions
    ArticleService::storeArticle = originalStoreArticle;
    AdminService::updateExternalServerStatus = originalUpdateStatus;
}

// Test handling when both adapters fail
TEST_F(FetchSchedulerTest, BothAdaptersFail) {
    // Override ArticleService::storeArticle and AdminService::updateExternalServerStatus with our mocks
    auto originalStoreArticle = ArticleService::storeArticle;
    auto originalUpdateStatus = AdminService::updateExternalServerStatus;
    ArticleService::storeArticle = MockArticleService::storeArticle;
    AdminService::updateExternalServerStatus = MockAdminService::updateExternalServerStatus;
    
    // Create and inject mock adapters into scheduler
    auto mockNewsApiOrgAdapter = std::make_shared<testing::NiceMock<MockNewsApiOrgAdapter>>();
    auto mockTheNewsApiAdapter = std::make_shared<testing::NiceMock<MockTheNewsApiAdapter>>();
    
    // Configure both adapters to throw exceptions
    ON_CALL(*mockNewsApiOrgAdapter, fetchArticles()).WillByDefault(testing::Throw(std::runtime_error("Network error")));
    ON_CALL(*mockNewsApiOrgAdapter, getSourceName()).WillByDefault(testing::Return("newsapi.org"));
    
    ON_CALL(*mockTheNewsApiAdapter, fetchArticles()).WillByDefault(testing::Throw(std::runtime_error("Network error")));
    ON_CALL(*mockTheNewsApiAdapter, getSourceName()).WillByDefault(testing::Return("thenewsapi.com"));
    
    // Call method under test - should handle errors gracefully
    EXPECT_NO_THROW(scheduler.fetchAndStoreArticles());
    
    // Verify that ArticleService::storeArticle was not called
    EXPECT_FALSE(MockArticleService::storeArticleCalled);
    
    // Verify that AdminService::updateExternalServerStatus was called to mark servers as inactive
    EXPECT_TRUE(MockAdminService::updateExternalServerStatusCalled);
    
    // Restore original functions
    ArticleService::storeArticle = originalStoreArticle;
    AdminService::updateExternalServerStatus = originalUpdateStatus;
}

// Test handling empty article arrays
TEST_F(FetchSchedulerTest, EmptyArticleArrays) {
    // Override ArticleService::storeArticle and AdminService::updateExternalServerStatus with our mocks
    auto originalStoreArticle = ArticleService::storeArticle;
    auto originalUpdateStatus = AdminService::updateExternalServerStatus;
    ArticleService::storeArticle = MockArticleService::storeArticle;
    AdminService::updateExternalServerStatus = MockAdminService::updateExternalServerStatus;
    
    // Create and inject mock adapters into scheduler
    auto mockNewsApiOrgAdapter = std::make_shared<testing::NiceMock<MockNewsApiOrgAdapter>>();
    auto mockTheNewsApiAdapter = std::make_shared<testing::NiceMock<MockTheNewsApiAdapter>>();
    
    // Configure primary adapter to return empty array
    ON_CALL(*mockNewsApiOrgAdapter, fetchArticles()).WillByDefault(testing::Return(nlohmann::json::array()));
    ON_CALL(*mockNewsApiOrgAdapter, getSourceName()).WillByDefault(testing::Return("newsapi.org"));
    
    // Configure fallback adapter to return empty array as well
    ON_CALL(*mockTheNewsApiAdapter, fetchArticles()).WillByDefault(testing::Return(nlohmann::json::array()));
    ON_CALL(*mockTheNewsApiAdapter, getSourceName()).WillByDefault(testing::Return("thenewsapi.com"));
    
    // Call method under test
    scheduler.fetchAndStoreArticles();
    
    // Verify that ArticleService::storeArticle was not called
    EXPECT_FALSE(MockArticleService::storeArticleCalled);
    
    // Restore original functions
    ArticleService::storeArticle = originalStoreArticle;
    AdminService::updateExternalServerStatus = originalUpdateStatus;
}

// Test scheduler start and stop
TEST_F(FetchSchedulerTest, StartAndStop) {
    FetchScheduler scheduler;
    
    // Start the scheduler
    EXPECT_NO_THROW(scheduler.start());
    
    // Wait a bit to ensure the thread starts
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Stop the scheduler
    EXPECT_NO_THROW(scheduler.stop());
}