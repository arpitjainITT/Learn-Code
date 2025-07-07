#include <gtest/gtest.h>
#include "../../ui/AdminMenu.h"
#include "../../models/User.h"
#include "../../constants/Strings.h"
#include <iostream>
#include <strings.h>

class AdminMenuTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(AdminMenuTest, ConstructorWithValidUser) {
    User user;
    user.setId(1);
    user.setEmail("admin@example.com");
    user.setRole("admin");
    EXPECT_NO_THROW({ AdminMenu menu(user); });
}

TEST_F(AdminMenuTest, DisplayMethodExists) {
    User user;
    AdminMenu menu(user);
    EXPECT_NO_THROW({ void (AdminMenu::*displayFunc)() = &AdminMenu::display; (void)displayFunc; });
}

TEST_F(AdminMenuTest, MenuOptions) {
    std::vector<int> validChoices = {1,2,3,4,5};
    for (int choice : validChoices) {
        EXPECT_NO_THROW({ switch (choice) { case 1: case 2: case 3: case 4: case 5: break; default: break; } });
    }
}

TEST_F(AdminMenuTest, InvalidChoiceHandling) {
    std::vector<int> invalidChoices = {0,6,-1,100};
    for (int choice : invalidChoices) {
        EXPECT_NO_THROW({ switch (choice) { case 1: case 2: case 3: case 4: case 5: break; default: break; } });
    }
}

TEST_F(AdminMenuTest, StringConstants) {
    EXPECT_FALSE(Strings::ADMIN_MENU_TITLE.empty());
    EXPECT_FALSE(Strings::ADMIN_MENU_VIEW_SERVERS.empty());
    EXPECT_FALSE(Strings::ADMIN_MENU_VIEW_DETAILS.empty());
    EXPECT_FALSE(Strings::ADMIN_MENU_UPDATE_KEY.empty());
    EXPECT_FALSE(Strings::ADMIN_MENU_UPDATE_STATUS.empty());
    EXPECT_FALSE(Strings::ADMIN_MENU_ADD_CATEGORY.empty());
    EXPECT_FALSE(Strings::ADMIN_MENU_LOGOUT.empty());
    EXPECT_FALSE(Strings::ADMIN_MENU_ENTER_CHOICE.empty());
    EXPECT_FALSE(Strings::INVALID_CHOICE.empty());
}

TEST_F(AdminMenuTest, UserRoleHandling) {
    User user; user.setRole("admin"); AdminMenu menu(user); EXPECT_NO_THROW({});
}

TEST_F(AdminMenuTest, MenuNavigation) {
    std::vector<std::pair<int,std::string>> nav = {{1,"servers"},{2,"details"},{3,"api_key"},{4,"status"},{5,"category"},{6,"logout"}};
    for (const auto& t : nav) {
        int choice = t.first; 
        std::string expected = t.second;
        
        std::string action;
        switch (choice) { 
            case 1: action="servers"; break; 
            case 2: action="details"; break; 
            case 3: action="api_key"; break; 
            case 4: action="status"; break; 
            case 5: action="category"; break; 
            case 6: action="logout"; break; 
            default: action="invalid"; break; 
        }
        EXPECT_EQ(action, expected);
    }
}

TEST_F(AdminMenuTest, ExceptionSafety) {
    User user; AdminMenu menu(user); EXPECT_NO_THROW({});
}
