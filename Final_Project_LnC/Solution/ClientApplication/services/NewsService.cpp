#include "NewsService.h"
#include "../utils/HttpClient.h"
#include "../utils/Paginator.h"
#include "../constants/APIEndpoints.h"
#include "../utils/nlohmann/json.hpp"
#include <iostream>

std::vector<Article> NewsService::getTodayHeadlines(const std::string& category) {
    std::string url = std::string(BASE_URL) + ARTICLES_ALL;
    if (category != "all")
        url = std::string(BASE_URL) + ARTICLES_BY_CATEGORY + "/" + category;

    std::string response = HttpClient::get(url);
    return parseArticlesFromJson(response);
}

void NewsService::printArticlesTable(const std::vector<Article>& articles) {
    Paginator<Article> pager(articles);
    pager.display([](const Article& article) {
        std::cout << article.id << "\t" << article.title << "\t"
                  << (article.content.length() > 24 ? article.content.substr(0, 24) + "..." : article.content) << "\t"
                  << article.source << "\t" << article.category << "\t"
                  << article.publishedTime << "\n";
    });
}

std::vector<Article> NewsService::searchArticles(const std::string& keyword,
                                                 const std::string& startDate,
                                                 const std::string& endDate) {
    std::string url = std::string(BASE_URL) + "/api/news/search?keyword=" + keyword +
                      "&start=" + startDate + "&end=" + endDate;
    std::string response = HttpClient::get(url);
    return parseArticlesFromJson(response);
}

std::vector<Article> NewsService::parseArticlesFromJson(const std::string& response) {
    std::vector<Article> articles;
    try {
        auto parsed = nlohmann::json::parse(response);
        auto jsonArr = parsed.is_array() ? parsed : parsed["articles"];

        for (const auto& item : jsonArr) {
            std::string id = std::to_string(item.value("id", 0));
            std::string title = item.value("title", "");
            std::string content = item.value("content", "");
            std::string source = item.value("source", "");
            std::string url = item.value("url", "");
            std::string category = item.value("category", "");
            std::string publishedTime = item.value("published_at", "");

            articles.push_back({id, title, content, source, url, category, publishedTime});
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse articles JSON: " << e.what() << "\n";
    }

    return articles;
}
