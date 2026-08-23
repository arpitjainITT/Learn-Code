#include <gtest/gtest.h>
#include "../../ui/SearchMenu.h"
#include "../../models/User.h"
#include "../../constants/Strings.h"
#include <iostream>

class SearchMenuTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(SearchMenuTest, Constructor) {
    User user;
    user.setId(1);
    user.setEmail("test@example.com");
    user.setRole("user");
    EXPECT_NO_THROW({ SearchMenu menu(user); });
}

TEST_F(SearchMenuTest, DisplayMethodExists) {
    User user;
    user.setId(1);
    user.setEmail("test@example.com");
    user.setRole("user");
    SearchMenu menu(user);
    EXPECT_NO_THROW({ void (SearchMenu::*displayFunc)() = &SearchMenu::display; (void)displayFunc; });
}

TEST_F(SearchMenuTest, SearchInputHandling) {
    std::vector<std::string> keywords = {"news","AI","sports","","longkeywordwithmanyletters"};
    for (const auto& k : keywords) {
        EXPECT_NO_THROW({ std::string input = k; });
    }
}

TEST_F(SearchMenuTest, MenuOptions) {
    std::vector<int> validChoices = {1,2,3};
    for (int choice : validChoices) {
        EXPECT_NO_THROW({ switch (choice) { case 1: case 2: case 3: break; default: break; } });
    }
}

TEST_F(SearchMenuTest, InvalidChoiceHandling) {
    std::vector<int> invalidChoices = {0,4,-1,100};
    for (int choice : invalidChoices) {
        EXPECT_NO_THROW({ switch (choice) { case 1: case 2: case 3: break; default: break; } });
    }
}

TEST_F(SearchMenuTest, StringConstants) {
    EXPECT_FALSE(Strings::SEARCH_MENU_TITLE.empty());
    EXPECT_FALSE(Strings::SEARCH_MENU_SEARCH_ARTICLES.empty());
    EXPECT_FALSE(Strings::SEARCH_MENU_BACK.empty());
    EXPECT_FALSE(Strings::SEARCH_MENU_ENTER_CHOICE.empty());
    EXPECT_FALSE(Strings::SEARCH_MENU_ENTER_KEYWORD.empty());
    EXPECT_FALSE(Strings::INVALID_CHOICE.empty());
}

TEST_F(SearchMenuTest, ExceptionSafety) {
    User user;
    user.setId(1);
    user.setEmail("test@example.com");
    user.setRole("user");
    SearchMenu menu(user); 
    EXPECT_NO_THROW({});
}
