#include "NewsFetcherManager.hpp"
#include "../services/ArticleService.hpp"
#include <iostream>
#include "../utils/Strings.hpp"

void NewsFetcherManager::registerAdapter(std::shared_ptr<INewsApiAdapter> adapter) {
    adapters.push_back(adapter);
}

void NewsFetcherManager::fetchAll() {
    for (const auto& adapter : adapters) {
        try {
            std::cout << Strings::NEWS_FETCHER_FETCHING << adapter->getSourceName() << "\n";
            auto articles = adapter->fetchArticles();
            for (const auto& article : articles) {
                ArticleService::storeArticle(article);
            }
        } catch (const std::exception& ex) {
            std::cerr << Strings::NEWS_FETCHER_ERROR << adapter->getSourceName() << ": " << ex.what() << "\n";
        }
    }
}
