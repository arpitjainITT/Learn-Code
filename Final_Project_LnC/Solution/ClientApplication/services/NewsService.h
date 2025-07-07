#ifndef NEWSSERVICE_H
#define NEWSSERVICE_H

#include "../models/Article.h"
#include <string>
#include <vector>

class NewsService {
public:
    static std::vector<Article> getArticles(const std::string& category = "", const std::string& startDate = "", const std::string& endDate = "");
    static std::vector<Article> searchArticles(const std::string& keyword, const std::string& startDate, const std::string& endDate, const std::string& sortBy = "");
    static std::vector<std::string> getAllCategories();

    static void saveArticle(int userId, int articleId);
    static void likeArticle(int userId, int articleId);
    static void dislikeArticle(int userId, int articleId);
    static void reportArticle(int userId, int articleId, const std::string& reason);
};

#endif 
