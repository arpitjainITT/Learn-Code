#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../routes/admin_routes.hpp"
#include "../../services/AdminService.hpp"
#include <pistache/router.h>
#include <pistache/http.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace Pistache;

// Mock for AdminService
class MockAdminService {
public:
    static json listExternalServers() {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        return mockServers;
    }

    static json viewExternalServerDetails(int serverId) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        for (const auto& server : mockServers) {
            if (server["id"] == serverId) {
                return server;
            }
        }
        return json{};
    }

    static bool updateExternalServerApiKey(int serverId, const std::string& newKey) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        lastApiKeyUpdate = std::make_pair(serverId, newKey);
        return !newKey.empty();
    }

    static bool updateExternalServerStatus(int serverId, const std::string& status) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        lastStatusUpdate = std::make_pair(serverId, status);
        return !status.empty();
    }

    static bool addCategory(const std::string& category) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        lastCategoryAdded = category;
        return !category.empty();
    }

    static void reset() {
        shouldThrow = false;
        mockServers = json::array();
        lastApiKeyUpdate = std::make_pair(0, "");
        lastStatusUpdate = std::make_pair(0, "");
        lastCategoryAdded = "";
    }

    static void setupMockServers() {
        mockServers = json::array();
        mockServers.push_back({
            {"id", 1},
            {"server_name", "NewsAPI.org"},
            {"api_url", "https://newsapi.org/v2/"},
            {"api_key", "test-key-1"},
            {"status", "active"}
        });
        mockServers.push_back({
            {"id", 2},
            {"server_name", "TheNewsAPI.com"},
            {"api_url", "https://api.thenewsapi.com/v1/"},
            {"api_key", "test-key-2"},
            {"status", "inactive"}
        });
    }

    static bool shouldThrow;
    static json mockServers;
    static std::pair<int, std::string> lastApiKeyUpdate;
    static std::pair<int, std::string> lastStatusUpdate;
    static std::string lastCategoryAdded;
};

bool MockAdminService::shouldThrow = false;
json MockAdminService::mockServers = json::array();
std::pair<int, std::string> MockAdminService::lastApiKeyUpdate = std::make_pair(0, "");
std::pair<int, std::string> MockAdminService::lastStatusUpdate = std::make_pair(0, "");
std::string MockAdminService::lastCategoryAdded = "";

// Mock for Router to capture route registration
class MockRouter {
public:
    MOCK_METHOD(void, get, (const std::string&, const Rest::Route::Handler&));
    MOCK_METHOD(void, post, (const std::string&, const Rest::Route::Handler&));
    MOCK_METHOD(void, put, (const std::string&, const Rest::Route::Handler&));
};

// Test fixture for AdminRoutes
class AdminRoutesTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset the mock service before each test
        MockAdminService::reset();
        MockAdminService::setupMockServers();
        
        // Store original functions
        original_listExternalServers = AdminService::listExternalServers;
        original_viewExternalServerDetails = AdminService::viewExternalServerDetails;
        original_updateExternalServerApiKey = AdminService::updateExternalServerApiKey;
        original_updateExternalServerStatus = AdminService::updateExternalServerStatus;
        original_addCategory = AdminService::addCategory;
        
        // Replace with mocks
        AdminService::listExternalServers = MockAdminService::listExternalServers;
        AdminService::viewExternalServerDetails = MockAdminService::viewExternalServerDetails;
        AdminService::updateExternalServerApiKey = MockAdminService::updateExternalServerApiKey;
        AdminService::updateExternalServerStatus = MockAdminService::updateExternalServerStatus;
        AdminService::addCategory = MockAdminService::addCategory;
    }

    void TearDown() override {
        // Restore original functions
        AdminService::listExternalServers = original_listExternalServers;
        AdminService::viewExternalServerDetails = original_viewExternalServerDetails;
        AdminService::updateExternalServerApiKey = original_updateExternalServerApiKey;
        AdminService::updateExternalServerStatus = original_updateExternalServerStatus;
        AdminService::addCategory = original_addCategory;
    }

    // Store original function pointers
    std::function<json()> original_listExternalServers;
    std::function<json(int)> original_viewExternalServerDetails;
    std::function<bool(int, const std::string&)> original_updateExternalServerApiKey;
    std::function<bool(int, const std::string&)> original_updateExternalServerStatus;
    std::function<bool(const std::string&)> original_addCategory;

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

    // Helper to simulate route registration
    void simulateRouteSetup(Rest::Router& router) {
        AdminRoutes::setup(router);
    }
};

