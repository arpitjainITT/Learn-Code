#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../services/NotificationService.hpp"
#include "../../database/Database.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Mock for Database class to avoid real database operations
class MockDatabase {
public:
    MOCK_METHOD(bool, notifyUsers, (int, const std::string&));
    MOCK_METHOD(json, getUsersToNotify, (const json&));
    MOCK_METHOD(json, getNotificationsForUser, (int));
    MOCK_METHOD(bool, markNotificationAsRead, (int));
    MOCK_METHOD(bool, deleteNotification, (int));
};

// Test fixture for NotificationService
class NotificationServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Store original functions
        original_notifyUsers = Database::notifyUsers;
        original_getUsersToNotify = Database::getUsersToNotify;
        original_getNotificationsForUser = Database::getNotificationsForUser;
        original_markNotificationAsRead = Database::markNotificationAsRead;
        original_deleteNotification = Database::deleteNotification;
    }

    void TearDown() override {
        // Restore original functions
        Database::notifyUsers = original_notifyUsers;
        Database::getUsersToNotify = original_getUsersToNotify;
        Database::getNotificationsForUser = original_getNotificationsForUser;
        Database::markNotificationAsRead = original_markNotificationAsRead;
        Database::deleteNotification = original_deleteNotification;
    }

    // Store original function pointers
    std::function<bool(int, const std::string&)> original_notifyUsers;
    std::function<json(const json&)> original_getUsersToNotify;
    std::function<json(int)> original_getNotificationsForUser;
    std::function<bool(int)> original_markNotificationAsRead;
    std::function<bool(int)> original_deleteNotification;

    // Helper to create a sample article
    json createSampleArticle() {
        return json{
            {"id", 123},
            {"title", "Test Article"},
            {"description", "This is a test article description"},
            {"category", "technology"},
            {"source", "Test News Source"}
        };
    }

    // Helper to create sample users to notify
    json createSampleUsersToNotify() {
        json users = json::array();
        
        users.push_back({
            {"id", 1},
            {"username", "user1"},
            {"email", "user1@example.com"}
        });
        
        users.push_back({
            {"id", 2},
            {"username", "user2"},
            {"email", "user2@example.com"}
        });
        
        return users;
    }

    // Helper to create sample notifications
    json createSampleNotifications() {
        json notifications = json::array();
        
        notifications.push_back({
            {"id", 1},
            {"user_id", 42},
            {"article_id", 123},
            {"message", "New article: Test Article"},
            {"read", false},
            {"created_at", "2023-07-07T14:30:00Z"}
        });
        
        notifications.push_back({
            {"id", 2},
            {"user_id", 42},
            {"article_id", 124},
            {"message", "New article: Another Test Article"},
            {"read", true},
            {"created_at", "2023-07-06T10:15:00Z"}
        });
        
        return notifications;
    }
};

// Test notifying users about a new article
TEST_F(NotificationServiceTest, NotifyUsersAboutArticle) {
    // Override the original functions with our test functions
    Database::getUsersToNotify = [this](const json& article) {
        return createSampleUsersToNotify();
    };
    
    bool notifyUsersCalled = false;
    int notifiedArticleId = 0;
    std::string notificationMessage;
    
    Database::notifyUsers = [&notifyUsersCalled, &notifiedArticleId, &notificationMessage](int articleId, const std::string& message) {
        notifyUsersCalled = true;
        notifiedArticleId = articleId;
        notificationMessage = message;
        return true;
    };

    // Call method under test
    auto article = createSampleArticle();
    bool result = NotificationService::notifyUsersAboutArticle(article);

    // Verify results
    EXPECT_TRUE(result);
    EXPECT_TRUE(notifyUsersCalled);
    EXPECT_EQ(notifiedArticleId, 123);
    EXPECT_TRUE(notificationMessage.find("Test Article") != std::string::npos);
}

// Test getting notifications for a user
TEST_F(NotificationServiceTest, GetNotificationsForUser) {
    // Override the original function with our test function
    Database::getNotificationsForUser = [this](int userId) {
        if (userId == 42) {
            return createSampleNotifications();
        }
        return json::array();
    };

    // Call method under test
    auto notifications = NotificationService::getNotificationsForUser(42);
    auto emptyNotifications = NotificationService::getNotificationsForUser(999);

    // Verify results
    ASSERT_EQ(notifications.size(), 2);
    EXPECT_EQ(notifications[0]["id"], 1);
    EXPECT_EQ(notifications[0]["article_id"], 123);
    EXPECT_FALSE(notifications[0]["read"]);
    EXPECT_EQ(notifications[1]["id"], 2);
    EXPECT_TRUE(notifications[1]["read"]);
    EXPECT_TRUE(emptyNotifications.empty());
}

