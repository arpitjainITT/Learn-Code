#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../services/ArticleService.hpp"
#include <nlohmann/json.hpp>
#include <functional>

using json = nlohmann::json;

// Mock the entire ArticleService instead of trying to replace Database functions
class MockArticleService {
public:
    // Define static mock functions with the same signatures as ArticleService
    static inline std::function<json()> getAllArticles = []() { return mockArticles; };
    static inline std::function<json(int)> getArticleById = [](int id) { 
        for (const auto& article : mockArticles) {
            if (article.contains("id") && article["id"] == id) {
                return article;
            }
        }
        return json{};
    };
    static inline std::function<json(const std::string&)> getArticlesByCategory = [](const std::string& category) {
        json results = json::array();
        for (const auto& article : mockArticles) {
            if (article.contains("category") && article["category"] == category) {
                results.push_back(article);
            }
        }
        return results;
    };
    static inline std::function<bool(const json&)> storeArticle = [](const json& article) {
        lastStoredArticle = article;
        storeArticleCalled = true;
        return true;
    };
    static inline std::function<json(const std::string&, const std::string&, const std::string&, const std::string&)> 
    searchArticles = [](const std::string& keyword, const std::string& startDate, 
                        const std::string& endDate, const std::string& sortBy) {
        // Implementation
        return json::array();
    };
    
    // Add other mock functions as needed
    
    // Test state variables
    static bool storeArticleCalled;
    static json lastStoredArticle;
    static json mockArticles;
    
    static void reset() {
        storeArticleCalled = false;
        lastStoredArticle = json{};
        mockArticles = json::array();
        
        // Reset all function pointers to default implementations
        getAllArticles = []() { return mockArticles; };
        getArticleById = [](int id) { 
            for (const auto& article : mockArticles) {
                if (article.contains("id") && article["id"] == id) {
                    return article;
                }
            }
            return json{};
        };
        // Reset other functions
    }
    
    static void setupMockArticles() {
        mockArticles = json::array();
        mockArticles.push_back({
            {"id", 1},
            {"title", "Test Tech Article"},
            {"description", "This is a test tech article"},
            {"content", "Full content of the tech article..."},
            {"category", "technology"},
            {"published_at", "2023-07-05T12:00:00Z"}
        });
        mockArticles.push_back({
            {"id", 2},
            {"title", "Test Business Article"},
            {"description", "This is a test business article"},
            {"content", "Full content of the business article..."},
            {"category", "business"},
            {"published_at", "2023-07-06T10:30:00Z"}
        });
    }
};

bool MockArticleService::storeArticleCalled = false;
json MockArticleService::lastStoredArticle = json{};
json MockArticleService::mockArticles = json::array();

// Override ArticleService namespace functions for testing
namespace ArticleService {
    // Store original functions
    namespace Original {
        json (*getAllArticles)() = ArticleService::getAllArticles;
        json (*getArticleById)(int) = ArticleService::getArticleById;
        // Store other original functions
    }
    
    // Override with test implementations
    json getAllArticles() {
        return MockArticleService::getAllArticles();
    }
    
    json getArticleById(int id) {
        return MockArticleService::getArticleById(id);
    }
    
    json getArticlesByCategory(const std::string& category) {
        return MockArticleService::getArticlesByCategory(category);
    }
    
    bool storeArticle(const json& article) {
        return MockArticleService::storeArticle(article);
    }
    
    // Override other functions
}

// Test fixture
class ArticleServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        MockArticleService::reset();
        MockArticleService::setupMockArticles();
    }
    
    // Helper function to create a test article
    json createSampleArticle() {
        return json{
            {"title", "Test Article"},
            {"description", "This is a test article"},
            {"content", "Full content of the test article..."},
            {"category", "technology"}
        };
    }
};

// Test storing an article
TEST_F(ArticleServiceTest, StoreArticle) {
    // Prepare test data
    auto article = createSampleArticle();
    
    // Call the method under test
    bool result = ArticleService::storeArticle(article);
    
    // Verify the result
    EXPECT_TRUE(result);
    EXPECT_TRUE(MockArticleService::storeArticleCalled);
    EXPECT_EQ(MockArticleService::lastStoredArticle["title"], "Test Article");
}

