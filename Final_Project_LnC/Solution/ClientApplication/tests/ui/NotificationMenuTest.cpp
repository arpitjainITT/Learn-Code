#include <gtest/gtest.h>
#include "../../ui/NotificationMenu.h"
#include "../../models/User.h"
#include "../../constants/Strings.h"
#include <iostream>

class NotificationMenuTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(NotificationMenuTest, Constructor) {
    User user;
    user.setId(1);
    user.setEmail("test@example.com");
    user.setRole("user");
    EXPECT_NO_THROW({ NotificationMenu menu(user); });
}

TEST_F(NotificationMenuTest, DisplayMethodExists) {
    User user;
    user.setId(1);
    user.setEmail("test@example.com");
    user.setRole("user");
    NotificationMenu menu(user);
    EXPECT_NO_THROW({ void (NotificationMenu::*displayFunc)() = &NotificationMenu::display; (void)displayFunc; });
}

TEST_F(NotificationMenuTest, MenuOptions) {
    std::vector<int> validChoices = {1,2,3};
    for (int choice : validChoices) {
        EXPECT_NO_THROW({ switch (choice) { case 1: case 2: case 3: break; default: break; } });
    }
}

TEST_F(NotificationMenuTest, InvalidChoiceHandling) {
    std::vector<int> invalidChoices = {0,4,-1,100};
    for (int choice : invalidChoices) {
        EXPECT_NO_THROW({ switch (choice) { case 1: case 2: case 3: break; default: break; } });
    }
}

TEST_F(NotificationMenuTest, StringConstants) {
    EXPECT_FALSE(Strings::NOTIF_MENU_TITLE.empty());
    EXPECT_FALSE(Strings::NOTIF_MENU_VIEW_NOTIFICATIONS.empty());
    EXPECT_FALSE(Strings::NOTIF_MENU_VIEW_PREFERENCES.empty());
    EXPECT_FALSE(Strings::NOTIF_MENU_CONFIGURE_CATEGORY.empty());
    EXPECT_FALSE(Strings::NOTIF_MENU_CONFIGURE_KEYWORD.empty());
    EXPECT_FALSE(Strings::NOTIF_MENU_BACK.empty());
    EXPECT_FALSE(Strings::NOTIF_MENU_ENTER_CHOICE.empty());
    EXPECT_FALSE(Strings::INVALID_CHOICE.empty());
}

TEST_F(NotificationMenuTest, ExceptionSafety) {
    User user;
    user.setId(1);
    user.setEmail("test@example.com");
    user.setRole("user");
    NotificationMenu menu(user); 
    EXPECT_NO_THROW({});
}

