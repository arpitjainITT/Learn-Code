#include <gtest/gtest.h>
#include "../../services/SavedArticleService.h"
#include "../../models/Article.h"
#include <string>
#include <strings.h>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

class SavedArticleServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
    
    void TearDown() override {
        // Cleanup code if needed
    }
};

TEST_F(SavedArticleServiceTest, GetSavedArticlesValidUserId) {
    // Test getSavedArticles with valid user IDs
    std::vector<int> validUserIds = {
        1,
        123,
        999,
        1000,
        10000
    };
    
    for (int userId : validUserIds) {
        EXPECT_NO_THROW({
            std::vector<Article> articles = SavedArticleService::getSavedArticles(userId);
            // Result depends on actual HTTP response
        });
    }
}

TEST_F(SavedArticleServiceTest, GetSavedArticlesZeroUserId) {
    // Test getSavedArticles with zero user ID
    EXPECT_NO_THROW({
        std::vector<Article> articles = SavedArticleService::getSavedArticles(0);
    });
}

TEST_F(SavedArticleServiceTest, GetSavedArticlesNegativeUserId) {
    // Test getSavedArticles with negative user ID
    EXPECT_NO_THROW({
        std::vector<Article> articles = SavedArticleService::getSavedArticles(-1);
        std::vector<Article> articles2 = SavedArticleService::getSavedArticles(-100);
    });
}

TEST_F(SavedArticleServiceTest, GetSavedArticlesLargeUserId) {
    // Test getSavedArticles with large user ID
    EXPECT_NO_THROW({
        std::vector<Article> articles = SavedArticleService::getSavedArticles(2147483647);
        std::vector<Article> articles2 = SavedArticleService::getSavedArticles(1000000);
    });
}

TEST_F(SavedArticleServiceTest, DeleteSavedArticleValidIds) {
    // Test deleteSavedArticle with valid IDs
    std::vector<std::pair<int, int>> testCases = {
        {1, 100},
        {123, 456},
        {999, 1000},
        {0, 0},
        {1000, 2000}
    };
    
    for (const auto& testCase : testCases) {
        int userId = testCase.first;
        int articleId = testCase.second;
        
        EXPECT_NO_THROW({
            SavedArticleService::deleteSavedArticle(userId, articleId);
        });
    }
}

TEST_F(SavedArticleServiceTest, DeleteSavedArticleNegativeIds) {
    // Test deleteSavedArticle with negative IDs
    EXPECT_NO_THROW({
        SavedArticleService::deleteSavedArticle(-1, -1);
        SavedArticleService::deleteSavedArticle(-100, -200);
        SavedArticleService::deleteSavedArticle(-1, 100);
        SavedArticleService::deleteSavedArticle(100, -1);
    });
}

TEST_F(SavedArticleServiceTest, DeleteSavedArticleLargeIds) {
    // Test deleteSavedArticle with large IDs
    EXPECT_NO_THROW({
        SavedArticleService::deleteSavedArticle(2147483647, 2147483647);
        SavedArticleService::deleteSavedArticle(1000000, 2000000);
    });
}

TEST_F(SavedArticleServiceTest, MethodSignatures) {
    // Test that method signatures are correct
    int userId = 1;
    int articleId = 100;
    
    // Test getSavedArticles method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        std::vector<Article> (*getSavedArticlesFunc)(int) = &SavedArticleService::getSavedArticles;
        (void)getSavedArticlesFunc; // Suppress unused variable warning
    });
    
    // Test deleteSavedArticle method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        void (*deleteSavedArticleFunc)(int, int) = &SavedArticleService::deleteSavedArticle;
        (void)deleteSavedArticleFunc; // Suppress unused variable warning
    });
}

TEST_F(SavedArticleServiceTest, StaticMethods) {
    // Test that methods are static
    int userId = 1;
    int articleId = 100;
    
    EXPECT_NO_THROW({
        // These should work without instantiating the class
        SavedArticleService::getSavedArticles(userId);
        SavedArticleService::deleteSavedArticle(userId, articleId);
    });
}

TEST_F(SavedArticleServiceTest, ExceptionSafety) {
    // Test exception safety
    int userId = 1;
    int articleId = 100;
    
    EXPECT_NO_THROW({
        // Should not throw unexpected exceptions
        SavedArticleService::getSavedArticles(userId);
        SavedArticleService::deleteSavedArticle(userId, articleId);
    });
}

