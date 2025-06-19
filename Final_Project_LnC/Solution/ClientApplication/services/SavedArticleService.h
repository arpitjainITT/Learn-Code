#pragma once
#include <string>
#include <vector>
#include "../models/User.h"
#include "../models/Article.h"

class SavedArticleService {
public:
    static void saveArticle(const User& user, const std::string& articleId);
    static std::vector<Article> getSavedArticles(const User& user);
    static void deleteArticle(const User& user, const std::string& articleId);
};
