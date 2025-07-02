#include <nlohmann/json.hpp>
#include <chrono>
#include <iostream>
#include "FetchScheduler.hpp"
#include "../managers/NewsFetcherManager.hpp"
#include "../adapters/NewsApiOrgAdapter.hpp"
#include "../adapters/TheNewsApiAdapter.hpp"
#include "../utils/HttpClient.hpp"
#include "../services/ArticleService.hpp"
#include "../services/AdminService.hpp"

using namespace std::chrono_literals;
using json = nlohmann::json;

FetchScheduler::FetchScheduler() : running(false) {}

FetchScheduler::~FetchScheduler() {
    stop();
}

void FetchScheduler::start() {
    running = true;
    schedulerThread = std::thread(&FetchScheduler::run, this);
}

void FetchScheduler::stop() {
    if (!running) return; // Already stopped
    
    running = false;
    cv.notify_one();  // Wake up thread if it's sleeping
    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }
}

void FetchScheduler::run() {
    while (running) {
        std::cout << "[FetchScheduler] Fetching articles...\n";
        fetchAndStoreArticles();
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait_for(lock, 180min, [this]() { return !running; }); // configurable interval that wait for 3 hours or close if interrupted
    }
}

auto safe_get = [](const json& j, const std::string& key) -> std::string {
    return (j.contains(key) && j[key].is_string()) ? j[key].get<std::string>() : "";
};

void FetchScheduler::fetchAndStoreArticles() {
    std::shared_ptr<INewsApiAdapter> primaryAdapter = std::make_shared<NewsApiOrgAdapter>();
    std::shared_ptr<INewsApiAdapter> fallbackAdapter = std::make_shared<TheNewsApiAdapter>();

    try {
        std::cout << "[FetchScheduler] Trying primary source: NewsAPI.org...\n";
        auto articles = primaryAdapter->fetchArticles();
        if (!articles.empty()) {
            std::cout << "[FetchScheduler] Articles fetched from NewsAPI.org\n";

            for (const auto& article : articles) {
                std::cout << "[FetchScheduler] Storing article: " << article.value("title", "No Title") << " from source: " << article.value("source", "Unknown") << "\n";
                ArticleService::storeArticle(article);
            }

            AdminService::updateExternalServerStatus(primaryAdapter->getSourceName(), "active");
            return;
        } else {
            std::cerr << "[FetchScheduler] NewsAPI.org returned no articles.\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "[FetchScheduler] NewsAPI.org failed: " << ex.what() << "\n";
        AdminService::updateExternalServerStatus(primaryAdapter->getSourceName(), "inactive");
    }

    try {
        std::cout << "[FetchScheduler] Trying fallback source: TheNewsAPI.com...\n";
        auto articles = fallbackAdapter->fetchArticles();
        if (!articles.empty()) {
            std::cout << "[FetchScheduler] Articles fetched from TheNewsAPI.com\n";
            
            for (const auto& article : articles) {
                std::cout << "[FetchScheduler] Storing article: " << article.value("title", "No Title") << " from source: " << article.value("source", "Unknown") << "\n";
                ArticleService::storeArticle(article);
            }

            AdminService::updateExternalServerStatus(fallbackAdapter->getSourceName(), "active");
        } else {
            std::cerr << "[FetchScheduler] TheNewsAPI.com also returned no articles.\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "[FetchScheduler] TheNewsAPI.com failed: " << ex.what() << "\n";
        AdminService::updateExternalServerStatus(fallbackAdapter->getSourceName(), "inactive");
    }
}
