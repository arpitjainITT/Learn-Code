#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../database/DataBase.hpp"
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

// Test fixture for Database class
class DatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any test-specific initialization
        // For a real test, we'd initialize a test database
        // Here we'll use mocks and test the interface
    }

    void TearDown() override {
        // Clean up any test-specific resources
    }

    // Helper to create a sample article
    json createSampleArticle() {
        return json{
            {"title", "Test Database Article"},
            {"description", "This is a test article for database operations"},
            {"content", "Full content of the test article..."},
            {"url", "https://example.com/test-article"},
            {"image_url", "https://example.com/test-article/image.jpg"},
            {"source", "Test News Source"},
            {"category", "technology"},
            {"language", "en"},
            {"locale", "us"}
        };
    }

    // Helper to create a sample user
    json createSampleUser() {
        return json{
            {"username", "testuser"},
            {"email", "testuser@example.com"},
            {"password", "hashedpassword123"}  // In real tests, we'd use a proper hash
        };
    }

    // Helper to create sample user preferences
    json createSamplePreferences() {
        return json{
            {"categories", {"technology", "business"}},
            {"keywords", {"ai", "blockchain"}}
        };
    }
};

// Test storing and retrieving an article
TEST_F(DatabaseTest, StoreAndRetrieveArticle) {
    // Create a sample article
    auto sampleArticle = createSampleArticle();
    
    // Store the article in the database
    bool storeResult = Database::storeArticle(sampleArticle);
    
    // Check that storage was successful
    EXPECT_TRUE(storeResult);
    
    // Retrieve all articles
    auto allArticles = Database::getAllArticles();
    
    // Check that at least one article was retrieved
    EXPECT_GT(allArticles.size(), 0);
    
    // Find our test article in the results
    bool foundArticle = false;
    for (const auto& article : allArticles) {
        if (article["title"] == "Test Database Article") {
            foundArticle = true;
            // Verify the article properties
            EXPECT_EQ(article["description"], "This is a test article for database operations");
            EXPECT_EQ(article["source"], "Test News Source");
            break;
        }
    }
    
    // Check that our test article was found
    EXPECT_TRUE(foundArticle);
}

// Test user authentication
TEST_F(DatabaseTest, UserAuthentication) {
    // Register a test user
    bool registerResult = Database::registerUser("testuser", "testuser@example.com", "password123");
    
    // Check that registration was successful
    EXPECT_TRUE(registerResult);
    
    // Authenticate the user
    int userId = 0;
    std::string role;
    bool authResult = Database::authenticateUser("testuser@example.com", "password123", userId, role);
    
    // Check that authentication was successful
    EXPECT_TRUE(authResult);
    EXPECT_GT(userId, 0);  // User ID should be positive
    EXPECT_FALSE(role.empty());  // Role should not be empty
    
    // Try to authenticate with wrong password
    int invalidUserId = 0;
    std::string invalidRole;
    bool invalidAuthResult = Database::authenticateUser("testuser@example.com", "wrongpassword", invalidUserId, invalidRole);
    
    // Check that authentication failed
    EXPECT_FALSE(invalidAuthResult);
}

// Test getting user ID by email
TEST_F(DatabaseTest, GetUserIdByEmail) {
    // Register a test user
    bool registerResult = Database::registerUser("emailuser", "emailuser@example.com", "password123");
    
    // Check that registration was successful
    EXPECT_TRUE(registerResult);
    
    // Get the user ID by email
    int userId = Database::getUserIdByEmail("emailuser@example.com");
    
    // Check that user ID was found
    EXPECT_GT(userId, 0);
    
    // Try to get user ID for non-existent email
    int nonExistentUserId = Database::getUserIdByEmail("nonexistent@example.com");
    
    // Check that no user ID was found
    EXPECT_EQ(nonExistentUserId, -1);  // Assuming -1 indicates not found
}

