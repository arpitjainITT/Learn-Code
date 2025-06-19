#include "NewsService.h"
#include "../utils/HttpClient.h"
#include "../utils/Paginator.h"
#include "../constants/APIEndpoints.h"
#include <iostream>

std::vector<Article> NewsService::getTodayHeadlines(const std::string& category) {
    std::string url = std::string(BASE_URL) + ARTICLES_ALL;
    if (category != "all") url = std::string(BASE_URL) + ARTICLES_BY_CATEGORY + "/" + category;

    std::string response = HttpClient::get(url);
    
    // NOTE: This is dummy data for now
    std::vector<Article> articles;
    articles.push_back({ "123", "Tesla hits record high", "benzinga.com", "https://news.link", "Business" });
    articles.push_back({ "124", "Virat Kohli scores century", "cricbuzz.com", "https://sports.link", "Sports" });

    return articles;
}

void NewsService::printArticlesTable(const std::vector<Article>& articles) {
    Paginator<Article> pager(articles);
    pager.display([](const Article& article) {
        std::cout << article.id << "\t" << article.title << "\t"
                  << article.source << "\t" << article.category << "\n";
    });
}

std::vector<Article> NewsService::searchArticles(const std::string& keyword,
    const std::string& startDate,
    const std::string& endDate) {
std::string url = "/api/news/search?keyword=" + keyword + "&start=" + startDate + "&end=" + endDate;
std::string response = HttpClient::get(url);

return {{"999", "Search Result on Tesla", "Yahoo", "https://...", "business"}};
}

