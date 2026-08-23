#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../routes/notification_routes.hpp"
#include "../../services/NotificationService.hpp"
#include "../../database/Database.hpp"
#include <pistache/router.h>
#include <pistache/http.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace Pistache;

// Mock for NotificationService
class MockNotificationService {
public:
    static bool setCategoryPreference(int userId, const std::string& category, bool enabled) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        lastCategoryPref = {userId, category, enabled};
        return true;
    }
    
    static bool setKeywordPreference(int userId, const std::string& keyword, bool enabled) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        lastKeywordPref = {userId, keyword, enabled};
        return true;
    }
    
    static json getUserPreferences(int userId) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        
        // Return mock preferences for userId 42
        if (userId == 42) {
            return {
                {"categories", {"technology", "business"}},
                {"keywords", {"ai", "blockchain"}}
            };
        }
        
        // Empty preferences for other users
        return {
            {"categories", json::array()},
            {"keywords", json::array()}
        };
    }
    
    static json getDeliveredNotifications(int userId) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        
        // Return mock notifications for userId 42
        if (userId == 42) {
            json notifications = json::array();
            notifications.push_back({
                {"id", 1},
                {"article_id", 101},
                {"title", "New AI Article"},
                {"read", false},
                {"created_at", "2023-07-05T12:00:00Z"}
            });
            notifications.push_back({
                {"id", 2},
                {"article_id", 102},
                {"title", "Blockchain Revolution"},
                {"read", true},
                {"created_at", "2023-07-04T10:30:00Z"}
            });
            return notifications;
        }
        
        // Empty notifications for other users
        return json::array();
    }
    
    static void reset() {
        shouldThrow = false;
        lastCategoryPref = {0, "", false};
        lastKeywordPref = {0, "", false};
    }
    
    static bool shouldThrow;
    static std::tuple<int, std::string, bool> lastCategoryPref;
    static std::tuple<int, std::string, bool> lastKeywordPref;
};

bool MockNotificationService::shouldThrow = false;
std::tuple<int, std::string, bool> MockNotificationService::lastCategoryPref = {0, "", false};
std::tuple<int, std::string, bool> MockNotificationService::lastKeywordPref = {0, "", false};

// Mock for Database functions specific to notifications
class MockDatabase {
public:
    static bool markNotificationsAsRead(int userId) {
        if (shouldThrow) {
            throw std::runtime_error("Database error");
        }
        lastMarkedReadUserId = userId;
        return true;
    }
    
    static void reset() {
        shouldThrow = false;
        lastMarkedReadUserId = 0;
    }
    
    static bool shouldThrow;
    static int lastMarkedReadUserId;
};

bool MockDatabase::shouldThrow = false;
int MockDatabase::lastMarkedReadUserId = 0;

