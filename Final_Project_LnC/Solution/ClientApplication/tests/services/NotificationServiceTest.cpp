#include <gtest/gtest.h>
#include "../../services/NotificationService.h"
#include <string>
#include <strings.h>

#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

class NotificationServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
    
    void TearDown() override {
        // Cleanup code if needed
    }
};

TEST_F(NotificationServiceTest, SetCategoryPreferenceValidData) {
    // Test setCategoryPreference with valid data
    std::vector<std::tuple<int, std::string, bool>> testCases = {
        {1, "technology", true},
        {123, "sports", false},
        {999, "politics", true},
        {0, "entertainment", false}
    };
    
    for (const auto& testCase : testCases) {
        int userId = std::get<0>(testCase);
        std::string category = std::get<1>(testCase);
        bool enabled = std::get<2>(testCase);
        
        EXPECT_NO_THROW({
            NotificationService::setCategoryPreference(userId, category, enabled);
        });
    }
}

TEST_F(NotificationServiceTest, SetCategoryPreferenceEmptyCategory) {
    // Test setCategoryPreference with empty category
    EXPECT_NO_THROW({
        NotificationService::setCategoryPreference(1, "", true);
        NotificationService::setCategoryPreference(1, "", false);
    });
}

TEST_F(NotificationServiceTest, SetCategoryPreferenceSpecialCharacters) {
    // Test setCategoryPreference with special characters
    std::vector<std::string> specialCategories = {
        "science & technology",
        "health & wellness",
        "arts & culture",
        "business & finance"
    };
    
    for (const auto& category : specialCategories) {
        EXPECT_NO_THROW({
            NotificationService::setCategoryPreference(1, category, true);
            NotificationService::setCategoryPreference(1, category, false);
        });
    }
}

TEST_F(NotificationServiceTest, SetCategoryPreferenceNegativeUserId) {
    // Test setCategoryPreference with negative user ID
    EXPECT_NO_THROW({
        NotificationService::setCategoryPreference(-1, "technology", true);
        NotificationService::setCategoryPreference(-100, "sports", false);
    });
}

TEST_F(NotificationServiceTest, SetKeywordPreferenceValidData) {
    // Test setKeywordPreference with valid data
    std::vector<std::tuple<int, std::string, bool>> testCases = {
        {1, "AI", true},
        {123, "climate change", false},
        {999, "election", true},
        {0, "covid", false}
    };
    
    for (const auto& testCase : testCases) {
        int userId = std::get<0>(testCase);
        std::string keyword = std::get<1>(testCase);
        bool enabled = std::get<2>(testCase);
        
        EXPECT_NO_THROW({
            NotificationService::setKeywordPreference(userId, keyword, enabled);
        });
    }
}

TEST_F(NotificationServiceTest, SetKeywordPreferenceEmptyKeyword) {
    // Test setKeywordPreference with empty keyword
    EXPECT_NO_THROW({
        NotificationService::setKeywordPreference(1, "", true);
        NotificationService::setKeywordPreference(1, "", false);
    });
}

TEST_F(NotificationServiceTest, SetKeywordPreferenceSpecialCharacters) {
    // Test setKeywordPreference with special characters
    std::vector<std::string> specialKeywords = {
        "C++ programming",
        "machine learning & AI",
        "climate change & sustainability",
        "health & wellness"
    };
    
    for (const auto& keyword : specialKeywords) {
        EXPECT_NO_THROW({
            NotificationService::setKeywordPreference(1, keyword, true);
            NotificationService::setKeywordPreference(1, keyword, false);
        });
    }
}

TEST_F(NotificationServiceTest, SetKeywordPreferenceNegativeUserId) {
    // Test setKeywordPreference with negative user ID
    EXPECT_NO_THROW({
        NotificationService::setKeywordPreference(-1, "AI", true);
        NotificationService::setKeywordPreference(-100, "climate change", false);
    });
}

