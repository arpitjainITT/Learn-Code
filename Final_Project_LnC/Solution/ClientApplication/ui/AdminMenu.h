#ifndef ADMINMENU_H
#define ADMINMENU_H

#include "../models/User.h"

class AdminMenu {
public:
    explicit AdminMenu(const User& user);
    void display();

private:
    User currentUser;

    void listExternalServers();
    void viewServerDetails();
    void updateServerKey();
    void updateServerStatus();
    void addCategory();
    void reviewReportedArticles();
    void hideUnhideArticle();
    void hideUnhideCategory();
    void manageFilteredKeywords();
    void showHiddenCategories();
    void showFilteredKeywords();
};

#endif 
