#include "NewsFetcherManager.hpp"
#include "../services/ArticleService.hpp"
#include <iostream>

void NewsFetcherManager::registerAdapter(std::shared_ptr<INewsApiAdapter> adapter) {
    adapters.push_back(adapter);
}

void NewsFetcherManager::fetchAll() {
    for (const auto& adapter : adapters) {
        try {
            std::cout << "[NewsFetcherManager] Fetching from: " << adapter->getSourceName() << "\n";
            auto articles = adapter->fetchArticles();
            for (const auto& article : articles) {
                ArticleService::storeArticle(article);
            }
        } catch (const std::exception& ex) {
            std::cerr << "[NewsFetcherManager] Error fetching from " << adapter->getSourceName() << ": " << ex.what() << "\n";
        }
    }
}
