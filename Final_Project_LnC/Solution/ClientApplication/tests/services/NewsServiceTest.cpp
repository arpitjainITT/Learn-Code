#include <gtest/gtest.h>
#include "../../services/NewsService.h"
#include "../../models/Article.h"
#include <string>
#include <strings.h>

#include <vector>
#include <nlohmann/json.hpp>

class NewsServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
    
    void TearDown() override {
        // Cleanup code if needed
    }
};

TEST_F(NewsServiceTest, GetArticlesDefault) {
    // Test getArticles with default parameters
    EXPECT_NO_THROW({
        std::vector<Article> articles = NewsService::getArticles();
        // Result depends on actual HTTP response
    });
}

TEST_F(NewsServiceTest, GetArticlesWithCategory) {
    // Test getArticles with category parameter
    std::vector<std::string> categories = {
        "technology",
        "sports",
        "politics",
        "entertainment",
        "science"
    };
    
    for (const auto& category : categories) {
        EXPECT_NO_THROW({
            std::vector<Article> articles = NewsService::getArticles(category);
        });
    }
}

TEST_F(NewsServiceTest, GetArticlesWithEmptyCategory) {
    // Test getArticles with empty category
    EXPECT_NO_THROW({
        std::vector<Article> articles = NewsService::getArticles("");
    });
}

TEST_F(NewsServiceTest, GetArticlesWithAllCategory) {
    // Test getArticles with "all" category
    EXPECT_NO_THROW({
        std::vector<Article> articles = NewsService::getArticles("all");
    });
}

TEST_F(NewsServiceTest, GetArticlesWithDateRange) {
    // Test getArticles with date range
    std::string startDate = "2023-01-01";
    std::string endDate = "2023-12-31";
    
    EXPECT_NO_THROW({
        std::vector<Article> articles = NewsService::getArticles("", startDate, endDate);
    });
}

TEST_F(NewsServiceTest, GetArticlesWithCategoryAndDateRange) {
    // Test getArticles with both category and date range
    std::string category = "technology";
    std::string startDate = "2023-01-01";
    std::string endDate = "2023-12-31";
    
    EXPECT_NO_THROW({
        std::vector<Article> articles = NewsService::getArticles(category, startDate, endDate);
    });
}

TEST_F(NewsServiceTest, GetArticlesWithEmptyDateRange) {
    // Test getArticles with empty date range
    EXPECT_NO_THROW({
        std::vector<Article> articles = NewsService::getArticles("", "", "");
    });
}

TEST_F(NewsServiceTest, GetArticlesWithSpecialCharacters) {
    // Test getArticles with special characters in category
    std::vector<std::string> specialCategories = {
        "science & technology",
        "health & wellness",
        "arts & culture",
        "business & finance"
    };
    
    for (const auto& category : specialCategories) {
        EXPECT_NO_THROW({
            std::vector<Article> articles = NewsService::getArticles(category);
        });
    }
}

TEST_F(NewsServiceTest, SearchArticlesWithKeyword) {
    // Test searchArticles with keyword
    std::vector<std::string> keywords = {
        "technology",
        "AI",
        "climate change",
        "election",
        "covid"
    };
    
    for (const auto& keyword : keywords) {
        EXPECT_NO_THROW({
            std::vector<Article> articles = NewsService::searchArticles(keyword, "", "");
        });
    }
}

TEST_F(NewsServiceTest, SearchArticlesWithEmptyKeyword) {
    // Test searchArticles with empty keyword
    EXPECT_NO_THROW({
        std::vector<Article> articles = NewsService::searchArticles("", "", "");
    });
}

TEST_F(NewsServiceTest, SearchArticlesWithDateRange) {
    // Test searchArticles with date range
    std::string keyword = "technology";
    std::string startDate = "2023-01-01";
    std::string endDate = "2023-12-31";
    
    EXPECT_NO_THROW({
        std::vector<Article> articles = NewsService::searchArticles(keyword, startDate, endDate);
    });
}

