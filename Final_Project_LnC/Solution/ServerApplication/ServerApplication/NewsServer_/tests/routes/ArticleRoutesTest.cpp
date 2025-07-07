#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../routes/article_routes.hpp"
#include "../../services/ArticleService.hpp"
#include <pistache/router.h>
#include <pistache/http.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace Pistache;

// Mock for ArticleService
class MockArticleService {
public:
    static json getAllArticles() {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        return mockArticles;
    }

    static json getArticleById(int articleId) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        for (const auto& article : mockArticles) {
            if (article["id"] == articleId) {
                return article;
            }
        }
        return json{};
    }

    static json getArticlesByCategory(const std::string& category) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        json result = json::array();
        for (const auto& article : mockArticles) {
            if (article["category"] == category) {
                result.push_back(article);
            }
        }
        return result;
    }

    static bool saveArticle(int userId, int articleId) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        lastSavedArticle = std::make_pair(userId, articleId);
        return true;
    }

    static bool deleteSavedArticle(int userId, int articleId) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        lastDeletedArticle = std::make_pair(userId, articleId);
        return true;
    }

    static json getSavedArticlesForUser(int userId) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        // In a real test this would return saved articles for the user
        // Here we'll just return the first 2 mock articles
        json result = json::array();
        if (mockArticles.size() >= 2) {
            result.push_back(mockArticles[0]);
            result.push_back(mockArticles[1]);
        }
        return result;
    }

    static bool likeArticle(int userId, int articleId) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        lastLikedArticle = std::make_pair(userId, articleId);
        return true;
    }

    static bool dislikeArticle(int userId, int articleId) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        lastDislikedArticle = std::make_pair(userId, articleId);
        return true;
    }

    static json searchArticles(const std::string& keyword, const std::string& startDate, 
                               const std::string& endDate, const std::string& sortBy) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        lastSearchParams = {keyword, startDate, endDate, sortBy};
        
        // Return articles that match the keyword
        json result = json::array();
        if (!keyword.empty()) {
            for (const auto& article : mockArticles) {
                std::string title = article["title"];
                if (title.find(keyword) != std::string::npos) {
                    result.push_back(article);
                }
            }
        } else {
            result = mockArticles;
        }
        return result;
    }

    static json getReactionStats(int articleId) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        // Mock reaction stats
        return {
            {"likes", 42},
            {"dislikes", 7}
        };
    }

    static void reset() {
        shouldThrow = false;
        mockArticles = json::array();
        lastSavedArticle = std::make_pair(0, 0);
        lastDeletedArticle = std::make_pair(0, 0);
        lastLikedArticle = std::make_pair(0, 0);
        lastDislikedArticle = std::make_pair(0, 0);
        lastSearchParams = {"", "", "", ""};
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
        mockArticles.push_back({
            {"id", 3},
            {"title", "Test Science Article"},
            {"description", "This is a test science article"},
            {"content", "Full content of the science article..."},
            {"category", "science"},
            {"published_at", "2023-07-07T14:15:00Z"}
        });
    }

    static bool shouldThrow;
    static json mockArticles;
    static std::pair<int, int> lastSavedArticle;
    static std::pair<int, int> lastDeletedArticle;
    static std::pair<int, int> lastLikedArticle;
    static std::pair<int, int> lastDislikedArticle;
    static std::tuple<std::string, std::string, std::string, std::string> lastSearchParams;
};

bool MockArticleService::shouldThrow = false;
json MockArticleService::mockArticles = json::array();
std::pair<int, int> MockArticleService::lastSavedArticle = std::make_pair(0, 0);
std::pair<int, int> MockArticleService::lastDeletedArticle = std::make_pair(0, 0);
std::pair<int, int> MockArticleService::lastLikedArticle = std::make_pair(0, 0);
std::pair<int, int> MockArticleService::lastDislikedArticle = std::make_pair(0, 0);
std::tuple<std::string, std::string, std::string, std::string> MockArticleService::lastSearchParams = {"", "", "", ""};

