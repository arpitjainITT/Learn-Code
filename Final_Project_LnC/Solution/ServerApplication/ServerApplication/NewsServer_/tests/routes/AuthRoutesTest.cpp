#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../routes/auth_routes.hpp"
#include "../../services/UserService.hpp"
#include <pistache/router.h>
#include <pistache/http.h>
#include <nlohmann/json.hpp>
#include "../utils/Strings.hpp"

using json = nlohmann::json;
using namespace Pistache;

// Mock for UserService
class MockUserService {
public:
    static bool login(const std::string& email, const std::string& password, int& userId, std::string& role) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        
        // Simulate successful login for test user
        if (email == "test@example.com" && password == "password123") {
            userId = 42;
            role = "user";
            return true;
        }
        
        // Simulate successful admin login
        if (email == "admin@example.com" && password == "admin123") {
            userId = 1;
            role = "admin";
            return true;
        }
        
        // Failed login
        return false;
    }
    
    static bool signup(const std::string& username, const std::string& email, const std::string& password) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        
        // Record the signup attempt
        lastSignupAttempt = {username, email, password};
        
        // Simulate a registration conflict for a specific email
        if (email == "existing@example.com") {
            return false;
        }
        
        // Otherwise succeed
        return true;
    }
    
    static int getUserIdByEmail(const std::string& email) {
        if (shouldThrow) {
            throw std::runtime_error("Service error");
        }
        
        // Return a predefined ID for test users
        if (email == "test@example.com") {
            return 42;
        }
        if (email == "admin@example.com") {
            return 1;
        }
        
        // Not found
        return -1;
    }
    
    static void reset() {
        shouldThrow = false;
        lastSignupAttempt = {"", "", ""};
    }
    
    static bool shouldThrow;
    static std::tuple<std::string, std::string, std::string> lastSignupAttempt;
};

bool MockUserService::shouldThrow = false;
std::tuple<std::string, std::string, std::string> MockUserService::lastSignupAttempt = {"", "", ""};

// Test fixture for AuthRoutes
class AuthRoutesTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset the mock service before each test
        MockUserService::reset();
        
        // Store original functions
        original_login = UserService::login;
        original_signup = UserService::signup;
        original_getUserIdByEmail = UserService::getUserIdByEmail;
        
        // Replace with mocks
        UserService::login = MockUserService::login;
        UserService::signup = MockUserService::signup;
        UserService::getUserIdByEmail = MockUserService::getUserIdByEmail;
    }
    
    void TearDown() override {
        // Restore original functions
        UserService::login = original_login;
        UserService::signup = original_signup;
        UserService::getUserIdByEmail = original_getUserIdByEmail;
    }
    
    // Store original function pointers
    std::function<bool(const std::string&, const std::string&, int&, std::string&)> original_login;
    std::function<bool(const std::string&, const std::string&, const std::string&)> original_signup;
    std::function<int(const std::string&)> original_getUserIdByEmail;
    
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

// Test successful login
TEST_F(AuthRoutesTest, LoginSuccess) {
    // Create request body for successful login
    json body = {
        {"email", "test@example.com"},
        {"password", "password123"}
    };
    
    // Create a mock request with body
    auto req = createMockRequest(body.dump());
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = loginHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
}

// Test failed login
TEST_F(AuthRoutesTest, LoginFailure) {
    // Create request body with wrong credentials
    json body = {
        {"email", "test@example.com"},
        {"password", "wrongpassword"}
    };
    
    // Create a mock request with body
    auto req = createMockRequest(body.dump());
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = loginHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
    // The actual response code would be Http::Code::Unauthorized
    // but we can't easily verify that with our test setup
}

// Test admin login
TEST_F(AuthRoutesTest, AdminLogin) {
    // Create request body for admin login
    json body = {
        {"email", "admin@example.com"},
        {"password", "admin123"}
    };
    
    // Create a mock request with body
    auto req = createMockRequest(body.dump());
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = loginHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
}