TEST_F(SavedArticleServiceTest, JSONHandling) {
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

TEST_F(SavedArticleServiceTest, ArticleCreation) {
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

TEST_F(SavedArticleServiceTest, URLConstruction) {
    // Test URL construction logic
    int userId = 123;
    int articleId = 456;
    
    EXPECT_NO_THROW({
        // Test getSavedArticles URL construction
        std::string getUrl = "/saved-articles/" + std::to_string(userId);
        EXPECT_TRUE(getUrl.find("123") != std::string::npos);
        
        // Test deleteSavedArticle URL construction
        std::string deleteUrl = "/delete-saved-article?userId=" + std::to_string(userId) + "&articleId=" + std::to_string(articleId);
        EXPECT_TRUE(deleteUrl.find("userId=123") != std::string::npos);
        EXPECT_TRUE(deleteUrl.find("articleId=456") != std::string::npos);
    });
}

TEST_F(SavedArticleServiceTest, DefaultCategoryHandling) {
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

TEST_F(SavedArticleServiceTest, EmptyResponseHandling) {
    // Test handling of empty responses
    EXPECT_NO_THROW({
        // Test with empty JSON array
        std::string emptyResponse = "[]";
        nlohmann::json emptyArray = nlohmann::json::parse(emptyResponse);
        EXPECT_TRUE(emptyArray.is_array());
        EXPECT_EQ(emptyArray.size(), 0);
    });
}

TEST_F(SavedArticleServiceTest, InvalidJSONHandling) {
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

TEST_F(SavedArticleServiceTest, MultipleArticlesHandling) {
    // Test handling of multiple articles in response
    std::string multipleArticlesResponse = R"([
        {"id": 1, "title": "Article 1", "description": "Desc 1", "category": "tech"},
        {"id": 2, "title": "Article 2", "description": "Desc 2", "category": "sports"},
        {"id": 3, "title": "Article 3", "description": "Desc 3", "category": "politics"}
    ])";
    
    EXPECT_NO_THROW({
        nlohmann::json articlesArray = nlohmann::json::parse(multipleArticlesResponse);
        EXPECT_TRUE(articlesArray.is_array());
        EXPECT_EQ(articlesArray.size(), 3);
        
        for (const auto& item : articlesArray) {
            EXPECT_TRUE(item.contains("id"));
            EXPECT_TRUE(item.contains("title"));
            EXPECT_TRUE(item.contains("description"));
            EXPECT_TRUE(item.contains("category"));
        }
    });
}

TEST_F(SavedArticleServiceTest, MissingFieldsHandling) {
    // Test handling of articles with missing fields
    std::vector<nlohmann::json> incompleteArticles = {
        {{"id", 1}}, // Only id
        {{"id", 2}, {"title", "Test"}}, // Missing description
        {{"id", 3}, {"title", "Test"}, {"description", "Desc"}} // Missing category
    };
    
    for (const auto& articleData : incompleteArticles) {
        EXPECT_NO_THROW({
            Article article;
            article.id = articleData.value("id", 0);
            article.title = articleData.value("title", "");
            article.description = articleData.value("description", "");
            article.category = articleData.value("category", "General");
            article.source = articleData.value("source", "");
            article.url = articleData.value("url", "");
            article.createdAt = articleData.value("created_at", "");
            
            // Should not crash and should use default values
            EXPECT_GE(article.id, 0);
        });
    }
}

TEST_F(SavedArticleServiceTest, StringToIntConversion) {
    // Test string to int conversion for user ID
    std::vector<int> testIds = {
        0,
        1,
        123,
        999,
        1000,
        2147483647
    };
    
    for (int id : testIds) {
        EXPECT_NO_THROW({
            std::string idStr = std::to_string(id);
            int convertedId = std::stoi(idStr);
            EXPECT_EQ(convertedId, id);
        });
    }
}

TEST_F(SavedArticleServiceTest, QueryParameterConstruction) {
    // Test query parameter construction for delete endpoint
    std::vector<std::pair<int, int>> testCases = {
        {1, 100},
        {123, 456},
        {0, 0},
        {-1, -1}
    };
    
    for (const auto& testCase : testCases) {
        int userId = testCase.first;
        int articleId = testCase.second;
        
        EXPECT_NO_THROW({
            std::string queryParams = "?userId=" + std::to_string(userId) + "&articleId=" + std::to_string(articleId);
            EXPECT_TRUE(queryParams.find("userId=") != std::string::npos);
            EXPECT_TRUE(queryParams.find("articleId=") != std::string::npos);
            EXPECT_TRUE(queryParams.find(std::to_string(userId)) != std::string::npos);
            EXPECT_TRUE(queryParams.find(std::to_string(articleId)) != std::string::npos);
        });
    }
}

TEST_F(SavedArticleServiceTest, VectorOperations) {
    // Test vector operations for articles
    std::vector<Article> articles;
    
    EXPECT_NO_THROW({
        // Test adding articles to vector
        Article article1;
        article1.id = 1;
        article1.title = "Article 1";
        articles.push_back(article1);
        
        Article article2;
        article2.id = 2;
        article2.title = "Article 2";
        articles.push_back(article2);
        
        EXPECT_EQ(articles.size(), 2);
        EXPECT_EQ(articles[0].id, 1);
        EXPECT_EQ(articles[1].id, 2);
        
        // Test clearing vector
        articles.clear();
        EXPECT_EQ(articles.size(), 0);
    });
}

TEST_F(SavedArticleServiceTest, ReturnValueConsistency) {
    // Test that methods return consistent values
    int userId = 1;
    
    EXPECT_NO_THROW({
        // getSavedArticles should always return a vector (even if empty)
        std::vector<Article> articles = SavedArticleService::getSavedArticles(userId);
        // Should not crash and should return a valid vector
    });
}

TEST_F(SavedArticleServiceTest, ErrorHandling) {
    // Test error handling scenarios
    int userId = 1;
    int articleId = 100;
    
    EXPECT_NO_THROW({
        // These should handle errors gracefully without throwing
        SavedArticleService::getSavedArticles(userId);
        SavedArticleService::deleteSavedArticle(userId, articleId);
    });
}
