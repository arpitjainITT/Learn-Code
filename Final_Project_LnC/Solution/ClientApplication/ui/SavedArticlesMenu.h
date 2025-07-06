#ifndef SAVEDARTICLESMENU_H
#define SAVEDARTICLESMENU_H

#include "../models/User.h"

class SavedArticlesMenu {
public:
    explicit SavedArticlesMenu(const User& user);
    void display();

private:
    User currentUser;

    void listSavedArticles();
    void deleteArticle(int articleId);
};

#endif 