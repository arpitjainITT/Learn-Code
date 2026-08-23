/**
 * UserServiceTest.cpp
 *
 * Tests UserService (login, signup) through the real Database layer
 * using an in-memory SQLite database.
 *
 * UserService is a thin wrapper around Database::authenticateUser() and
 * Database::registerUser(), both of which hash passwords via PasswordHasher.
 */
#include <gtest/gtest.h>
#include "../../services/UserService.hpp"
#include "../../database/DBManager.hpp"
#include "../../database/DataBase.hpp"
#include <string>

class UserServiceTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        ASSERT_TRUE(DBManager::getInstance().initializeDB(":memory:"))
            << "Failed to open in-memory SQLite database for UserServiceTest";
        // Create the default admin that the server normally creates on startup
        Database::createDefaultAdmin();
    }
    static void TearDownTestSuite() {
        DBManager::getInstance().close();
    }
};

// ─────────────────────────────────────────────────────────
// signup (registerUser)
// ─────────────────────────────────────────────────────────

TEST_F(UserServiceTest, Signup_NewUser_Succeeds) {
    bool ok = UserService::signup("Alice", "alice@test.com", "secret1");
    EXPECT_TRUE(ok) << "Registering a new unique user must succeed";
}

TEST_F(UserServiceTest, Signup_DuplicateEmail_Fails) {
    UserService::signup("Bob", "bob@test.com", "secret2");
    bool ok = UserService::signup("Bob2", "bob@test.com", "secret3");  // same email
    EXPECT_FALSE(ok) << "Registering with a duplicate email must fail";
}

TEST_F(UserServiceTest, Signup_DuplicateUsername_Fails) {
    UserService::signup("Charlie", "charlie@test.com", "secret4");
    bool ok = UserService::signup("Charlie", "charlie2@test.com", "secret5");  // same username
    EXPECT_FALSE(ok) << "Registering with a duplicate username must fail";
}

// ─────────────────────────────────────────────────────────
// login (authenticateUser)
// ─────────────────────────────────────────────────────────

TEST_F(UserServiceTest, Login_CorrectCredentials_Succeeds) {
    UserService::signup("Diana", "diana@test.com", "dpassword");

    int userId = -1;
    std::string role;
    bool ok = UserService::login("diana@test.com", "dpassword", userId, role);

    EXPECT_TRUE(ok)       << "Login with correct credentials must succeed";
    EXPECT_GT(userId, 0)  << "Returned userId must be a positive integer";
    EXPECT_EQ(role, "user") << "Newly signed-up user must have 'user' role";
}

TEST_F(UserServiceTest, Login_WrongPassword_Fails) {
    UserService::signup("Eve", "eve@test.com", "correct_pw");

    int userId = -1;
    std::string role;
    bool ok = UserService::login("eve@test.com", "wrong_pw", userId, role);

    EXPECT_FALSE(ok) << "Login with wrong password must fail";
}

TEST_F(UserServiceTest, Login_NonExistentEmail_Fails) {
    int userId = -1;
    std::string role;
    bool ok = UserService::login("nobody@test.com", "anypassword", userId, role);
    EXPECT_FALSE(ok) << "Login with an email that was never registered must fail";
}

TEST_F(UserServiceTest, Login_EmptyCredentials_Fails) {
    int userId = -1;
    std::string role;
    bool ok = UserService::login("", "", userId, role);
    EXPECT_FALSE(ok) << "Login with empty email and password must fail";
}

TEST_F(UserServiceTest, Login_DefaultAdmin_Succeeds) {
    // The default admin (headmaster@news.com / headmaster1223) is created in SetUpTestSuite
    int userId = -1;
    std::string role;
    bool ok = UserService::login("headmaster@news.com", "headmaster1223", userId, role);

    EXPECT_TRUE(ok)          << "Default admin login must succeed";
    EXPECT_GT(userId, 0)     << "Admin userId must be positive";
    EXPECT_EQ(role, "admin") << "Default admin must have 'admin' role";
}

TEST_F(UserServiceTest, Signup_ThenLogin_PasswordHashing_Works) {
    // Verifies end-to-end: signup stores a hash, login hashes the same input and compares
    const std::string email    = "frank@test.com";
    const std::string password = "fr@nkP@ss!";

    bool signupOk = UserService::signup("Frank", email, password);
    ASSERT_TRUE(signupOk);

    int userId = -1;
    std::string role;
    bool loginOk = UserService::login(email, password, userId, role);
    EXPECT_TRUE(loginOk) << "Login must succeed with the same password used during signup";
}