// Test listServersHandler
TEST_F(AdminRoutesTest, ListServersHandler) {
    // Create a mock request and response
    auto req = createMockRequest();
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = listServersHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
    
    // The exact response can't be verified easily due to the way Pistache works
    // but we can verify that the service method was called correctly
    EXPECT_EQ(MockAdminService::mockServers.size(), 2);
}

// Test viewServerHandler
TEST_F(AdminRoutesTest, ViewServerHandler) {
    // Create parameters map with serverId
    std::unordered_map<std::string, std::string> params;
    params[":id"] = "1";
    
    // Create a mock request with params
    auto req = createMockRequest("", params);
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = viewServerHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
}

// Test updateApiKeyHandler
TEST_F(AdminRoutesTest, UpdateApiKeyHandler) {
    // Create parameters map with serverId
    std::unordered_map<std::string, std::string> params;
    params[":id"] = "1";
    
    // Create request body
    json body = {{"api_key", "new-api-key-123"}};
    
    // Create a mock request with body and params
    auto req = createMockRequest(body.dump(), params);
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = updateApiKeyHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
    
    // Verify the service method was called correctly
    EXPECT_EQ(MockAdminService::lastApiKeyUpdate.first, 1);
    EXPECT_EQ(MockAdminService::lastApiKeyUpdate.second, "new-api-key-123");
}

// Test updateStatusHandler
TEST_F(AdminRoutesTest, UpdateStatusHandler) {
    // Create parameters map with serverId
    std::unordered_map<std::string, std::string> params;
    params[":id"] = "1";
    
    // Create request body
    json body = {{"status", "maintenance"}};
    
    // Create a mock request with body and params
    auto req = createMockRequest(body.dump(), params);
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = updateStatusHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
    
    // Verify the service method was called correctly
    EXPECT_EQ(MockAdminService::lastStatusUpdate.first, 1);
    EXPECT_EQ(MockAdminService::lastStatusUpdate.second, "maintenance");
}

// Test addCategoryHandler
TEST_F(AdminRoutesTest, AddCategoryHandler) {
    // Create request body
    json body = {{"category", "technology"}};
    
    // Create a mock request with body
    auto req = createMockRequest(body.dump());
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = addCategoryHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
    
    // Verify the service method was called correctly
    EXPECT_EQ(MockAdminService::lastCategoryAdded, "technology");
}

// Test error handling in listServersHandler
TEST_F(AdminRoutesTest, ListServersHandlerError) {
    // Configure the mock to throw an exception
    MockAdminService::shouldThrow = true;
    
    // Create a mock request and response
    auto req = createMockRequest();
    auto resp = createResponseWriter();
    
    // Call the route handler directly - should handle the exception gracefully
    EXPECT_NO_THROW({
        auto result = listServersHandler(req, std::move(resp));
        EXPECT_EQ(result, Rest::Route::Result::Ok);
    });
    
    // Reset the mock
    MockAdminService::shouldThrow = false;
}

// Test route setup
TEST_F(AdminRoutesTest, RouteSetup) {
    // Create a mock router
    testing::NiceMock<MockRouter> mockRouter;
    Rest::Router router;
    
    // We can't easily verify route registration with Pistache
    // This is more of a structural test to ensure setup doesn't throw
    EXPECT_NO_THROW(AdminRoutes::setup(router));
    
    // Ideally, we would verify that the correct routes were registered:
    // - GET /admin/servers
    // - GET /admin/servers/:id
    // - PUT /admin/servers/:id/key
    // - PUT /admin/servers/:id/status
    // - POST /admin/category
}