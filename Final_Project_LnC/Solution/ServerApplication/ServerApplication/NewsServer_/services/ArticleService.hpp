#ifndef ARTICLESERVICE_HPP
#define ARTICLESERVICE_HPP

#include <nlohmann/json.hpp>
#include <string>

class ArticleService {
public:
    static nlohmann::json getAllArticles();
    static nlohmann::json getArticleById(int articleId);
    static nlohmann::json getArticlesByCategory(const std::string& category);

    static void saveArticle(int userId, int articleId);
    static void deleteSavedArticle(int userId, int articleId);
    static nlohmann::json getSavedArticlesForUser(int userId);

    static void likeArticle(int userId, int articleId);
    static void dislikeArticle(int userId, int articleId);
    static nlohmann::json getReactionStats(int articleId);
    static nlohmann::json searchArticles(const std::string& keyword, const std::string& startDate, const std::string& endDate, const std::string& sortBy);

    static void storeArticle(const nlohmann::json& article);  // Used by scheduler/API adapter
    static nlohmann::json getAllCategories();
    static void reportArticle(int userId, int articleId, const std::string& reason);
    static void hideArticle(int articleId);
    static void unhideArticle(int articleId);
    static nlohmann::json getReportedArticles();
};

#endif 
