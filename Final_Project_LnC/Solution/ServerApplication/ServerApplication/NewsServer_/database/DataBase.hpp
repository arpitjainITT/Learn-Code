#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <nlohmann/json.hpp>

class Database {
public:
    static nlohmann::json getAllArticles();
    static nlohmann::json getArticlesByCategory(const std::string& category);
    static nlohmann::json getArticleById(int id);

    static void saveArticleForUser(int userId, int articleId);
    static nlohmann::json getSavedArticlesForUser(int userId);
    static void deleteSavedArticle(int savedId);

    static void storeArticle(const nlohmann::json& article);

};

#endif 
