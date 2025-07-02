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
    static void deleteSavedArticle(int userId, int articleId);

    static void likeArticle(int userId, int articleId);
    static void dislikeArticle(int userId, int articleId);
    static nlohmann::json getReactionStats(int articleId);

    static void addCategory(const std::string& category);
    static int getCategoryId(const std::string& category);

    static void setCategoryNotificationPreference(int userId, int categoryId, bool isEnabled);
    static void setKeywordNotificationPreference(int userId, const std::string& keyword, bool isEnabled);
    static nlohmann::json getUserNotificationPreferences(int userId);

    static nlohmann::json getAllExternalServers();
    static nlohmann::json getExternalServerById(int serverId);
    static void updateExternalServerApiKey(int serverId, const std::string& newApiKey);
    static void updateExternalServerStatus(int serverId, const std::string& status);
    static void updateExternalServerStatusByName(const std::string& serverName, const std::string& status);

    static int getUserIdByEmail(const std::string& email);
    static bool authenticateUser(const std::string& email, const std::string& password, int& userId, std::string& role);
    static bool registerUser(const std::string& username, const std::string& email, const std::string& password);
    static void createDefaultAdmin();

    static void storeArticle(const nlohmann::json& article);

};

#endif 