// Test marking a notification as read
TEST_F(NotificationServiceTest, MarkNotificationAsRead) {
    // Override the original function with our test function
    bool markAsReadCalled = false;
    int markedNotificationId = 0;
    
    Database::markNotificationAsRead = [&markAsReadCalled, &markedNotificationId](int notificationId) {
        markAsReadCalled = true;
        markedNotificationId = notificationId;
        return true;
    };

    // Call method under test
    bool result = NotificationService::markNotificationAsRead(42);

    // Verify results
    EXPECT_TRUE(result);
    EXPECT_TRUE(markAsReadCalled);
    EXPECT_EQ(markedNotificationId, 42);
}

// Test deleting a notification
TEST_F(NotificationServiceTest, DeleteNotification) {
    // Override the original function with our test function
    bool deleteNotificationCalled = false;
    int deletedNotificationId = 0;
    
    Database::deleteNotification = [&deleteNotificationCalled, &deletedNotificationId](int notificationId) {
        deleteNotificationCalled = true;
        deletedNotificationId = notificationId;
        return true;
    };

    // Call method under test
    bool result = NotificationService::deleteNotification(42);

    // Verify results
    EXPECT_TRUE(result);
    EXPECT_TRUE(deleteNotificationCalled);
    EXPECT_EQ(deletedNotificationId, 42);
}

// Test no users to notify
TEST_F(NotificationServiceTest, NoUsersToNotify) {
    // Override the original function with our test function
    Database::getUsersToNotify = [](const json& article) {
        return json::array(); // Empty array, no users to notify
    };
    
    bool notifyUsersCalled = false;
    Database::notifyUsers = [&notifyUsersCalled](int articleId, const std::string& message) {
        notifyUsersCalled = true;
        return true;
    };

    // Call method under test
    auto article = createSampleArticle();
    bool result = NotificationService::notifyUsersAboutArticle(article);

    // Verify results - should still return true but not call notifyUsers
    EXPECT_TRUE(result);
    EXPECT_FALSE(notifyUsersCalled);
}

// Test error when notifying users
TEST_F(NotificationServiceTest, ErrorNotifyingUsers) {
    // Override the original functions with our test functions
    Database::getUsersToNotify = [this](const json& article) {
        return createSampleUsersToNotify();
    };
    
    Database::notifyUsers = [](int articleId, const std::string& message) {
        return false; // Simulate database error
    };

    // Call method under test
    auto article = createSampleArticle();
    bool result = NotificationService::notifyUsersAboutArticle(article);

    // Verify results
    EXPECT_FALSE(result);
}

// Test error when marking notification as read
TEST_F(NotificationServiceTest, ErrorMarkingNotificationAsRead) {
    // Override the original function with our test function
    Database::markNotificationAsRead = [](int notificationId) {
        return false; // Simulate database error
    };

    // Call method under test
    bool result = NotificationService::markNotificationAsRead(42);

    // Verify results
    EXPECT_FALSE(result);
}

// Test error when deleting notification
TEST_F(NotificationServiceTest, ErrorDeletingNotification) {
    // Override the original function with our test function
    Database::deleteNotification = [](int notificationId) {
        return false; // Simulate database error
    };

    // Call method under test
    bool result = NotificationService::deleteNotification(42);

    // Verify results
    EXPECT_FALSE(result);
}

// Test with non-existent article
TEST_F(NotificationServiceTest, NonExistentArticle) {
    // Override the original function with our test function
    Database::getUsersToNotify = [](const json& article) {
        // If article ID doesn't exist or is invalid, return empty array
        if (!article.contains("id") || article["id"] <= 0) {
            return json::array();
        }
        return json::array({{"id", 1}, {"username", "user1"}});
    };

    // Call method under test with invalid article
    json invalidArticle = {{"title", "Invalid Article"}}; // No ID
    bool result = NotificationService::notifyUsersAboutArticle(invalidArticle);

    // Verify results
    EXPECT_TRUE(result); // Still returns true but doesn't notify anyone
}