// Test saving and retrieving user preferences
TEST_F(DatabaseTest, UserPreferences) {
    // Register a test user
    bool registerResult = Database::registerUser("prefsuser", "prefsuser@example.com", "password123");
    
    // Check that registration was successful
    EXPECT_TRUE(registerResult);
    
    // Get the user ID
    int userId = Database::getUserIdByEmail("prefsuser@example.com");
    
    // Save user preferences
    auto preferences = createSamplePreferences();
    bool saveResult = Database::saveUserPreferences(userId, preferences);
    
    // Check that preferences were saved successfully
    EXPECT_TRUE(saveResult);
    
    // Retrieve user preferences
    auto retrievedPreferences = Database::getUserPreferences(userId);
    
    // Check that preferences were retrieved correctly
    EXPECT_TRUE(retrievedPreferences.contains("categories"));
    EXPECT_TRUE(retrievedPreferences.contains("keywords"));
    EXPECT_EQ(retrievedPreferences["categories"][0], "technology");
    EXPECT_EQ(retrievedPreferences["categories"][1], "business");
    EXPECT_EQ(retrievedPreferences["keywords"][0], "ai");
    EXPECT_EQ(retrievedPreferences["keywords"][1], "blockchain");
}

// Test getting articles by category
TEST_F(DatabaseTest, GetArticlesByCategory) {
    // Create and store a sample article
    auto sampleArticle = createSampleArticle();
    bool storeResult = Database::storeArticle(sampleArticle);
    EXPECT_TRUE(storeResult);
    
    // Get articles by category
    auto techArticles = Database::getArticlesByCategory("technology");
    
    // Check that we got some articles
    EXPECT_GT(techArticles.size(), 0);
    
    // Check that all articles have the correct category
    for (const auto& article : techArticles) {
        EXPECT_EQ(article["category"], "technology");
    }
    
    // Get articles for a non-existent category
    auto nonExistentCategoryArticles = Database::getArticlesByCategory("nonexistentcategory");
    
    // Check that no articles were found
    EXPECT_EQ(nonExistentCategoryArticles.size(), 0);
}

// Test getting articles by keyword
TEST_F(DatabaseTest, GetArticlesByKeyword) {
    // Create and store a sample article with a specific keyword in the title
    auto sampleArticle = createSampleArticle();
    sampleArticle["title"] = "Test Article with Blockchain Technology";
    bool storeResult = Database::storeArticle(sampleArticle);
    EXPECT_TRUE(storeResult);
    
    // Get articles by keyword
    auto blockchainArticles = Database::getArticlesByKeyword("blockchain");
    
    // Check that we got some articles
    EXPECT_GT(blockchainArticles.size(), 0);
    
    // Check that all articles contain the keyword
    for (const auto& article : blockchainArticles) {
        std::string title = article["title"];
        std::string description = article["description"];
        std::string content = article["content"];
        
        bool containsKeyword = 
            (title.find("blockchain") != std::string::npos) || 
            (title.find("Blockchain") != std::string::npos) ||
            (description.find("blockchain") != std::string::npos) || 
            (description.find("Blockchain") != std::string::npos) ||
            (content.find("blockchain") != std::string::npos) || 
            (content.find("Blockchain") != std::string::npos);
            
        EXPECT_TRUE(containsKeyword);
    }
    
    // Get articles for a non-existent keyword
    auto nonExistentKeywordArticles = Database::getArticlesByKeyword("nonexistentkeyword");
    
    // Check that no articles were found
    EXPECT_EQ(nonExistentKeywordArticles.size(), 0);
}

