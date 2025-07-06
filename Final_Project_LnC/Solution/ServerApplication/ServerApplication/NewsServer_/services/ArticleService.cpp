#include "ArticleService.hpp"
#include "../database/Database.hpp"
#include <iostream>

using json = nlohmann::json;

json ArticleService::getAllArticles() {
    return Database::getAllArticles();
}

json ArticleService::getArticleById(int articleId) {
    return Database::getArticleById(articleId);
}

json ArticleService::getArticlesByCategory(const std::string& category) {
    return Database::getArticlesByCategory(category);
}

void ArticleService::saveArticle(int userId, int articleId) {
    Database::saveArticleForUser(userId, articleId);
}

void ArticleService::deleteSavedArticle(int userId, int articleId) {
    Database::deleteSavedArticle(userId, articleId);
}

json ArticleService::getSavedArticlesForUser(int userId) {
    return Database::getSavedArticlesForUser(userId);
}

void ArticleService::likeArticle(int userId, int articleId) {
    Database::likeArticle(userId, articleId);
}

void ArticleService::dislikeArticle(int userId, int articleId) {
    Database::dislikeArticle(userId, articleId);
}

json ArticleService::searchArticles(const std::string& keyword, const std::string& startDate, const std::string& endDate, const std::string& sortBy) {
    return Database::searchArticles(keyword, startDate, endDate, sortBy);
}


json ArticleService::getReactionStats(int articleId) {
    return Database::getReactionStats(articleId);
}

void ArticleService::storeArticle(const json& article) {
    Database::storeArticle(article);
}
