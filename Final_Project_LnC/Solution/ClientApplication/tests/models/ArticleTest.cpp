#include <gtest/gtest.h>
#include "../../models/Article.h"
#include <string>
#include <strings.h>


class ArticleTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
};

TEST_F(ArticleTest, ParameterizedConstructor) {
    Article article(123, "Test Title", "Test Description", "Technology", 
                   "Test Source", "https://example.com", "2023-01-01");
    
    // Test all parameters are set correctly
    EXPECT_EQ(article.id, 123);
    EXPECT_EQ(article.title, "Test Title");
    EXPECT_EQ(article.description, "Test Description");
    EXPECT_EQ(article.category, "Technology");
    EXPECT_EQ(article.source, "Test Source");
    EXPECT_EQ(article.url, "https://example.com");
    EXPECT_EQ(article.createdAt, "2023-01-01");
}

TEST_F(ArticleTest, ParameterizedConstructorWithEmptyStrings) {
    Article article(0, "", "", "", "", "", "");
    
    // Test with empty strings
    EXPECT_EQ(article.id, 0);
    EXPECT_EQ(article.title, "");
    EXPECT_EQ(article.description, "");
    EXPECT_EQ(article.category, "");
    EXPECT_EQ(article.source, "");
    EXPECT_EQ(article.url, "");
    EXPECT_EQ(article.createdAt, "");
}

TEST_F(ArticleTest, MultipleArticlesIndependent) {
    Article article1(1, "First Article", "First Description", "News", 
                    "First Source", "https://first.com", "2023-01-01");
    Article article2(2, "Second Article", "Second Description", "Sports", 
                    "Second Source", "https://second.com", "2023-01-02");
    
    // Verify they are independent
    EXPECT_EQ(article1.id, 1);
    EXPECT_EQ(article1.title, "First Article");
    EXPECT_EQ(article1.category, "News");
    
    EXPECT_EQ(article2.id, 2);
    EXPECT_EQ(article2.title, "Second Article");
    EXPECT_EQ(article2.category, "Sports");
}

TEST_F(ArticleTest, DirectMemberAccess) {
    Article article;
    
    // Test direct member access and modification
    article.id = 999;
    article.title = "Modified Title";
    article.description = "Modified Description";
    article.category = "Modified Category";
    article.source = "Modified Source";
    article.url = "https://modified.com";
    article.createdAt = "2023-06-15";
    
    // Verify modifications
    EXPECT_EQ(article.id, 999);
    EXPECT_EQ(article.title, "Modified Title");
    EXPECT_EQ(article.description, "Modified Description");
    EXPECT_EQ(article.category, "Modified Category");
    EXPECT_EQ(article.source, "Modified Source");
    EXPECT_EQ(article.url, "https://modified.com");
    EXPECT_EQ(article.createdAt, "2023-06-15");
}

TEST_F(ArticleTest, EdgeCases) {
    Article article;
    
    // Test with very large ID
    article.id = 2147483647; // Max int
    EXPECT_EQ(article.id, 2147483647);
    
    // Test with negative ID
    article.id = -2147483648; // Min int
    EXPECT_EQ(article.id, -2147483648);
    
    // Test with very long strings
    std::string longString = "This is a very long string with many characters to test the limits of the string storage in the Article struct. It should handle long strings without any issues.";
    article.title = longString;
    article.description = longString;
    article.category = longString;
    article.source = longString;
    article.url = longString;
    article.createdAt = longString;
    
    EXPECT_EQ(article.title, longString);
    EXPECT_EQ(article.description, longString);
    EXPECT_EQ(article.category, longString);
    EXPECT_EQ(article.source, longString);
    EXPECT_EQ(article.url, longString);
    EXPECT_EQ(article.createdAt, longString);
}

TEST_F(ArticleTest, CopyConstructor) {
    Article original(123, "Original Title", "Original Description", "Original Category", 
                    "Original Source", "https://original.com", "2023-01-01");
    
    Article copy = original;
    
    // Verify copy has same values
    EXPECT_EQ(copy.id, original.id);
    EXPECT_EQ(copy.title, original.title);
    EXPECT_EQ(copy.description, original.description);
    EXPECT_EQ(copy.category, original.category);
    EXPECT_EQ(copy.source, original.source);
    EXPECT_EQ(copy.url, original.url);
    EXPECT_EQ(copy.createdAt, original.createdAt);
    
    // Verify they are independent
    copy.id = 456;
    copy.title = "Modified Title";
    EXPECT_NE(copy.id, original.id);
    EXPECT_NE(copy.title, original.title);
}

TEST_F(ArticleTest, AssignmentOperator) {
    Article original(123, "Original Title", "Original Description", "Original Category", 
                    "Original Source", "https://original.com", "2023-01-01");
    Article assigned;
    
    assigned = original;
    
    // Verify assignment
    EXPECT_EQ(assigned.id, original.id);
    EXPECT_EQ(assigned.title, original.title);
    EXPECT_EQ(assigned.description, original.description);
    EXPECT_EQ(assigned.category, original.category);
    EXPECT_EQ(assigned.source, original.source);
    EXPECT_EQ(assigned.url, original.url);
    EXPECT_EQ(assigned.createdAt, original.createdAt);
}
