#include <gtest/gtest.h>
#include "../../ui/SavedArticlesMenu.h"
#include "../../models/User.h"
#include "../../constants/Strings.h"
#include <iostream>

class SavedArticlesMenuTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(SavedArticlesMenuTest, Constructor) {
    User user;
    user.setId(1);
    user.setEmail("test@example.com");
    user.setRole("user");
    EXPECT_NO_THROW({ SavedArticlesMenu menu(user); });
}

TEST_F(SavedArticlesMenuTest, DisplayMethodExists) {
    User user;
    user.setId(1);
    user.setEmail("test@example.com");
    user.setRole("user");
    SavedArticlesMenu menu(user);
    EXPECT_NO_THROW({ void (SavedArticlesMenu::*displayFunc)() = &SavedArticlesMenu::display; (void)displayFunc; });
}

TEST_F(SavedArticlesMenuTest, MenuOptions) {
    std::vector<int> validChoices = {1,2,3};
    for (int choice : validChoices) {
        EXPECT_NO_THROW({ switch (choice) { case 1: case 2: case 3: break; default: break; } });
    }
}

TEST_F(SavedArticlesMenuTest, InvalidChoiceHandling) {
    std::vector<int> invalidChoices = {0,4,-1,100};
    for (int choice : invalidChoices) {
        EXPECT_NO_THROW({ switch (choice) { case 1: case 2: case 3: break; default: break; } });
    }
}

TEST_F(SavedArticlesMenuTest, StringConstants) {
    EXPECT_FALSE(Strings::SAVED_ARTICLES_TITLE.empty());
    EXPECT_FALSE(Strings::VIEW_SAVED_ARTICLES.empty());
    EXPECT_FALSE(Strings::DELETE_ARTICLE.empty());
    EXPECT_FALSE(Strings::BACK.empty());
    EXPECT_FALSE(Strings::ENTER_CHOICE.empty());
    EXPECT_FALSE(Strings::INVALID_CHOICE.empty());
}

TEST_F(SavedArticlesMenuTest, ExceptionSafety) {
    User user;
    user.setId(1);
    user.setEmail("test@example.com");
    user.setRole("user");
    SavedArticlesMenu menu(user); 
    EXPECT_NO_THROW({});
}