// Test notification system
TEST_F(DatabaseTest, NotificationSystem) {
    // Register a test user
    bool registerResult = Database::registerUser("notifyuser", "notifyuser@example.com", "password123");
    EXPECT_TRUE(registerResult);
    
    // Get the user ID
    int userId = Database::getUserIdByEmail("notifyuser@example.com");
    
    // Create and store a test article
    auto sampleArticle = createSampleArticle();
    bool storeResult = Database::storeArticle(sampleArticle);
    EXPECT_TRUE(storeResult);
    
    // Get all articles to find the ID of our test article
    auto allArticles = Database::getAllArticles();
    int articleId = -1;
    for (const auto& article : allArticles) {
        if (article["title"] == "Test Database Article") {
            articleId = article["id"];
            break;
        }
    }
    EXPECT_GT(articleId, 0);
    
    // Create a notification for the user
    bool notifyResult = Database::notifyUsers(articleId, "New article available: Test Database Article");
    EXPECT_TRUE(notifyResult);
    
    // Get notifications for the user
    auto notifications = Database::getNotificationsForUser(userId);
    
    // Check that we got at least one notification
    EXPECT_GT(notifications.size(), 0);
    
    // Find our test notification
    bool foundNotification = false;
    int notificationId = -1;
    for (const auto& notification : notifications) {
        if (notification["article_id"] == articleId) {
            foundNotification = true;
            notificationId = notification["id"];
            // Verify notification properties
            EXPECT_EQ(notification["user_id"], userId);
            EXPECT_FALSE(notification["read"]);
            break;
        }
    }
    EXPECT_TRUE(foundNotification);
    EXPECT_GT(notificationId, 0);
    
    // Mark the notification as read
    bool markReadResult = Database::markNotificationAsRead(notificationId);
    EXPECT_TRUE(markReadResult);
    
    // Get notifications again to verify it's marked as read
    notifications = Database::getNotificationsForUser(userId);
    for (const auto& notification : notifications) {
        if (notification["id"] == notificationId) {
            EXPECT_TRUE(notification["read"]);
            break;
        }
    }
    
    // Delete the notification
    bool deleteResult = Database::deleteNotification(notificationId);
    EXPECT_TRUE(deleteResult);
    
    // Get notifications again to verify it's deleted
    notifications = Database::getNotificationsForUser(userId);
    bool notificationDeleted = true;
    for (const auto& notification : notifications) {
        if (notification["id"] == notificationId) {
            notificationDeleted = false;
            break;
        }
    }
    EXPECT_TRUE(notificationDeleted);
}

// Test external server management
TEST_F(DatabaseTest, ExternalServerManagement) {
    // Get all external servers
    auto servers = Database::getAllExternalServers();
    
    // There should be at least one server configured
    EXPECT_GT(servers.size(), 0);
    
    // Get the ID of the first server
    int serverId = servers[0]["id"];
    
    // Get server details by ID
    auto serverDetails = Database::getExternalServerById(serverId);
    
    // Verify server details
    EXPECT_EQ(serverDetails["id"], serverId);
    EXPECT_FALSE(serverDetails["server_name"].empty());
    EXPECT_FALSE(serverDetails["api_url"].empty());
    
    // Update server API key
    std::string newApiKey = "new-test-api-key-" + std::to_string(rand());
    bool updateApiKeyResult = Database::updateExternalServerApiKey(serverId, newApiKey);
    EXPECT_TRUE(updateApiKeyResult);
    
    // Get server details again to verify API key was updated
    serverDetails = Database::getExternalServerById(serverId);
    EXPECT_EQ(serverDetails["api_key"], newApiKey);
    
    // Update server status
    bool updateStatusResult = Database::updateExternalServerStatus(serverId, "maintenance");
    EXPECT_TRUE(updateStatusResult);
    
    // Get server details again to verify status was updated
    serverDetails = Database::getExternalServerById(serverId);
    EXPECT_EQ(serverDetails["status"], "maintenance");
    
    // Update server status by name
    std::string serverName = serverDetails["server_name"];
    bool updateStatusByNameResult = Database::updateExternalServerStatusByName(serverName, "active");
    EXPECT_TRUE(updateStatusByNameResult);
    
    // Get server details again to verify status was updated
    serverDetails = Database::getExternalServerById(serverId);
    EXPECT_EQ(serverDetails["status"], "active");
}

// Test category management
TEST_F(DatabaseTest, CategoryManagement) {
    // Add a new category
    std::string newCategory = "test-category-" + std::to_string(rand());
    bool addCategoryResult = Database::addCategory(newCategory);
    EXPECT_TRUE(addCategoryResult);
    
    // Get articles by the new category - should be empty at first
    auto articlesInNewCategory = Database::getArticlesByCategory(newCategory);
    EXPECT_EQ(articlesInNewCategory.size(), 0);
    
    // Create and store an article in the new category
    auto sampleArticle = createSampleArticle();
    sampleArticle["category"] = newCategory;
    bool storeResult = Database::storeArticle(sampleArticle);
    EXPECT_TRUE(storeResult);
    
    // Get articles by the new category again - should have our article now
    articlesInNewCategory = Database::getArticlesByCategory(newCategory);
    EXPECT_GT(articlesInNewCategory.size(), 0);
    
    // Verify the article is in the correct category
    for (const auto& article : articlesInNewCategory) {
        EXPECT_EQ(article["category"], newCategory);
    }
}