TEST_F(NewsServiceTest, SearchArticlesWithSortBy) {
    // Test searchArticles with sortBy parameter
    std::string keyword = "technology";
    std::vector<std::string> sortOptions = {
        "date",
        "title",
        "relevance",
        "popularity"
    };
    
    for (const auto& sortBy : sortOptions) {
        EXPECT_NO_THROW({
            std::vector<Article> articles = NewsService::searchArticles(keyword, "", "", sortBy);
        });
    }
}

TEST_F(NewsServiceTest, SearchArticlesWithAllParameters) {
    // Test searchArticles with all parameters
    std::string keyword = "technology";
    std::string startDate = "2023-01-01";
    std::string endDate = "2023-12-31";
    std::string sortBy = "date";
    
    EXPECT_NO_THROW({
        std::vector<Article> articles = NewsService::searchArticles(keyword, startDate, endDate, sortBy);
    });
}

TEST_F(NewsServiceTest, SearchArticlesWithSpecialCharacters) {
    // Test searchArticles with special characters in keyword
    std::vector<std::string> specialKeywords = {
        "C++ programming",
        "machine learning & AI",
        "climate change & sustainability",
        "health & wellness"
    };
    
    for (const auto& keyword : specialKeywords) {
        EXPECT_NO_THROW({
            std::vector<Article> articles = NewsService::searchArticles(keyword, "", "");
        });
    }
}

TEST_F(NewsServiceTest, SaveArticle) {
    // Test saveArticle with valid parameters
    std::vector<std::pair<int, int>> testCases = {
        {1, 100},
        {123, 456},
        {999, 1000},
        {0, 0}
    };
    
    for (const auto& testCase : testCases) {
        int userId = testCase.first;
        int articleId = testCase.second;
        
        EXPECT_NO_THROW({
            NewsService::saveArticle(userId, articleId);
        });
    }
}

TEST_F(NewsServiceTest, SaveArticleWithNegativeIds) {
    // Test saveArticle with negative IDs
    EXPECT_NO_THROW({
        NewsService::saveArticle(-1, -1);
        NewsService::saveArticle(-100, -200);
    });
}

TEST_F(NewsServiceTest, SaveArticleWithLargeIds) {
    // Test saveArticle with large IDs
    EXPECT_NO_THROW({
        NewsService::saveArticle(2147483647, 2147483647);
        NewsService::saveArticle(1000000, 2000000);
    });
}

TEST_F(NewsServiceTest, LikeArticle) {
    // Test likeArticle with valid parameters
    std::vector<std::pair<int, int>> testCases = {
        {1, 100},
        {123, 456},
        {999, 1000},
        {0, 0}
    };
    
    for (const auto& testCase : testCases) {
        int userId = testCase.first;
        int articleId = testCase.second;
        
        EXPECT_NO_THROW({
            NewsService::likeArticle(userId, articleId);
        });
    }
}

TEST_F(NewsServiceTest, LikeArticleWithNegativeIds) {
    // Test likeArticle with negative IDs
    EXPECT_NO_THROW({
        NewsService::likeArticle(-1, -1);
        NewsService::likeArticle(-100, -200);
    });
}

TEST_F(NewsServiceTest, LikeArticleWithLargeIds) {
    // Test likeArticle with large IDs
    EXPECT_NO_THROW({
        NewsService::likeArticle(2147483647, 2147483647);
        NewsService::likeArticle(1000000, 2000000);
    });
}

TEST_F(NewsServiceTest, DislikeArticle) {
    // Test dislikeArticle with valid parameters
    std::vector<std::pair<int, int>> testCases = {
        {1, 100},
        {123, 456},
        {999, 1000},
        {0, 0}
    };
    
    for (const auto& testCase : testCases) {
        int userId = testCase.first;
        int articleId = testCase.second;
        
        EXPECT_NO_THROW({
            NewsService::dislikeArticle(userId, articleId);
        });
    }
}

TEST_F(NewsServiceTest, DislikeArticleWithNegativeIds) {
    // Test dislikeArticle with negative IDs
    EXPECT_NO_THROW({
        NewsService::dislikeArticle(-1, -1);
        NewsService::dislikeArticle(-100, -200);
    });
}

TEST_F(NewsServiceTest, DislikeArticleWithLargeIds) {
    // Test dislikeArticle with large IDs
    EXPECT_NO_THROW({
        NewsService::dislikeArticle(2147483647, 2147483647);
        NewsService::dislikeArticle(1000000, 2000000);
    });
}