// Test fixture for ArticleRoutes
class ArticleRoutesTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset the mock service before each test
        MockArticleService::reset();
        MockArticleService::setupMockArticles();
        
        // Store original functions
        original_getAllArticles = ArticleService::getAllArticles;
        original_getArticleById = ArticleService::getArticleById;
        original_getArticlesByCategory = ArticleService::getArticlesByCategory;
        original_saveArticle = ArticleService::saveArticle;
        original_deleteSavedArticle = ArticleService::deleteSavedArticle;
        original_getSavedArticlesForUser = ArticleService::getSavedArticlesForUser;
        original_likeArticle = ArticleService::likeArticle;
        original_dislikeArticle = ArticleService::dislikeArticle;
        original_searchArticles = ArticleService::searchArticles;
        original_getReactionStats = ArticleService::getReactionStats;
        
        // Replace with mocks
        ArticleService::getAllArticles = MockArticleService::getAllArticles;
        ArticleService::getArticleById = MockArticleService::getArticleById;
        ArticleService::getArticlesByCategory = MockArticleService::getArticlesByCategory;
        ArticleService::saveArticle = MockArticleService::saveArticle;
        ArticleService::deleteSavedArticle = MockArticleService::deleteSavedArticle;
        ArticleService::getSavedArticlesForUser = MockArticleService::getSavedArticlesForUser;
        ArticleService::likeArticle = MockArticleService::likeArticle;
        ArticleService::dislikeArticle = MockArticleService::dislikeArticle;
        ArticleService::searchArticles = MockArticleService::searchArticles;
        ArticleService::getReactionStats = MockArticleService::getReactionStats;
    }

    void TearDown() override {
        // Restore original functions
        ArticleService::getAllArticles = original_getAllArticles;
        ArticleService::getArticleById = original_getArticleById;
        ArticleService::getArticlesByCategory = original_getArticlesByCategory;
        ArticleService::saveArticle = original_saveArticle;
        ArticleService::deleteSavedArticle = original_deleteSavedArticle;
        ArticleService::getSavedArticlesForUser = original_getSavedArticlesForUser;
        ArticleService::likeArticle = original_likeArticle;
        ArticleService::dislikeArticle = original_dislikeArticle;
        ArticleService::searchArticles = original_searchArticles;
        ArticleService::getReactionStats = original_getReactionStats;
    }

    // Store original function pointers
    std::function<json()> original_getAllArticles;
    std::function<json(int)> original_getArticleById;
    std::function<json(const std::string&)> original_getArticlesByCategory;
    std::function<bool(int, int)> original_saveArticle;
    std::function<bool(int, int)> original_deleteSavedArticle;
    std::function<json(int)> original_getSavedArticlesForUser;
    std::function<bool(int, int)> original_likeArticle;
    std::function<bool(int, int)> original_dislikeArticle;
    std::function<json(const std::string&, const std::string&, const std::string&, const std::string&)> original_searchArticles;
    std::function<json(int)> original_getReactionStats;

    // Helper to create a mock request
    Rest::Request createMockRequest(const std::string& body = "", 
                                  const std::unordered_map<std::string, std::string>& params = {},
                                  const std::unordered_map<std::string, std::string>& query = {}) {
        // Create a mock request with the given body, params, and query
        Http::Request httpReq;
        httpReq.body(body);
        
        for (const auto& [key, value] : query) {
            httpReq.query().add(key, value);
        }
        
        Rest::Request req(httpReq, params);
        return req;
    }

    // Helper to create a response
    Http::ResponseWriter createResponseWriter() {
        auto fd = 0; // Dummy file descriptor
        auto handler = [](const Http::Response&) { };
        return Http::ResponseWriter(fd, handler, Http::Tcp{});
    }
};

// Test getAllArticlesHandler
TEST_F(ArticleRoutesTest, GetAllArticlesHandler) {
    // Create a mock request and response
    auto req = createMockRequest();
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = getAllArticlesHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
}

// Test getArticleByIdHandler
TEST_F(ArticleRoutesTest, GetArticleByIdHandler) {
    // Create parameters map with articleId
    std::unordered_map<std::string, std::string> params;
    params[":id"] = "1";
    
    // Create a mock request with params
    auto req = createMockRequest("", params);
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = getArticleByIdHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
}

// Test getArticlesByCategoryHandler
TEST_F(ArticleRoutesTest, GetArticlesByCategoryHandler) {
    // Create parameters map with category
    std::unordered_map<std::string, std::string> params;
    params[":category"] = "technology";
    
    // Create a mock request with params
    auto req = createMockRequest("", params);
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = getArticlesByCategoryHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
}

// Test saveArticleHandler
TEST_F(ArticleRoutesTest, SaveArticleHandler) {
    // Create request body
    json body = {
        {"user_id", 42},
        {"article_id", 1}
    };
    
    // Create a mock request with body
    auto req = createMockRequest(body.dump());
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = saveArticleHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
    
    // Verify the service method was called correctly
    EXPECT_EQ(MockArticleService::lastSavedArticle.first, 42);
    EXPECT_EQ(MockArticleService::lastSavedArticle.second, 1);
}

