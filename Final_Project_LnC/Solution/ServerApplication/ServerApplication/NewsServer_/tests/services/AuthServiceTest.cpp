#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "services/AuthService.hpp"
#include <nlohmann/json.hpp>
#include <string>

// Mock for Database class
class MockDatabase {
public:
    static testing::NiceMock<MockDatabase>& getInstance() {
        static testing::NiceMock<MockDatabase> instance;
        return instance;
    }

    MOCK_METHOD(bool, authenticateUser, (const std::string&, const std::string&, nlohmann::json&));
    MOCK_METHOD(bool, registerUser, (const std::string&, const std::string&, const std::string&));
    MOCK_METHOD(bool, checkUserExists, (const std::string&));
    MOCK_METHOD(nlohmann::json, getUserProfile, (int));
    MOCK_METHOD(bool, updateUserProfile, (int, const nlohmann::json&));
    MOCK_METHOD(bool, changePassword, (int, const std::string&, const std::string&));
};

// Test fixture for AuthService
class AuthServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up mock behavior before each test
    }

    void TearDown() override {
        // Clean up after each test
    }

    // Helper to create a sample user
    nlohmann::json createSampleUser() {
        return {
            {"id", 123},
            {"username", "testuser"},
            {"email", "test@example.com"},
            {"role", "user"},
            {"created_at", "2023-07-01T10:00:00Z"}
        };
    }
};

// Test user login with valid credentials
TEST_F(AuthServiceTest, LoginValidCredentials) {
    // Set up expectations
    std::string email = "test@example.com";
    std::string password = "password123";
    nlohmann::json userInfo = createSampleUser();
    
    EXPECT_CALL(MockDatabase::getInstance(), authenticateUser(email, password, testing::_))
        .WillOnce(testing::DoAll(
            testing::SetArgReferee<2>(userInfo),
            testing::Return(true)
        ));

    // Call method under test
    nlohmann::json result;
    bool success = AuthService::login(email, password, result);

    // Verify results
    EXPECT_TRUE(success);
    EXPECT_EQ(result["id"], 123);
    EXPECT_EQ(result["username"], "testuser");
    EXPECT_EQ(result["email"], "test@example.com");
    EXPECT_EQ(result["role"], "user");
}

// Test user login with invalid credentials
TEST_F(AuthServiceTest, LoginInvalidCredentials) {
    // Set up expectations
    std::string email = "test@example.com";
    std::string password = "wrongpassword";
    nlohmann::json userInfo;
    
    EXPECT_CALL(MockDatabase::getInstance(), authenticateUser(email, password, testing::_))
        .WillOnce(testing::Return(false));

    // Call method under test
    nlohmann::json result;
    bool success = AuthService::login(email, password, result);

    // Verify results
    EXPECT_FALSE(success);
    EXPECT_TRUE(result.empty());
}

// Test user login with empty credentials
TEST_F(AuthServiceTest, LoginEmptyCredentials) {
    // Set up expectations
    std::string email = "";
    std::string password = "";
    nlohmann::json userInfo;
    
    EXPECT_CALL(MockDatabase::getInstance(), authenticateUser(email, password, testing::_))
        .WillOnce(testing::Return(false));

    // Call method under test
    nlohmann::json result;
    bool success = AuthService::login(email, password, result);

    // Verify results
    EXPECT_FALSE(success);
    EXPECT_TRUE(result.empty());
}

// Test user registration with valid data
TEST_F(AuthServiceTest, RegisterValidData) {
    // Set up expectations
    std::string username = "newuser";
    std::string email = "newuser@example.com";
    std::string password = "password123";
    
    EXPECT_CALL(MockDatabase::getInstance(), checkUserExists(email))
        .WillOnce(testing::Return(false));
    
    EXPECT_CALL(MockDatabase::getInstance(), registerUser(username, email, password))
        .WillOnce(testing::Return(true));

    // Call method under test
    bool success = AuthService::signup(username, email, password);

    // Verify results
    EXPECT_TRUE(success);
}

// Test user registration with existing email
TEST_F(AuthServiceTest, RegisterExistingEmail) {
    // Set up expectations
    std::string username = "newuser";
    std::string email = "existing@example.com";
    std::string password = "password123";
    
    EXPECT_CALL(MockDatabase::getInstance(), checkUserExists(email))
        .WillOnce(testing::Return(true));
    
    // registerUser should not be called

    // Call method under test
    bool success = AuthService::signup(username, email, password);

    // Verify results
    EXPECT_FALSE(success);
}

// Test user registration with empty data
TEST_F(AuthServiceTest, RegisterEmptyData) {
    // Set up expectations
    std::string username = "";
    std::string email = "";
    std::string password = "";
    
    // Depending on implementation, may or may not check for empty strings
    EXPECT_CALL(MockDatabase::getInstance(), checkUserExists(email))
        .WillOnce(testing::Return(false));
    
    EXPECT_CALL(MockDatabase::getInstance(), registerUser(username, email, password))
        .WillOnce(testing::Return(false));

    // Call method under test
    bool success = AuthService::signup(username, email, password);

    // Verify results
    EXPECT_FALSE(success);
}

