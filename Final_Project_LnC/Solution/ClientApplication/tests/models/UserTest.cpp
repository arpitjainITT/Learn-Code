#include <gtest/gtest.h>
#include "../../models/User.h"
#include <string>
#include <strings.h>


class UserTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
};

TEST_F(UserTest, DefaultConstructor) {
    User user;
    
    // Test default values
    EXPECT_EQ(user.getId(), -1);
    EXPECT_EQ(user.getRole(), "user");
    EXPECT_EQ(user.getEmail(), "");
}

TEST_F(UserTest, SetAndGetId) {
    User user;
    
    // Test setting and getting ID
    user.setId(123);
    EXPECT_EQ(user.getId(), 123);
    
    user.setId(0);
    EXPECT_EQ(user.getId(), 0);
    
    user.setId(-5);
    EXPECT_EQ(user.getId(), -5);
}

TEST_F(UserTest, SetAndGetRole) {
    User user;
    
    // Test setting and getting role
    user.setRole("admin");
    EXPECT_EQ(user.getRole(), "admin");
    
    user.setRole("user");
    EXPECT_EQ(user.getRole(), "user");
    
    user.setRole("moderator");
    EXPECT_EQ(user.getRole(), "moderator");
    
    // Test empty role
    user.setRole("");
    EXPECT_EQ(user.getRole(), "");
}

TEST_F(UserTest, SetAndGetEmail) {
    User user;
    
    // Test setting and getting email
    user.setEmail("test@example.com");
    EXPECT_EQ(user.getEmail(), "test@example.com");
    
    user.setEmail("user.name@domain.co.uk");
    EXPECT_EQ(user.getEmail(), "user.name@domain.co.uk");
    
    // Test empty email
    user.setEmail("");
    EXPECT_EQ(user.getEmail(), "");
}

TEST_F(UserTest, MultipleUsersIndependent) {
    User user1;
    User user2;
    
    // Set different values for each user
    user1.setId(1);
    user1.setRole("admin");
    user1.setEmail("admin@example.com");
    
    user2.setId(2);
    user2.setRole("user");
    user2.setEmail("user@example.com");
    
    // Verify they are independent
    EXPECT_EQ(user1.getId(), 1);
    EXPECT_EQ(user1.getRole(), "admin");
    EXPECT_EQ(user1.getEmail(), "admin@example.com");
    
    EXPECT_EQ(user2.getId(), 2);
    EXPECT_EQ(user2.getRole(), "user");
    EXPECT_EQ(user2.getEmail(), "user@example.com");
}

TEST_F(UserTest, UpdateValues) {
    User user;
    
    // Set initial values
    user.setId(100);
    user.setRole("user");
    user.setEmail("initial@example.com");
    
    // Verify initial values
    EXPECT_EQ(user.getId(), 100);
    EXPECT_EQ(user.getRole(), "user");
    EXPECT_EQ(user.getEmail(), "initial@example.com");
    
    // Update values
    user.setId(200);
    user.setRole("admin");
    user.setEmail("updated@example.com");
    
    // Verify updated values
    EXPECT_EQ(user.getId(), 200);
    EXPECT_EQ(user.getRole(), "admin");
    EXPECT_EQ(user.getEmail(), "updated@example.com");
}

TEST_F(UserTest, ConstMethods) {
    const User user;
    
    // Test that const methods work correctly
    EXPECT_EQ(user.getId(), -1);
    EXPECT_EQ(user.getRole(), "user");
    EXPECT_EQ(user.getEmail(), "");
}

TEST_F(UserTest, EdgeCases) {
    User user;
    
    // Test with very large ID
    user.setId(2147483647); // Max int
    EXPECT_EQ(user.getId(), 2147483647);
    
    // Test with negative ID
    user.setId(-2147483648); // Min int
    EXPECT_EQ(user.getId(), -2147483648);
    
    // Test with very long email
    std::string longEmail = "very.long.email.address.with.many.parts@very.long.domain.name.com";
    user.setEmail(longEmail);
    EXPECT_EQ(user.getEmail(), longEmail);
    
    // Test with very long role
    std::string longRole = "very_long_role_name_with_many_characters";
    user.setRole(longRole);
    EXPECT_EQ(user.getRole(), longRole);
}