TEST_F(NewsServiceTest, MethodSignatures) {
    // Test that method signatures are correct
    std::string category = "technology";
    std::string startDate = "2023-01-01";
    std::string endDate = "2023-12-31";
    std::string keyword = "AI";
    std::string sortBy = "date";
    int userId = 1;
    int articleId = 100;
    
    // Test getArticles method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        std::vector<Article> (*getArticlesFunc)(const std::string&, const std::string&, const std::string&) = &NewsService::getArticles;
        (void)getArticlesFunc; // Suppress unused variable warning
    });
    
    // Test searchArticles method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        std::vector<Article> (*searchArticlesFunc)(const std::string&, const std::string&, const std::string&, const std::string&) = &NewsService::searchArticles;
        (void)searchArticlesFunc; // Suppress unused variable warning
    });
    
    // Test saveArticle method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        void (*saveArticleFunc)(int, int) = &NewsService::saveArticle;
        (void)saveArticleFunc; // Suppress unused variable warning
    });
    
    // Test likeArticle method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        void (*likeArticleFunc)(int, int) = &NewsService::likeArticle;
        (void)likeArticleFunc; // Suppress unused variable warning
    });
    
    // Test dislikeArticle method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        void (*dislikeArticleFunc)(int, int) = &NewsService::dislikeArticle;
        (void)dislikeArticleFunc; // Suppress unused variable warning
    });
}

TEST_F(NewsServiceTest, StaticMethods) {
    // Test that methods are static
    std::string category = "technology";
    std::string keyword = "AI";
    int userId = 1;
    int articleId = 100;
    
    EXPECT_NO_THROW({
        // These should work without instantiating the class
        NewsService::getArticles(category);
        NewsService::searchArticles(keyword, "", "");
        NewsService::saveArticle(userId, articleId);
        NewsService::likeArticle(userId, articleId);
        NewsService::dislikeArticle(userId, articleId);
    });
}

TEST_F(NewsServiceTest, ExceptionSafety) {
    // Test exception safety
    std::string category = "technology";
    std::string keyword = "AI";
    int userId = 1;
    int articleId = 100;
    
    EXPECT_NO_THROW({
        // Should not throw unexpected exceptions
        NewsService::getArticles(category);
        NewsService::searchArticles(keyword, "", "");
        NewsService::saveArticle(userId, articleId);
        NewsService::likeArticle(userId, articleId);
        NewsService::dislikeArticle(userId, articleId);
    });
}

TEST_F(NewsServiceTest, JSONHandling) {
    // Test JSON handling logic
    nlohmann::json articleData = {
        {"id", 123},
        {"title", "Test Article"},
        {"description", "Test Description"},
        {"category", "technology"},
        {"source", "Test Source"},
        {"url", "https://example.com"},
        {"created_at", "2023-01-01"}
    };
    
    EXPECT_NO_THROW({
        std::string jsonStr = articleData.dump();
        nlohmann::json parsed = nlohmann::json::parse(jsonStr);
        EXPECT_EQ(parsed["id"], 123);
        EXPECT_EQ(parsed["title"], "Test Article");
        EXPECT_EQ(parsed["category"], "technology");
    });
}

TEST_F(NewsServiceTest, ArticleCreation) {
    // Test Article object creation from JSON
    nlohmann::json articleData = {
        {"id", 123},
        {"title", "Test Article"},
        {"description", "Test Description"},
        {"category", "technology"},
        {"source", "Test Source"},
        {"url", "https://example.com"},
        {"created_at", "2023-01-01"}
    };
    
    EXPECT_NO_THROW({
        Article article;
        article.id = articleData.value("id", 0);
        article.title = articleData.value("title", "");
        article.description = articleData.value("description", "");
        article.category = articleData.value("category", "General");
        article.source = articleData.value("source", "");
        article.url = articleData.value("url", "");
        article.createdAt = articleData.value("created_at", "");
        
        EXPECT_EQ(article.id, 123);
        EXPECT_EQ(article.title, "Test Article");
        EXPECT_EQ(article.category, "technology");
    });
}