// Test fixture for NotificationRoutes
class NotificationRoutesTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset the mock services before each test
        MockNotificationService::reset();
        MockDatabase::reset();
        
        // Store original functions
        original_setCategoryPreference = NotificationService::setCategoryPreference;
        original_setKeywordPreference = NotificationService::setKeywordPreference;
        original_getUserPreferences = NotificationService::getUserPreferences;
        original_getDeliveredNotifications = NotificationService::getDeliveredNotifications;
        original_markNotificationsAsRead = Database::markNotificationsAsRead;
        
        // Replace with mocks
        NotificationService::setCategoryPreference = MockNotificationService::setCategoryPreference;
        NotificationService::setKeywordPreference = MockNotificationService::setKeywordPreference;
        NotificationService::getUserPreferences = MockNotificationService::getUserPreferences;
        NotificationService::getDeliveredNotifications = MockNotificationService::getDeliveredNotifications;
        Database::markNotificationsAsRead = MockDatabase::markNotificationsAsRead;
    }
    
    void TearDown() override {
        // Restore original functions
        NotificationService::setCategoryPreference = original_setCategoryPreference;
        NotificationService::setKeywordPreference = original_setKeywordPreference;
        NotificationService::getUserPreferences = original_getUserPreferences;
        NotificationService::getDeliveredNotifications = original_getDeliveredNotifications;
        Database::markNotificationsAsRead = original_markNotificationsAsRead;
    }
    
    // Store original function pointers
    std::function<bool(int, const std::string&, bool)> original_setCategoryPreference;
    std::function<bool(int, const std::string&, bool)> original_setKeywordPreference;
    std::function<json(int)> original_getUserPreferences;
    std::function<json(int)> original_getDeliveredNotifications;
    std::function<bool(int)> original_markNotificationsAsRead;
    
    // Helper to create a mock request
    Rest::Request createMockRequest(const std::string& body = "", 
                                  const std::unordered_map<std::string, std::string>& params = {}) {
        // Create a mock request with the given body and params
        Http::Request httpReq;
        httpReq.body(body);
        
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

// Test setCategoryPrefHandler
TEST_F(NotificationRoutesTest, SetCategoryPrefHandler) {
    // Create request body for setting category preference
    json body = {
        {"user_id", 42},
        {"category", "technology"},
        {"enabled", true}
    };
    
    // Create a mock request with body
    auto req = createMockRequest(body.dump());
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = setCategoryPrefHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
    
    // Verify the service method was called correctly
    EXPECT_EQ(std::get<0>(MockNotificationService::lastCategoryPref), 42);
    EXPECT_EQ(std::get<1>(MockNotificationService::lastCategoryPref), "technology");
    EXPECT_EQ(std::get<2>(MockNotificationService::lastCategoryPref), true);
}

// Test setKeywordPrefHandler
TEST_F(NotificationRoutesTest, SetKeywordPrefHandler) {
    // Create request body for setting keyword preference
    json body = {
        {"user_id", 42},
        {"keyword", "blockchain"},
        {"enabled", true}
    };
    
    // Create a mock request with body
    auto req = createMockRequest(body.dump());
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = setKeywordPrefHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
    
    // Verify the service method was called correctly
    EXPECT_EQ(std::get<0>(MockNotificationService::lastKeywordPref), 42);
    EXPECT_EQ(std::get<1>(MockNotificationService::lastKeywordPref), "blockchain");
    EXPECT_EQ(std::get<2>(MockNotificationService::lastKeywordPref), true);
}

// Test getPreferencesHandler
TEST_F(NotificationRoutesTest, GetPreferencesHandler) {
    // Create parameters map with userId
    std::unordered_map<std::string, std::string> params;
    params[":userId"] = "42";
    
    // Create a mock request with params
    auto req = createMockRequest("", params);
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = getPreferencesHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
}

// Test getDeliveredNotificationsHandler
TEST_F(NotificationRoutesTest, GetDeliveredNotificationsHandler) {
    // Create parameters map with userId
    std::unordered_map<std::string, std::string> params;
    params[":userId"] = "42";
    
    // Create a mock request with params
    auto req = createMockRequest("", params);
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = getDeliveredNotificationsHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
}

// Test markNotificationsAsReadHandler
TEST_F(NotificationRoutesTest, MarkNotificationsAsReadHandler) {
    // Create parameters map with userId
    std::unordered_map<std::string, std::string> params;
    params[":userId"] = "42";
    
    // Create a mock request with params
    auto req = createMockRequest("", params);
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = markNotificationsAsReadHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
    
    // Verify the database method was called correctly
    EXPECT_EQ(MockDatabase::lastMarkedReadUserId, 42);
}

// Test error handling in setCategoryPrefHandler
TEST_F(NotificationRoutesTest, SetCategoryPrefHandlerError) {
    // Configure the mock to throw an exception
    MockNotificationService::shouldThrow = true;
    
    // Create request body
    json body = {
        {"user_id", 42},
        {"category", "technology"},
        {"enabled", true}
    };
    
    // Create a mock request with body
    auto req = createMockRequest(body.dump());
    auto resp = createResponseWriter();
    
    // Call the route handler directly - should handle the exception gracefully
    EXPECT_NO_THROW({
        auto result = setCategoryPrefHandler(req, std::move(resp));
        EXPECT_EQ(result, Rest::Route::Result::Ok);
    });
    
    // Reset the mock
    MockNotificationService::shouldThrow = false;
}

// Test error handling in getPreferencesHandler
TEST_F(NotificationRoutesTest, GetPreferencesHandlerError) {
    // Configure the mock to throw an exception
    MockNotificationService::shouldThrow = true;
    
    // Create parameters map with userId
    std::unordered_map<std::string, std::string> params;
    params[":userId"] = "42";
    
    // Create a mock request with params
    auto req = createMockRequest("", params);
    auto resp = createResponseWriter();
    
    // Call the route handler directly - should handle the exception gracefully
    EXPECT_NO_THROW({
        auto result = getPreferencesHandler(req, std::move(resp));
        EXPECT_EQ(result, Rest::Route::Result::Ok);
    });
    
    // Reset the mock
    MockNotificationService::shouldThrow = false;
}

// Test error handling in markNotificationsAsReadHandler
TEST_F(NotificationRoutesTest, MarkNotificationsAsReadHandlerError) {
    // Configure the mock to throw an exception
    MockDatabase::shouldThrow = true;
    
    // Create parameters map with userId
    std::unordered_map<std::string, std::string> params;
    params[":userId"] = "42";
    
    // Create a mock request with params
    auto req = createMockRequest("", params);
    auto resp = createResponseWriter();
    
    // Call the route handler directly - should handle the exception gracefully
    EXPECT_NO_THROW({
        auto result = markNotificationsAsReadHandler(req, std::move(resp));
        EXPECT_EQ(result, Rest::Route::Result::Ok);
    });
    
    // Reset the mock
    MockDatabase::shouldThrow = false;
}

// Test route setup
TEST_F(NotificationRoutesTest, RouteSetup) {
    // Create a router
    Rest::Router router;
    
    // This is a structural test to ensure setup doesn't throw
    EXPECT_NO_THROW(NotificationRoutes::setup(router));
    
    // Ideally, we would verify that the correct routes were registered:
    // - POST /notifications/category
    // - POST /notifications/keyword
    // - GET /notifications/preferences/:userId
    // - GET /notifications/:userId
    // - POST /notifications/markread/:userId
}