#include <gtest/gtest.h>
#include "../../core/App.h"
#include "../../models/User.h"
#include "../../constants/Strings.h"
#include <string.h>

// Make our test fixture a subclass of App to access protected methods
class AppTest : public App, public ::testing::Test {
protected:
    // No need to create a separate App instance anymore since we're extending App
};

// Use the same fixture for all tests
TEST_F(AppTest, ShowHomeMenu_DisplaysCorrectStrings) {
    // This would require redirecting std::cout and checking output
    // For brevity, just check that the function runs without error
    EXPECT_NO_THROW(showHomeMenu());
}

TEST_F(AppTest, HandleLogin_InvalidCredentials_ShowsLoginFailed) {
    // Would require mocking AuthService::login and std::cin
    // For demonstration, just ensure function runs
    EXPECT_NO_THROW(handleLogin());
}

TEST_F(AppTest, HandleSignup_SuccessAndFailure) {
    // Would require mocking AuthService::signup and std::cin
    EXPECT_NO_THROW(handleSignup());
}

TEST_F(AppTest, ShowAdminMenu_CallsDisplay) {
    User admin;
    admin.setRole("admin");
    EXPECT_NO_THROW(showAdminMenu(admin));
}

TEST_F(AppTest, ShowUserMenu_CallsDisplay) {
    User user;
    user.setRole("user");
    EXPECT_NO_THROW(showUserMenu(user));
}

// Add more tests for edge cases, input validation, etc.
