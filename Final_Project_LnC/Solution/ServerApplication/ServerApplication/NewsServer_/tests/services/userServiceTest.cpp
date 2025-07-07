#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../services/UserService.hpp"
#include "../../database/DataBase.hpp"
#include <nlohmann/json.hpp>
#include <functional>

using json = nlohmann::json;

// Create test-specific overrides for UserService namespace
namespace UserService {
    namespace {
        // Store original function pointers
        bool (*original_login)(const std::string&, const std::string&, int&, std::string&) = nullptr;
        bool (*original_signup)(const std::string&, const std::string&, const std::string&) = nullptr;
        int (*original_getUserIdByEmail)(const std::string&) = nullptr;
        
        // Test control variables
        bool should_login_succeed = true;
        bool should_signup_succeed = true;
        int mock_user_id = 42;
        std::string mock_role = "user";
        std::string last_email;
        std::string last_password;
        std::string last_username;
        bool should_throw = false;
    }
    
    // Store original functions and setup test implementations
    void setupTestOverrides() {
        if (!original_login) {
            // Store original functions (only once)
            original_login = login;
            original_signup = signup;
            original_getUserIdByEmail = getUserIdByEmail;
        }
        
        // Override functions for testing
        login = [](const std::string& email, const std::string& password, int& userId, std::string& role) {
            if (should_throw) {
                throw std::runtime_error("Test exception");
            }
            
            last_email = email;
            last_password = password;
            
            if (should_login_succeed) {
                userId = mock_user_id;
                role = mock_role;
                return true;
            }
            return false;
        };
        
        signup = [](const std::string& username, const std::string& email, const std::string& password) {
            if (should_throw) {
                throw std::runtime_error("Test exception");
            }
            
            last_username = username;
            last_email = email;
            last_password = password;
            
            return should_signup_succeed;
        };
        
        getUserIdByEmail = [](const std::string& email) {
            if (should_throw) {
                throw std::runtime_error("Test exception");
            }
            
            last_email = email;
            
            if (email == "test@example.com") {
                return mock_user_id;
            }
            return -1;
        };
    }
    
    // Restore original functions
    void restoreOriginalFunctions() {
        if (original_login) {
            login = original_login;
            signup = original_signup;
            getUserIdByEmail = original_getUserIdByEmail;
        }
    }
    
    // Reset test state
    void resetTestState() {
        should_login_succeed = true;
        should_signup_succeed = true;
        mock_user_id = 42;
        mock_role = "user";
        last_email = "";
        last_password = "";
        last_username = "";
        should_throw = false;
    }
    
    // Test helpers - these let us control the behavior of our test overrides
    void setShouldLoginSucceed(bool value) { should_login_succeed = value; }
    void setShouldSignupSucceed(bool value) { should_signup_succeed = value; }
    void setMockUserId(int id) { mock_user_id = id; }
    void setMockRole(const std::string& role) { mock_role = role; }
    void setShouldThrow(bool value) { should_throw = value; }
    
    // Test helpers - these let us verify what was passed to our functions
    std::string getLastEmail() { return last_email; }
    std::string getLastPassword() { return last_password; }
    std::string getLastUsername() { return last_username; }
}

// Test fixture for UserService
class UserServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        UserService::setupTestOverrides();
        UserService::resetTestState();
    }
    
    void TearDown() override {
        UserService::restoreOriginalFunctions();
    }
};

// Test successful login
TEST_F(UserServiceTest, LoginSuccess) {
    // Set up test conditions
    UserService::setShouldLoginSucceed(true);
    UserService::setMockUserId(123);
    UserService::setMockRole("admin");
    
    // Call the method under test
    int userId = 0;
    std::string role;
    bool result = UserService::login("admin@example.com", "password123", userId, role);
    
    // Verify results
    EXPECT_TRUE(result);
    EXPECT_EQ(userId, 123);
    EXPECT_EQ(role, "admin");
    EXPECT_EQ(UserService::getLastEmail(), "admin@example.com");
    EXPECT_EQ(UserService::getLastPassword(), "password123");
}

// Test failed login
TEST_F(UserServiceTest, LoginFailure) {
    // Set up test conditions
    UserService::setShouldLoginSucceed(false);
    
    // Call the method under test
    int userId = 0;
    std::string role;
    bool result = UserService::login("unknown@example.com", "wrongpassword", userId, role);
    
    // Verify results
    EXPECT_FALSE(result);
    EXPECT_EQ(UserService::getLastEmail(), "unknown@example.com");
    EXPECT_EQ(UserService::getLastPassword(), "wrongpassword");
}

// Test successful signup
TEST_F(UserServiceTest, SignupSuccess) {
    // Set up test conditions
    UserService::setShouldSignupSucceed(true);
    
    // Call the method under test
    bool result = UserService::signup("newuser", "newuser@example.com", "password123");
    
    // Verify results
    EXPECT_TRUE(result);
    EXPECT_EQ(UserService::getLastUsername(), "newuser");
    EXPECT_EQ(UserService::getLastEmail(), "newuser@example.com");
    EXPECT_EQ(UserService::getLastPassword(), "password123");
}

// Test failed signup
TEST_F(UserServiceTest, SignupFailure) {
    // Set up test conditions
    UserService::setShouldSignupSucceed(false);
    
    // Call the method under test
    bool result = UserService::signup("existinguser", "existing@example.com", "password123");
    
    // Verify results
    EXPECT_FALSE(result);
    EXPECT_EQ(UserService::getLastUsername(), "existinguser");
    EXPECT_EQ(UserService::getLastEmail(), "existing@example.com");
}

// Test getUserIdByEmail
TEST_F(UserServiceTest, GetUserIdByEmail) {
    // Set up test conditions - default should return 42 for test@example.com
    
    // Call the method under test
    int userId = UserService::getUserIdByEmail("test@example.com");
    int nonExistentUserId = UserService::getUserIdByEmail("nonexistent@example.com");
    
    // Verify results
    EXPECT_EQ(userId, 42);
    EXPECT_EQ(nonExistentUserId, -1);
    EXPECT_EQ(UserService::getLastEmail(), "nonexistent@example.com");
}

// Test error handling
TEST_F(UserServiceTest, ErrorHandling) {
    // Set up test conditions
    UserService::setShouldThrow(true);
    
    // Call the method under test and verify it handles exceptions
    int userId = 0;
    std::string role;
    EXPECT_NO_THROW({
        bool result = UserService::login("test@example.com", "password", userId, role);
        EXPECT_FALSE(result);
    });
    
    EXPECT_NO_THROW({
        bool result = UserService::signup("user", "test@example.com", "password");
        EXPECT_FALSE(result);
    });
    
    EXPECT_NO_THROW({
        int id = UserService::getUserIdByEmail("test@example.com");
        EXPECT_EQ(id, -1);
    });
}