#include <gtest/gtest.h>
#include "../../constants/Strings.h"
#include <string>
#include <strings.h>

#include <regex>

class StringsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
};

// Test all string constants are properly defined and not empty
TEST_F(StringsTest, AllStringConstantsAreDefined) {
    // App constants
    EXPECT_FALSE(Strings::APP_HOME_WELCOME.empty());
    EXPECT_FALSE(Strings::APP_EXITING.empty());
    EXPECT_FALSE(Strings::APP_INVALID_CHOICE.empty());
    EXPECT_FALSE(Strings::APP_HOME_ENTER_CHOICE.empty());
    EXPECT_FALSE(Strings::APP_LOGIN_EMAIL.empty());
    EXPECT_FALSE(Strings::APP_LOGIN_PASSWORD.empty());
    EXPECT_FALSE(Strings::APP_SIGNUP_USERNAME.empty());
    EXPECT_FALSE(Strings::APP_SIGNUP_EMAIL.empty());
    EXPECT_FALSE(Strings::APP_SIGNUP_PASSWORD.empty());
    EXPECT_FALSE(Strings::APP_LOGIN_FAILED.empty());
    EXPECT_FALSE(Strings::APP_SIGNUP_SUCCESS.empty());
    EXPECT_FALSE(Strings::APP_SIGNUP_FAILED.empty());
}

TEST_F(StringsTest, MenuConstantsAreDefined) {
    // Menu constants
    EXPECT_FALSE(Strings::APP_HOME_SEPARATOR.empty());
    EXPECT_FALSE(Strings::APP_HOME_WELCOME.empty());
    EXPECT_FALSE(Strings::APP_HOME_LOGIN.empty());
    EXPECT_FALSE(Strings::APP_HOME_SIGNUP.empty());
    EXPECT_FALSE(Strings::APP_HOME_EXIT.empty());
    EXPECT_FALSE(Strings::USER_MENU_WELCOME.empty());
    EXPECT_FALSE(Strings::USER_MENU_SEPARATOR.empty());
    EXPECT_FALSE(Strings::USER_MENU_HEADLINES.empty());
    EXPECT_FALSE(Strings::USER_MENU_SAVED_ARTICLES.empty());
    EXPECT_FALSE(Strings::USER_MENU_SEARCH.empty());
    EXPECT_FALSE(Strings::USER_MENU_NOTIFICATIONS.empty());
    EXPECT_FALSE(Strings::USER_MENU_LOGOUT.empty());
}

TEST_F(StringsTest, ServiceConstantsAreDefined) {
    // Service constants
    EXPECT_FALSE(Strings::NEWS_SERVICE_PARSE_LIST_FAIL.empty());
    EXPECT_FALSE(Strings::NEWS_SERVICE_SEARCH_FAIL.empty());
    EXPECT_FALSE(Strings::SAVED_ARTICLE_SERVICE_FETCH_FAIL.empty());
    EXPECT_FALSE(Strings::SAVED_ARTICLE_SERVICE_DELETE_FAIL.empty());
    EXPECT_FALSE(Strings::AUTH_SERVICE_LOGIN_FAIL.empty());
    EXPECT_FALSE(Strings::AUTH_SERVICE_SIGNUP_FAIL.empty());
    EXPECT_FALSE(Strings::ADMIN_SERVICE_FETCH_SERVERS_FAIL.empty());
    EXPECT_FALSE(Strings::ADMIN_SERVICE_FETCH_SERVER_DETAILS_FAIL.empty());
}

TEST_F(StringsTest, HTTPConstantsAreDefined) {
    // HTTP constants
    EXPECT_FALSE(Strings::HTTP_LOG_METHOD_URL.empty());
    EXPECT_FALSE(Strings::HTTP_LOG_BODY.empty());
    EXPECT_FALSE(Strings::HTTP_LOG_AUTH.empty());
    EXPECT_FALSE(Strings::HTTP_ERROR_REQUEST_FAILED.empty());
    EXPECT_FALSE(Strings::HTTP_ERROR_CURL_INIT.empty());
    EXPECT_FALSE(Strings::HTTP_METHOD_GET.empty());
    EXPECT_FALSE(Strings::HTTP_METHOD_POST.empty());
    EXPECT_FALSE(Strings::HTTP_METHOD_PUT.empty());
    EXPECT_FALSE(Strings::HTTP_METHOD_DELETE.empty());
    EXPECT_FALSE(Strings::HTTP_HEADER_CONTENT_TYPE_JSON.empty());
    EXPECT_FALSE(Strings::HTTP_HEADER_AUTH_BEARER.empty());
}