// Test deleteSavedArticleHandler
TEST_F(ArticleRoutesTest, DeleteSavedArticleHandler) {
    // Create request body
    json body = {
        {"userId", 42},
        {"articleId", 1}
    };
    
    // Create a mock request with body
    auto req = createMockRequest(body.dump());
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = deleteSavedArticleHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
    
    // Verify the service method was called correctly
    EXPECT_EQ(MockArticleService::lastDeletedArticle.first, 42);
    EXPECT_EQ(MockArticleService::lastDeletedArticle.second, 1);
}

// Test getSavedArticlesHandler
TEST_F(ArticleRoutesTest, GetSavedArticlesHandler) {
    // Create parameters map with userId
    std::unordered_map<std::string, std::string> params;
    params[":userId"] = "42";
    
    // Create a mock request with params
    auto req = createMockRequest("", params);
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = getSavedArticlesHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
}

// Test likeArticleHandler
TEST_F(ArticleRoutesTest, LikeArticleHandler) {
    // Create parameters map with articleId
    std::unordered_map<std::string, std::string> params;
    params[":id"] = "1";
    
    // Create request body
    json body = {{"user_id", 42}};
    
    // Create a mock request with body and params
    auto req = createMockRequest(body.dump(), params);
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = likeArticleHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
    
    // Verify the service method was called correctly
    EXPECT_EQ(MockArticleService::lastLikedArticle.first, 42);
    EXPECT_EQ(MockArticleService::lastLikedArticle.second, 1);
}

// Test dislikeArticleHandler
TEST_F(ArticleRoutesTest, DislikeArticleHandler) {
    // Create parameters map with articleId
    std::unordered_map<std::string, std::string> params;
    params[":id"] = "1";
    
    // Create request body
    json body = {{"user_id", 42}};
    
    // Create a mock request with body and params
    auto req = createMockRequest(body.dump(), params);
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = dislikeArticleHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
    
    // Verify the service method was called correctly
    EXPECT_EQ(MockArticleService::lastDislikedArticle.first, 42);
    EXPECT_EQ(MockArticleService::lastDislikedArticle.second, 1);
}

// Test searchArticlesHandler
TEST_F(ArticleRoutesTest, SearchArticlesHandler) {
    // Create query parameters
    std::unordered_map<std::string, std::string> query;
    query["keyword"] = "Test";
    query["startDate"] = "2023-07-01";
    query["endDate"] = "2023-07-31";
    query["sort"] = "likes";
    
    // Create a mock request with query parameters
    auto req = createMockRequest("", {}, query);
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = searchArticlesHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
    
    // Verify the service method was called correctly
    EXPECT_EQ(std::get<0>(MockArticleService::lastSearchParams), "Test");
    EXPECT_EQ(std::get<1>(MockArticleService::lastSearchParams), "2023-07-01");
    EXPECT_EQ(std::get<2>(MockArticleService::lastSearchParams), "2023-07-31");
    EXPECT_EQ(std::get<3>(MockArticleService::lastSearchParams), "likes");
}

// Test getReactionStatsHandler
TEST_F(ArticleRoutesTest, GetReactionStatsHandler) {
    // Create parameters map with articleId
    std::unordered_map<std::string, std::string> params;
    params[":id"] = "1";
    
    // Create a mock request with params
    auto req = createMockRequest("", params);
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = getReactionStatsHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
}

// Test error handling
TEST_F(ArticleRoutesTest, ErrorHandling) {
    // Configure the mock to throw an exception
    MockArticleService::shouldThrow = true;
    
    // Create a mock request and response
    auto req = createMockRequest();
    auto resp = createResponseWriter();
    
    // Call the route handler directly - should handle the exception gracefully
    EXPECT_NO_THROW({
        auto result = getAllArticlesHandler(req, std::move(resp));
        EXPECT_EQ(result, Rest::Route::Result::Ok);
    });
    
    // Reset the mock
    MockArticleService::shouldThrow = false;
}

// Test route setup
TEST_F(ArticleRoutesTest, RouteSetup) {
    // Create a router
    Rest::Router router;
    
    // This is a structural test to ensure setup doesn't throw
    EXPECT_NO_THROW(ArticleRoutes::setup(router));
}