#include "SavedArticleService.h"
#include "../utils/HttpClient.h"
#include "../constants/APIEndpoints.h"
#include "../constants/Strings.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

std::vector<Article> SavedArticleService::getSavedArticles(int userId) {
    std::vector<Article> articles;
    try {
        std::string url = API::SAVED_ARTICLES + std::to_string(userId);
        std::string responseStr = HttpClient::get(url);
        auto jsonArr = json::parse(responseStr);

        for (const auto& item : jsonArr) {
            Article a;
            a.id = item.value("id", 0);
            a.title = item.value("title", "");
            a.description = item.value("description", "");
            a.category = item.value("category", Strings::SAVED_ARTICLE_SERVICE_DEFAULT_CATEGORY);
            a.source = item.value("source", "");
            a.url = item.value("url", "");
            a.createdAt = item.value("created_at", "");
            articles.push_back(a);
        }
    } catch (...) {
        std::cerr << Strings::SAVED_ARTICLE_SERVICE_FETCH_FAIL;
    }

    return articles;
}

void SavedArticleService::deleteSavedArticle(int userId, int articleId) {
    std::string endpoint = API::DELETE_SAVED_ARTICLE + "?userId=" +
                           std::to_string(userId) + "&articleId=" + std::to_string(articleId);
    try {
        HttpClient::deleteRequest(endpoint);  
    } catch (...) {
        std::cerr << Strings::SAVED_ARTICLE_SERVICE_DELETE_FAIL;
    }
}
