#include "NewsService.h"
#include "../utils/HttpClient.h"
#include "../constants/APIEndpoints.h"
#include "../constants/Strings.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

std::vector<Article> NewsService::getArticles(const std::string& category,
                                              const std::string& startDate,
                                              const std::string& endDate,
                                              int userId) {
    std::vector<Article> articles;

    std::string endpoint;
    if (!category.empty() && category != "all") {
        endpoint = API::ARTICLES_BY_CATEGORY + category;
    } else {
        endpoint = API::ALL_ARTICLES;
    }

    bool hasParams = endpoint.find('?') != std::string::npos;
    if (!startDate.empty() && !endDate.empty()) {
        endpoint += (hasParams ? "&" : "?");
        endpoint += "start=" + startDate + "&end=" + endDate;
        hasParams = true;
    }
    if (userId != -1) {
        endpoint += (hasParams ? "&" : "?");
        endpoint += "userId=" + std::to_string(userId);
    }

    try {
        std::string responseStr = HttpClient::get(endpoint);
        auto jsonArr = json::parse(responseStr);

        for (const auto& item : jsonArr) {
            Article a;
            a.id = item.value("id", 0);
            a.title = item.value("title", "");
            a.description = item.value("description", "");
            a.category = item.value("category", Strings::NEWS_SERVICE_DEFAULT_CATEGORY);
            a.source = item.value("source", "");
            a.url = item.value("url", "");
            a.createdAt = item.value("created_at", "");
            articles.push_back(a);
        }
    } catch (...) {
        std::cerr << Strings::NEWS_SERVICE_PARSE_LIST_FAIL;
    }

    return articles;
}

std::vector<Article> NewsService::searchArticles(const std::string& keyword,
                                                 const std::string& startDate,
                                                 const std::string& endDate,
                                                 const std::string& sortBy) {
    std::vector<Article> articles;

    std::string url = API::SEARCH_ARTICLES + "?keyword=" + keyword;

    if (!startDate.empty() && !endDate.empty()) {
        url += "&start_date=" + startDate + "&end_date=" + endDate;
    }
    if (!sortBy.empty()) {
        url += "&sort=" + sortBy;
    }

    try {
        std::string responseStr = HttpClient::get(url);
        auto jsonArr = json::parse(responseStr);

        for (const auto& item : jsonArr) {
            Article a;
            a.id = item.value("id", 0);
            a.title = item.value("title", "");
            a.description = item.value("description", "");
            a.category = item.value("category", Strings::NEWS_SERVICE_DEFAULT_CATEGORY);
            a.source = item.value("source", "");
            a.url = item.value("url", "");
            a.createdAt = item.value("created_at", "");
            articles.push_back(a);
        }
    } catch (...) {
        std::cerr << Strings::NEWS_SERVICE_SEARCH_FAIL;
    }

    return articles;
}

std::vector<std::string> NewsService::getAllCategories() {
    std::vector<std::string> categories;
    try {
        std::string responseStr = HttpClient::get(API::ALL_CATEGORIES);
        auto jsonArr = json::parse(responseStr);
        for (const auto& item : jsonArr) {
            categories.push_back(item.get<std::string>());
        }
    } catch (...) {
        std::cerr << "Failed to fetch categories.\n";
    }
    return categories;
}

void NewsService::saveArticle(int userId, int articleId) {
    json body = {
        {"user_id", userId},
        {"article_id", articleId}
    };

    HttpClient::post(API::SAVE_ARTICLE, body.dump());
}

void NewsService::likeArticle(int userId, int articleId) {
    json body = {
        {"user_id", userId}
    };
    HttpClient::post(API::LIKE_ARTICLE + std::to_string(articleId) + "/like", body.dump());
}

void NewsService::dislikeArticle(int userId, int articleId) {
    json body = {
        {"user_id", userId}
    };
    HttpClient::post(API::DISLIKE_ARTICLE + std::to_string(articleId) + "/dislike", body.dump());
}

void NewsService::reportArticle(int userId, int articleId, const std::string& reason) {
    json body = {
        {"user_id", userId},
        {"reason", reason}
    };
    std::string endpoint = API::REPORT_ARTICLE + std::to_string(articleId) + "/report";
    HttpClient::post(endpoint, body.dump());
}