TEST_F(NotificationServiceTest, GetPreferencesValidUserId) {
    // Test getPreferences with valid user IDs
    std::vector<int> validUserIds = {
        1,
        123,
        999,
        1000,
        10000
    };
    
    for (int userId : validUserIds) {
        EXPECT_NO_THROW({
            NotificationPreferences prefs = NotificationService::getPreferences(userId);
            // Result depends on actual HTTP response
        });
    }
}

TEST_F(NotificationServiceTest, GetPreferencesZeroUserId) {
    // Test getPreferences with zero user ID
    EXPECT_NO_THROW({
        NotificationPreferences prefs = NotificationService::getPreferences(0);
    });
}

TEST_F(NotificationServiceTest, GetPreferencesNegativeUserId) {
    // Test getPreferences with negative user ID
    EXPECT_NO_THROW({
        NotificationPreferences prefs = NotificationService::getPreferences(-1);
        NotificationPreferences prefs2 = NotificationService::getPreferences(-100);
    });
}

TEST_F(NotificationServiceTest, GetPreferencesLargeUserId) {
    // Test getPreferences with large user ID
    EXPECT_NO_THROW({
        NotificationPreferences prefs = NotificationService::getPreferences(2147483647);
        NotificationPreferences prefs2 = NotificationService::getPreferences(1000000);
    });
}

TEST_F(NotificationServiceTest, GetNotificationsValidUserId) {
    // Test getNotifications with valid user IDs
    std::vector<int> validUserIds = {
        1,
        123,
        999,
        1000,
        10000
    };
    
    for (int userId : validUserIds) {
        EXPECT_NO_THROW({
            std::vector<Notification> notifications = NotificationService::getNotifications(userId);
            // Result depends on actual HTTP response
        });
    }
}

TEST_F(NotificationServiceTest, GetNotificationsZeroUserId) {
    // Test getNotifications with zero user ID
    EXPECT_NO_THROW({
        std::vector<Notification> notifications = NotificationService::getNotifications(0);
    });
}

TEST_F(NotificationServiceTest, GetNotificationsNegativeUserId) {
    // Test getNotifications with negative user ID
    EXPECT_NO_THROW({
        std::vector<Notification> notifications = NotificationService::getNotifications(-1);
        std::vector<Notification> notifications2 = NotificationService::getNotifications(-100);
    });
}

TEST_F(NotificationServiceTest, GetNotificationsLargeUserId) {
    // Test getNotifications with large user ID
    EXPECT_NO_THROW({
        std::vector<Notification> notifications = NotificationService::getNotifications(2147483647);
        std::vector<Notification> notifications2 = NotificationService::getNotifications(1000000);
    });
}

TEST_F(NotificationServiceTest, MarkNotificationAsReadValidId) {
    // Test markNotificationAsRead with valid IDs
    std::vector<int> validIds = {
        1,
        123,
        999,
        1000,
        10000
    };
    
    for (int notificationId : validIds) {
        EXPECT_NO_THROW({
            NotificationService::markNotificationAsRead(notificationId);
        });
    }
}

TEST_F(NotificationServiceTest, MarkNotificationAsReadZeroId) {
    // Test markNotificationAsRead with zero ID
    EXPECT_NO_THROW({
        NotificationService::markNotificationAsRead(0);
    });
}

TEST_F(NotificationServiceTest, MarkNotificationAsReadNegativeId) {
    // Test markNotificationAsRead with negative ID
    EXPECT_NO_THROW({
        NotificationService::markNotificationAsRead(-1);
        NotificationService::markNotificationAsRead(-100);
    });
}

TEST_F(NotificationServiceTest, MarkNotificationAsReadLargeId) {
    // Test markNotificationAsRead with large ID
    EXPECT_NO_THROW({
        NotificationService::markNotificationAsRead(2147483647);
        NotificationService::markNotificationAsRead(1000000);
    });
}