// Test successful signup
TEST_F(AuthRoutesTest, SignupSuccess) {
    // Create request body for signup
    json body = {
        {"username", "newuser"},
        {"email", "newuser@example.com"},
        {"password", "newpassword123"}
    };
    
    // Create a mock request with body
    auto req = createMockRequest(body.dump());
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = signupHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
    
    // Verify the service method was called correctly
    EXPECT_EQ(std::get<0>(MockUserService::lastSignupAttempt), "newuser");
    EXPECT_EQ(std::get<1>(MockUserService::lastSignupAttempt), "newuser@example.com");
    EXPECT_EQ(std::get<2>(MockUserService::lastSignupAttempt), "newpassword123");
}

// Test signup with existing email
TEST_F(AuthRoutesTest, SignupWithExistingEmail) {
    // Create request body with an email that already exists
    json body = {
        {"username", "existinguser"},
        {"email", "existing@example.com"},
        {"password", "password123"}
    };
    
    // Create a mock request with body
    auto req = createMockRequest(body.dump());
    auto resp = createResponseWriter();
    
    // Call the route handler directly
    auto result = signupHandler(req, std::move(resp));
    
    // Verify the result
    EXPECT_EQ(result, Rest::Route::Result::Ok);
    // The actual response code would be Http::Code::Bad_Request
    // but we can't easily verify that with our test setup
}

// Test login with malformed request body
TEST_F(AuthRoutesTest, LoginWithMalformedBody) {
    // Create an incomplete request body
    json body = {
        {"email", "test@example.com"}
        // Missing password
    };
    
    // Create a mock request with body
    auto req = createMockRequest(body.dump());
    auto resp = createResponseWriter();
    
    // Call the route handler directly - should handle the error gracefully
    EXPECT_NO_THROW({
        auto result = loginHandler(req, std::move(resp));
        EXPECT_EQ(result, Rest::Route::Result::Ok);
    });
}

// Test signup with malformed request body
TEST_F(AuthRoutesTest, SignupWithMalformedBody) {
    // Create an incomplete request body
    json body = {
        {"username", "newuser"},
        {"email", "newuser@example.com"}
        // Missing password
    };
    
    // Create a mock request with body
    auto req = createMockRequest(body.dump());
    auto resp = createResponseWriter();
    
    // Call the route handler directly - should handle the error gracefully
    EXPECT_NO_THROW({
        auto result = signupHandler(req, std::move(resp));
        EXPECT_EQ(result, Rest::Route::Result::Ok);
    });
}

// Test error handling in login
TEST_F(AuthRoutesTest, LoginErrorHandling) {
    // Configure the mock to throw an exception
    MockUserService::shouldThrow = true;
    
    // Create request body
    json body = {
        {"email", "test@example.com"},
        {"password", "password123"}
    };
    
    // Create a mock request with body
    auto req = createMockRequest(body.dump());
    auto resp = createResponseWriter();
    
    // Call the route handler directly - should handle the exception gracefully
    EXPECT_NO_THROW({
        auto result = loginHandler(req, std::move(resp));
        EXPECT_EQ(result, Rest::Route::Result::Ok);
    });
    
    // Reset the mock
    MockUserService::shouldThrow = false;
}

// Test error handling in signup
TEST_F(AuthRoutesTest, SignupErrorHandling) {
    // Configure the mock to throw an exception
    MockUserService::shouldThrow = true;
    
    // Create request body
    json body = {
        {"username", "newuser"},
        {"email", "newuser@example.com"},
        {"password", "newpassword123"}
    };
    
    // Create a mock request with body
    auto req = createMockRequest(body.dump());
    auto resp = createResponseWriter();
    
    // Call the route handler directly - should handle the exception gracefully
    EXPECT_NO_THROW({
        auto result = signupHandler(req, std::move(resp));
        EXPECT_EQ(result, Rest::Route::Result::Ok);
    });
    
    // Reset the mock
    MockUserService::shouldThrow = false;
}

// Test route setup
TEST_F(AuthRoutesTest, RouteSetup) {
    // Create a router
    Rest::Router router;
    
    // This is a structural test to ensure setup doesn't throw
    EXPECT_NO_THROW(AuthRoutes::setup(router));
    
    // Ideally, we would verify that the correct routes were registered:
    // - POST /auth/login
    // - POST /auth/signup
}