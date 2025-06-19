#pragma once
#include <vector>
#include "../models/Article.h"

class NewsService {
public:
    static std::vector<Article> getTodayHeadlines(const std::string& category);
    static void printArticlesTable(const std::vector<Article>& articles);
    static std::vector<Article> searchArticles(const std::string& keyword, const std::string& startDate, const std::string& endDate);

};