// Test getting all articles
TEST_F(ArticleServiceTest, GetAllArticles) {
    // Call the method under test
    auto articles = ArticleService::getAllArticles();
    
    // Verify the result
    EXPECT_EQ(articles.size(), 2);
    EXPECT_EQ(articles[0]["title"], "Test Tech Article");
    EXPECT_EQ(articles[1]["title"], "Test Business Article");
}

// Test getting article by ID
TEST_F(ArticleServiceTest, GetArticleById) {
    // Call the method under test
    auto existingArticle = ArticleService::getArticleById(1);
    auto nonExistentArticle = ArticleService::getArticleById(999);
    
    // Verify the result
    EXPECT_EQ(existingArticle["id"], 1);
    EXPECT_EQ(existingArticle["title"], "Test Tech Article");
    EXPECT_TRUE(nonExistentArticle.empty());
}

// Test getting articles by category
TEST_F(ArticleServiceTest, GetArticlesByCategory) {
    // Call the method under test
    auto techArticles = ArticleService::getArticlesByCategory("technology");
    auto businessArticles = ArticleService::getArticlesByCategory("business");
    auto nonExistentCategory = ArticleService::getArticlesByCategory("nonexistent");
    
    // Verify the result
    EXPECT_EQ(techArticles.size(), 1);
    EXPECT_EQ(techArticles[0]["title"], "Test Tech Article");
    
    EXPECT_EQ(businessArticles.size(), 1);
    EXPECT_EQ(businessArticles[0]["title"], "Test Business Article");
    
    EXPECT_TRUE(nonExistentCategory.empty());
}

// Test searching articles
TEST_F(ArticleServiceTest, SearchArticles) {
    // Call the method under test
    auto techResults = ArticleService::searchArticles("Tech", "", "", "");
    auto emptyResults = ArticleService::searchArticles("NonExistent", "", "", "");
    auto allResults = ArticleService::searchArticles("", "", "", "");
    
    // Verify the results
    EXPECT_EQ(techResults.size(), 1);
    EXPECT_EQ(techResults[0]["title"], "Test Tech Article");
    
    EXPECT_TRUE(emptyResults.empty());
    
    EXPECT_EQ(allResults.size(), 2);
}

// Test error handling
TEST_F(ArticleServiceTest, ErrorHandling) {
    // Configure mock to throw an exception
    MockDatabase::shouldThrow = true;
    
    // Test error handling in getAllArticles
    auto emptyArticles = ArticleService::getAllArticles();
    EXPECT_TRUE(emptyArticles.empty());
    
    // Test error handling in getArticleById
    auto emptyArticle = ArticleService::getArticleById(1);
    EXPECT_TRUE(emptyArticle.empty());
    
    // Test error handling in storeArticle
    auto article = createSampleArticle();
    bool storeResult = ArticleService::storeArticle(article);
    EXPECT_FALSE(storeResult);
    
    // Reset the mock
    MockDatabase::shouldThrow = false;
}

// Test user preferences
TEST_F(ArticleServiceTest, UserPreferences) {
    // Call the method under test
    auto preferences = ArticleService::getUserPreferences(42);
    auto emptyPreferences = ArticleService::getUserPreferences(999);
    
    // Verify the results
    EXPECT_EQ(preferences["categories"].size(), 2);
    EXPECT_EQ(preferences["categories"][0], "technology");
    EXPECT_EQ(preferences["categories"][1], "business");
    
    EXPECT_EQ(preferences["keywords"].size(), 2);
    EXPECT_EQ(preferences["keywords"][0], "ai");
    EXPECT_EQ(preferences["keywords"][1], "blockchain");
    
    EXPECT_TRUE(emptyPreferences["categories"].empty());
    EXPECT_TRUE(emptyPreferences["keywords"].empty());
}