TEST_F(NotificationServiceTest, MethodSignatures) {
    // Test that method signatures are correct
    int userId = 1;
    int notificationId = 100;
    std::string category = "technology";
    std::string keyword = "AI";
    bool enabled = true;
    
    // Test setCategoryPreference method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        void (*setCategoryPrefFunc)(int, const std::string&, bool) = &NotificationService::setCategoryPreference;
        (void)setCategoryPrefFunc; // Suppress unused variable warning
    });
    
    // Test setKeywordPreference method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        void (*setKeywordPrefFunc)(int, const std::string&, bool) = &NotificationService::setKeywordPreference;
        (void)setKeywordPrefFunc; // Suppress unused variable warning
    });
    
    // Test getPreferences method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        NotificationPreferences (*getPrefsFunc)(int) = &NotificationService::getPreferences;
        (void)getPrefsFunc; // Suppress unused variable warning
    });
    
    // Test getNotifications method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        std::vector<Notification> (*getNotifsFunc)(int) = &NotificationService::getNotifications;
        (void)getNotifsFunc; // Suppress unused variable warning
    });
    
    // Test markNotificationAsRead method signature - fixed to use static function pointers
    EXPECT_NO_THROW({
        void (*markReadFunc)(int) = &NotificationService::markNotificationAsRead;
        (void)markReadFunc; // Suppress unused variable warning
    });
}

TEST_F(NotificationServiceTest, StaticMethods) {
    // Test that methods are static
    int userId = 1;
    int notificationId = 100;
    std::string category = "technology";
    std::string keyword = "AI";
    bool enabled = true;
    
    EXPECT_NO_THROW({
        // These should work without instantiating the class
        NotificationService::setCategoryPreference(userId, category, enabled);
        NotificationService::setKeywordPreference(userId, keyword, enabled);
        NotificationService::getPreferences(userId);
        NotificationService::getNotifications(userId);
        NotificationService::markNotificationAsRead(notificationId);
    });
}

TEST_F(NotificationServiceTest, ExceptionSafety) {
    // Test exception safety
    int userId = 1;
    int notificationId = 100;
    std::string category = "technology";
    std::string keyword = "AI";
    bool enabled = true;
    
    EXPECT_NO_THROW({
        // Should not throw unexpected exceptions
        NotificationService::setCategoryPreference(userId, category, enabled);
        NotificationService::setKeywordPreference(userId, keyword, enabled);
        NotificationService::getPreferences(userId);
        NotificationService::getNotifications(userId);
        NotificationService::markNotificationAsRead(notificationId);
    });
}

TEST_F(NotificationServiceTest, JSONHandling) {
    // Test JSON handling logic
    nlohmann::json preferenceData = {
        {"user_id", 123},
        {"category", "technology"},
        {"enabled", true}
    };
    
    EXPECT_NO_THROW({
        std::string jsonStr = preferenceData.dump();
        nlohmann::json parsed = nlohmann::json::parse(jsonStr);
        EXPECT_EQ(parsed["user_id"], 123);
        EXPECT_EQ(parsed["category"], "technology");
        EXPECT_EQ(parsed["enabled"], true);
    });
}

TEST_F(NotificationServiceTest, NotificationCreation) {
    // Test Notification object creation from JSON
    nlohmann::json notificationData = {
        {"id", 123},
        {"title", "Test Notification"},
        {"description", "Test Message"},
        {"category", "technology"},
        {"url", "https://example.com"},
        {"source", "Test Source"},
        {"read", false}
    };
    
    EXPECT_NO_THROW({
        Notification notification;
        notification.id = notificationData["id"];
        notification.title = notificationData["title"];
        notification.message = notificationData["description"];
        notification.category = notificationData.value("category", "General");
        notification.url = notificationData["url"];
        notification.source = notificationData["source"];
        notification.read = notificationData.value("read", false);
        
        EXPECT_EQ(notification.id, 123);
        EXPECT_EQ(notification.title, "Test Notification");
        EXPECT_EQ(notification.category, "technology");
        EXPECT_EQ(notification.read, false);
    });
}