TEST_F(StringsTest, ValidatorConstantsAreDefined) {
    // Validator constants
    EXPECT_FALSE(Strings::VALIDATOR_EMAIL_REGEX.empty());
    EXPECT_GT(Strings::VALIDATOR_PASSWORD_MIN_LENGTH, 0);
}

TEST_F(StringsTest, NotificationConstantsAreDefined) {
    // Notification constants
    EXPECT_FALSE(Strings::NOTIF_SERVICE_FETCH_PREFS_FAIL.empty());
    EXPECT_FALSE(Strings::NOTIF_SERVICE_FETCH_NOTIFS_FAIL.empty());
    EXPECT_FALSE(Strings::NOTIF_SERVICE_MARK_READ_FAIL.empty());
    EXPECT_FALSE(Strings::NOTIF_SERVICE_DEFAULT_CATEGORY.empty());
}

TEST_F(StringsTest, NewsConstantsAreDefined) {
    // News constants
    EXPECT_FALSE(Strings::NEWS_SERVICE_PARSE_LIST_FAIL.empty());
    EXPECT_FALSE(Strings::NEWS_SERVICE_SEARCH_FAIL.empty());
    EXPECT_FALSE(Strings::NEWS_SERVICE_DEFAULT_CATEGORY.empty());
}

TEST_F(StringsTest, SavedArticleConstantsAreDefined) {
    // Saved Article constants
    EXPECT_FALSE(Strings::SAVED_ARTICLES_TITLE.empty());
    EXPECT_FALSE(Strings::VIEW_SAVED_ARTICLES.empty());
    EXPECT_FALSE(Strings::DELETE_ARTICLE.empty());
    EXPECT_FALSE(Strings::BACK.empty());
    EXPECT_FALSE(Strings::ENTER_CHOICE.empty());
    EXPECT_FALSE(Strings::ENTER_ARTICLE_ID_TO_DELETE.empty());
}

TEST_F(StringsTest, AdminConstantsAreDefined) {
    // Admin constants
    EXPECT_FALSE(Strings::ADMIN_MENU_TITLE.empty());
    EXPECT_FALSE(Strings::ADMIN_MENU_VIEW_SERVERS.empty());
    EXPECT_FALSE(Strings::ADMIN_MENU_VIEW_DETAILS.empty());
    EXPECT_FALSE(Strings::ADMIN_MENU_UPDATE_KEY.empty());
    EXPECT_FALSE(Strings::ADMIN_MENU_UPDATE_STATUS.empty());
    EXPECT_FALSE(Strings::ADMIN_MENU_ADD_CATEGORY.empty());
    EXPECT_FALSE(Strings::ADMIN_MENU_LOGOUT.empty());
}

TEST_F(StringsTest, AuthConstantsAreDefined) {
    // Auth constants
    EXPECT_FALSE(Strings::AUTH_SERVICE_LOGIN_FAIL.empty());
    EXPECT_FALSE(Strings::AUTH_SERVICE_SIGNUP_FAIL.empty());
}

TEST_F(StringsTest, PaginatorConstantsAreDefined) {
    // There are currently no paginator-specific constants in Strings.h
    // This test is kept as a placeholder for future paginator constants
    EXPECT_TRUE(true);
}

TEST_F(StringsTest, SpecificStringValues) {
    // Test specific expected values
    EXPECT_EQ(Strings::HTTP_METHOD_GET, "GET");
    EXPECT_EQ(Strings::HTTP_METHOD_POST, "POST");
    EXPECT_EQ(Strings::HTTP_METHOD_PUT, "PUT");
    EXPECT_EQ(Strings::HTTP_METHOD_DELETE, "DELETE");
    EXPECT_EQ(Strings::HTTP_HEADER_CONTENT_TYPE_JSON, "Content-Type: application/json");
    EXPECT_EQ(Strings::HTTP_HEADER_AUTH_BEARER, "Authorization: Bearer ");
    EXPECT_EQ(Strings::NOTIF_SERVICE_DEFAULT_CATEGORY, "General");
    EXPECT_EQ(Strings::NEWS_SERVICE_DEFAULT_CATEGORY, "General");
    EXPECT_GE(Strings::VALIDATOR_PASSWORD_MIN_LENGTH, 6);
}

TEST_F(StringsTest, EmailRegexIsValid) {
    // Test that email regex is a valid regex pattern
    std::regex emailRegex(Strings::VALIDATOR_EMAIL_REGEX);
    EXPECT_TRUE(std::regex_match("test@example.com", emailRegex));
    EXPECT_TRUE(std::regex_match("user.name@domain.co.uk", emailRegex));
    EXPECT_FALSE(std::regex_match("invalid-email", emailRegex));
    EXPECT_FALSE(std::regex_match("@domain.com", emailRegex));
    EXPECT_FALSE(std::regex_match("user@", emailRegex));
}