// Test getting user profile
TEST_F(AuthServiceTest, GetUserProfile) {
    // Set up expectations
    int userId = 123;
    nlohmann::json userProfile = createSampleUser();
    
    EXPECT_CALL(MockDatabase::getInstance(), getUserProfile(userId))
        .WillOnce(testing::Return(userProfile));

    // Call method under test
    auto result = AuthService::getUserProfile(userId);

    // Verify results
    EXPECT_EQ(result["id"], 123);
    EXPECT_EQ(result["username"], "testuser");
    EXPECT_EQ(result["email"], "test@example.com");
}

// Test getting profile for non-existent user
TEST_F(AuthServiceTest, GetNonExistentUserProfile) {
    // Set up expectations
    int userId = 999;
    nlohmann::json emptyProfile;
    
    EXPECT_CALL(MockDatabase::getInstance(), getUserProfile(userId))
        .WillOnce(testing::Return(emptyProfile));

    // Call method under test
    auto result = AuthService::getUserProfile(userId);

    // Verify results
    EXPECT_TRUE(result.empty());
}

// Test updating user profile
TEST_F(AuthServiceTest, UpdateUserProfile) {
    // Set up expectations
    int userId = 123;
    nlohmann::json profileData = {
        {"username", "updateduser"},
        {"bio", "This is my updated bio"}
    };
    
    EXPECT_CALL(MockDatabase::getInstance(), updateUserProfile(userId, testing::_))
        .WillOnce(testing::Return(true));

    // Call method under test
    bool success = AuthService::updateUserProfile(userId, profileData);

    // Verify results
    EXPECT_TRUE(success);
}

// Test changing password
TEST_F(AuthServiceTest, ChangePassword) {
    // Set up expectations
    int userId = 123;
    std::string oldPassword = "oldpassword";
    std::string newPassword = "newpassword";
    
    EXPECT_CALL(MockDatabase::getInstance(), changePassword(userId, oldPassword, newPassword))
        .WillOnce(testing::Return(true));

    // Call method under test
    bool success = AuthService::changePassword(userId, oldPassword, newPassword);

    // Verify results
    EXPECT_TRUE(success);
}

// Test changing password with incorrect old password
TEST_F(AuthServiceTest, ChangePasswordIncorrectOld) {
    // Set up expectations
    int userId = 123;
    std::string oldPassword = "wrongpassword";
    std::string newPassword = "newpassword";
    
    EXPECT_CALL(MockDatabase::getInstance(), changePassword(userId, oldPassword, newPassword))
        .WillOnce(testing::Return(false));

    // Call method under test
    bool success = AuthService::changePassword(userId, oldPassword, newPassword);

    // Verify results
    EXPECT_FALSE(success);
}

// Test error handling
TEST_F(AuthServiceTest, ErrorHandling) {
    // Login with database error
    std::string email = "test@example.com";
    std::string password = "password123";
    
    EXPECT_CALL(MockDatabase::getInstance(), authenticateUser(email, password, testing::_))
        .WillOnce(testing::Throw(std::runtime_error("Database error")));
    
    nlohmann::json result;
    bool loginSuccess = AuthService::login(email, password, result);
    EXPECT_FALSE(loginSuccess);
    
    // Registration with database error
    std::string username = "newuser";
    
    EXPECT_CALL(MockDatabase::getInstance(), checkUserExists(email))
        .WillOnce(testing::Return(false));
    
    EXPECT_CALL(MockDatabase::getInstance(), registerUser(username, email, password))
        .WillOnce(testing::Throw(std::runtime_error("Database error")));
    
    bool signupSuccess = AuthService::signup(username, email, password);
    EXPECT_FALSE(signupSuccess);
}

// Test with special characters
TEST_F(AuthServiceTest, SpecialCharacters) {
    // Set up expectations for email with special characters
    std::string username = "user.name";
    std::string email = "user+tag@example.com";
    std::string password = "P@ssw0rd!";
    
    EXPECT_CALL(MockDatabase::getInstance(), checkUserExists(email))
        .WillOnce(testing::Return(false));
    
    EXPECT_CALL(MockDatabase::getInstance(), registerUser(username, email, password))
        .WillOnce(testing::Return(true));
    
    // Call method under test
    bool success = AuthService::signup(username, email, password);
    
    // Verify results
    EXPECT_TRUE(success);
}

// Test input validation if implemented
TEST_F(AuthServiceTest, InputValidation) {
    // If the service has validation logic, test it here
    // For example, test with invalid email format
    
    std::string username = "testuser";
    std::string invalidEmail = "not-an-email";
    std::string password = "password123";
    
    // Assuming there's validation before DB calls
    bool success = AuthService::signup(username, invalidEmail, password);
    
    // Depending on implementation, may fail validation or pass to DB
    // Adjust expectation based on actual implementation
}