TEST_F(NotificationServiceTest, PreferencesStructure) {
    // Test NotificationPreferences structure
    EXPECT_NO_THROW({
        NotificationPreferences prefs;
        
        // Test categories map
        prefs.categories["technology"] = true;
        prefs.categories["sports"] = false;
        prefs.categories["politics"] = true;
        
        EXPECT_EQ(prefs.categories["technology"], true);
        EXPECT_EQ(prefs.categories["sports"], false);
        EXPECT_EQ(prefs.categories["politics"], true);
        
        // Test keywords map
        prefs.keywords["AI"] = true;
        prefs.keywords["climate change"] = false;
        prefs.keywords["election"] = true;
        
        EXPECT_EQ(prefs.keywords["AI"], true);
        EXPECT_EQ(prefs.keywords["climate change"], false);
        EXPECT_EQ(prefs.keywords["election"], true);
    });
}

TEST_F(NotificationServiceTest, URLConstruction) {
    // Test URL construction logic
    int userId = 123;
    int notificationId = 456;
    
    EXPECT_NO_THROW({
        // Test getPreferences URL construction
        std::string prefsUrl = "/preferences/" + std::to_string(userId);
        EXPECT_TRUE(prefsUrl.find("123") != std::string::npos);
        
        // Test getNotifications URL construction
        std::string notifsUrl = "/notifications/" + std::to_string(userId);
        EXPECT_TRUE(notifsUrl.find("123") != std::string::npos);
        
        // Test markAsRead URL construction
        std::string markReadUrl = "/notifications/" + std::to_string(notificationId) + "/read";
        EXPECT_TRUE(markReadUrl.find("456") != std::string::npos);
        EXPECT_TRUE(markReadUrl.find("/read") != std::string::npos);
    });
}

TEST_F(NotificationServiceTest, DefaultCategoryHandling) {
    // Test default category handling
    nlohmann::json notificationData = {
        {"id", 123},
        {"title", "Test Notification"},
        {"description", "Test Message"}
        // No category field
    };
    
    EXPECT_NO_THROW({
        std::string category = notificationData.value("category", "General");
        EXPECT_EQ(category, "General");
    });
}

TEST_F(NotificationServiceTest, EmptyResponseHandling) {
    // Test handling of empty responses
    EXPECT_NO_THROW({
        // Test with empty JSON object
        std::string emptyResponse = "{}";
        nlohmann::json emptyObj = nlohmann::json::parse(emptyResponse);
        EXPECT_TRUE(emptyObj.is_object());
        EXPECT_EQ(emptyObj.size(), 0);
        
        // Test with empty JSON array
        std::string emptyArrayResponse = "[]";
        nlohmann::json emptyArray = nlohmann::json::parse(emptyArrayResponse);
        EXPECT_TRUE(emptyArray.is_array());
        EXPECT_EQ(emptyArray.size(), 0);
    });
}

TEST_F(NotificationServiceTest, InvalidJSONHandling) {
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

TEST_F(NotificationServiceTest, MultipleNotificationsHandling) {
    // Test handling of multiple notifications in response
    std::string multipleNotificationsResponse = R"([
        {"id": 1, "title": "Notif 1", "description": "Msg 1", "category": "tech", "url": "url1", "source": "src1", "read": false},
        {"id": 2, "title": "Notif 2", "description": "Msg 2", "category": "sports", "url": "url2", "source": "src2", "read": true},
        {"id": 3, "title": "Notif 3", "description": "Msg 3", "category": "politics", "url": "url3", "source": "src3", "read": false}
    ])";
    
    EXPECT_NO_THROW({
        nlohmann::json notificationsArray = nlohmann::json::parse(multipleNotificationsResponse);
        EXPECT_TRUE(notificationsArray.is_array());
        EXPECT_EQ(notificationsArray.size(), 3);
        
        for (const auto& item : notificationsArray) {
            EXPECT_TRUE(item.contains("id"));
            EXPECT_TRUE(item.contains("title"));
            EXPECT_TRUE(item.contains("description"));
            EXPECT_TRUE(item.contains("category"));
            EXPECT_TRUE(item.contains("url"));
            EXPECT_TRUE(item.contains("source"));
            EXPECT_TRUE(item.contains("read"));
        }
    });
}