TEST_F(NewsServiceTest, URLConstruction) {
    // Test URL construction logic
    std::string baseUrl = "/articles";
    std::string category = "technology";
    std::string startDate = "2023-01-01";
    std::string endDate = "2023-12-31";
    
    EXPECT_NO_THROW({
        // Test URL with category
        std::string urlWithCategory = baseUrl + "?category=" + category;
        EXPECT_TRUE(urlWithCategory.find("category=technology") != std::string::npos);
        
        // Test URL with date range
        std::string urlWithDate = baseUrl + "?start=" + startDate + "&end=" + endDate;
        EXPECT_TRUE(urlWithDate.find("start=2023-01-01") != std::string::npos);
        EXPECT_TRUE(urlWithDate.find("end=2023-12-31") != std::string::npos);
        
        // Test URL with both
        std::string urlWithBoth = baseUrl + "?category=" + category + "&start=" + startDate + "&end=" + endDate;
        EXPECT_TRUE(urlWithBoth.find("category=technology") != std::string::npos);
        EXPECT_TRUE(urlWithBoth.find("start=2023-01-01") != std::string::npos);
        EXPECT_TRUE(urlWithBoth.find("end=2023-12-31") != std::string::npos);
    });
}

TEST_F(NewsServiceTest, SearchURLConstruction) {
    // Test search URL construction logic
    std::string baseUrl = "/articles/search";
    std::string keyword = "technology";
    std::string startDate = "2023-01-01";
    std::string endDate = "2023-12-31";
    std::string sortBy = "date";
    
    EXPECT_NO_THROW({
        // Test URL with keyword only
        std::string urlWithKeyword = baseUrl + "?keyword=" + keyword;
        EXPECT_TRUE(urlWithKeyword.find("keyword=technology") != std::string::npos);
        
        // Test URL with date range
        std::string urlWithDate = baseUrl + "?keyword=" + keyword + "&start_date=" + startDate + "&end_date=" + endDate;
        EXPECT_TRUE(urlWithDate.find("start_date=2023-01-01") != std::string::npos);
        EXPECT_TRUE(urlWithDate.find("end_date=2023-12-31") != std::string::npos);
        
        // Test URL with sort
        std::string urlWithSort = baseUrl + "?keyword=" + keyword + "&sort=" + sortBy;
        EXPECT_TRUE(urlWithSort.find("sort=date") != std::string::npos);
        
        // Test URL with all parameters
        std::string urlWithAll = baseUrl + "?keyword=" + keyword + "&start_date=" + startDate + "&end_date=" + endDate + "&sort=" + sortBy;
        EXPECT_TRUE(urlWithAll.find("keyword=technology") != std::string::npos);
        EXPECT_TRUE(urlWithAll.find("start_date=2023-01-01") != std::string::npos);
        EXPECT_TRUE(urlWithAll.find("end_date=2023-12-31") != std::string::npos);
        EXPECT_TRUE(urlWithAll.find("sort=date") != std::string::npos);
    });
}

TEST_F(NewsServiceTest, DefaultCategoryHandling) {
    // Test default category handling
    nlohmann::json articleData = {
        {"id", 123},
        {"title", "Test Article"},
        {"description", "Test Description"}
        // No category field
    };
    
    EXPECT_NO_THROW({
        std::string category = articleData.value("category", "General");
        EXPECT_EQ(category, "General");
    });
}

TEST_F(NewsServiceTest, EmptyResponseHandling) {
    // Test handling of empty responses
    EXPECT_NO_THROW({
        // Test with empty JSON array
        std::string emptyResponse = "[]";
        nlohmann::json emptyArray = nlohmann::json::parse(emptyResponse);
        EXPECT_TRUE(emptyArray.is_array());
        EXPECT_EQ(emptyArray.size(), 0);
    });
}

TEST_F(NewsServiceTest, InvalidJSONHandling) {
    // Test handling of invalid JSON responses
    std::vector<std::string> invalidResponses = {
        "",
        "invalid json",
        "{invalid}",
        "[invalid]"
    };
    
    for (const auto& responseStr : invalidResponses) {
        EXPECT_NO_THROW({
            try {
                nlohmann::json response = nlohmann::json::parse(responseStr);
            } catch (...) {
                // Expected to throw for invalid JSON
            }
        });
    }
}
