#ifndef HEADLINESMENU_H
#define HEADLINESMENU_H

#include "../models/User.h"

class HeadlinesMenu {
public:
    explicit HeadlinesMenu(const User& user);
    void display();

private:
    User currentUser;

    void showTodayMenu();
    void showDateRangeMenu();
    void fetchAndDisplayArticles(const std::string& category = "", const std::string& startDate = "", const std::string& endDate = "");
    void showArticleOptions(int articleId);
    std::string selectCategoryMenu();
};

#endif 