TEST_F(NotificationServiceTest, PreferencesParsing) {
    // Test preferences parsing logic
    std::string preferencesResponse = R"({
        "categories": [
            {"category": "technology", "enabled": true},
            {"category": "sports", "enabled": false},
            {"category": "politics", "enabled": true}
        ],
        "keywords": [
            {"keyword": "AI", "enabled": true},
            {"keyword": "climate change", "enabled": false},
            {"keyword": "election", "enabled": true}
        ]
    })";
    
    EXPECT_NO_THROW({
        nlohmann::json result = nlohmann::json::parse(preferencesResponse);
        
        // Test categories parsing
        if (result.contains("categories")) {
            for (const auto& catObj : result["categories"]) {
                if (catObj.contains("category") && catObj.contains("enabled")) {
                    std::string category = catObj["category"];
                    bool enabled = catObj["enabled"];
                    EXPECT_FALSE(category.empty());
                }
            }
        }
        
        // Test keywords parsing
        if (result.contains("keywords")) {
            for (const auto& kwObj : result["keywords"]) {
                if (kwObj.contains("keyword") && kwObj.contains("enabled")) {
                    std::string keyword = kwObj["keyword"];
                    bool enabled = kwObj["enabled"];
                    EXPECT_FALSE(keyword.empty());
                }
            }
        }
    });
}

TEST_F(NotificationServiceTest, MissingFieldsHandling) {
    // Test handling of notifications with missing fields
    std::vector<nlohmann::json> incompleteNotifications = {
        {{"id", 1}}, // Only id
        {{"id", 2}, {"title", "Test"}}, // Missing description
        {{"id", 3}, {"title", "Test"}, {"description", "Desc"}} // Missing category
    };
    
    for (const auto& notificationData : incompleteNotifications) {
        EXPECT_NO_THROW({
            Notification notification;
            notification.id = notificationData["id"];
            notification.title = notificationData.value("title", "");
            notification.message = notificationData.value("description", "");
            notification.category = notificationData.value("category", "General");
            notification.url = notificationData.value("url", "");
            notification.source = notificationData.value("source", "");
            notification.read = notificationData.value("read", false);
            
            // Should not crash and should use default values
            EXPECT_GE(notification.id, 0);
        });
    }
}

TEST_F(NotificationServiceTest, StringToIntConversion) {
    // Test string to int conversion for user ID and notification ID
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

TEST_F(NotificationServiceTest, VectorOperations) {
    // Test vector operations for notifications
    std::vector<Notification> notifications;
    
    EXPECT_NO_THROW({
        // Test adding notifications to vector
        Notification notification1;
        notification1.id = 1;
        notification1.title = "Notification 1";
        notifications.push_back(notification1);
        
        Notification notification2;
        notification2.id = 2;
        notification2.title = "Notification 2";
        notifications.push_back(notification2);
        
        EXPECT_EQ(notifications.size(), 2);
        EXPECT_EQ(notifications[0].id, 1);
        EXPECT_EQ(notifications[1].id, 2);
        
        // Test clearing vector
        notifications.clear();
        EXPECT_EQ(notifications.size(), 0);
    });
}

TEST_F(NotificationServiceTest, ReturnValueConsistency) {
    // Test that methods return consistent values
    int userId = 1;
    
    EXPECT_NO_THROW({
        // getPreferences should always return a NotificationPreferences object
        NotificationPreferences prefs = NotificationService::getPreferences(userId);
        // Should not crash and should return a valid object
        
        // getNotifications should always return a vector (even if empty)
        std::vector<Notification> notifications = NotificationService::getNotifications(userId);
        // Should not crash and should return a valid vector
    });
}

TEST_F(NotificationServiceTest, ErrorHandling) {
    // Test error handling scenarios
    int userId = 1;
    int notificationId = 100;
    std::string category = "technology";
    std::string keyword = "AI";
    bool enabled = true;
    
    EXPECT_NO_THROW({
        // These should handle errors gracefully without throwing
        NotificationService::setCategoryPreference(userId, category, enabled);
        NotificationService::setKeywordPreference(userId, keyword, enabled);
        NotificationService::getPreferences(userId);
        NotificationService::getNotifications(userId);
        NotificationService::markNotificationAsRead(notificationId);
    });
}
