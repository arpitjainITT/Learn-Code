#include "SavedArticleService.h"
#include "../utils/HttpClient.h"
#include "../constants/APIEndpoints.h"
#include <iostream>

void SavedArticleService::saveArticle(const User& user, const std::string& articleId) {
    std::string body = "{\"email\": \"" + user.getEmail() + "\", \"articleId\": \"" + articleId + "\"}";
    HttpClient::post(std::string(BASE_URL) + ARTICLE_SAVE, body);
    std::cout << "Article saved.\n";
}

std::vector<Article> SavedArticleService::getSavedArticles(const User& user) {
    std::string response = HttpClient::get(std::string(BASE_URL) + ARTICLE_SAVED);;
    // Return dummy articles
    return {{"123", "Saved Tesla News", "benzinga", "https://...", "business"}};
}

void SavedArticleService::deleteArticle(const User& user, const std::string& articleId) {
    std::string url = std::string(BASE_URL) + ARTICLE_DELETE_SAVED + "/" + articleId;
    HttpClient::deleteRequest(url);
    std::cout << "Article deleted.\n";
}
