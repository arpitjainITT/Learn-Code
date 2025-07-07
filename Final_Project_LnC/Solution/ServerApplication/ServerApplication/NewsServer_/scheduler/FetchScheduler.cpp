#include <nlohmann/json.hpp>
#include <chrono>
#include <iostream>
#include "FetchScheduler.hpp"
#include "../adapters/NewsApiOrgAdapter.hpp"
#include "../adapters/TheNewsApiAdapter.hpp"
#include "../utils/HttpClient.hpp"
#include "../services/ArticleService.hpp"
#include "../services/AdminService.hpp"
#include "../utils/Strings.hpp"

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
        std::cout << Strings::FETCH_SCHEDULER_FETCHING;
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
        std::cout << Strings::FETCH_SCHEDULER_TRY_PRIMARY;
        auto articles = primaryAdapter->fetchArticles();
        if (!articles.empty()) {
            std::cout << Strings::FETCH_SCHEDULER_PRIMARY_FETCHED;

            for (const auto& article : articles) {
                std::cout << Strings::FETCH_SCHEDULER_STORING_ARTICLE << article.value("title", "No Title") << Strings::FETCH_SCHEDULER_FROM_SOURCE << article.value("source", "Unknown") << "\n";
                ArticleService::storeArticle(article);
            }

            AdminService::updateExternalServerStatus(primaryAdapter->getSourceName(), "active");
            return;
        } else {
            std::cerr << Strings::FETCH_SCHEDULER_PRIMARY_NO_ARTICLES;
        }
    } catch (const std::exception& ex) {
        std::cerr << Strings::FETCH_SCHEDULER_PRIMARY_FAILED << ex.what() << "\n";
        AdminService::updateExternalServerStatus(primaryAdapter->getSourceName(), "inactive");
    }

    try {
        std::cout << Strings::FETCH_SCHEDULER_TRY_FALLBACK;
        auto articles = fallbackAdapter->fetchArticles();
        if (!articles.empty()) {
            std::cout << Strings::FETCH_SCHEDULER_FALLBACK_FETCHED;
            
            for (const auto& article : articles) {
                std::cout << Strings::FETCH_SCHEDULER_STORING_ARTICLE << article.value("title", "No Title") << Strings::FETCH_SCHEDULER_FROM_SOURCE << article.value("source", "Unknown") << "\n";
                ArticleService::storeArticle(article);
            }

            AdminService::updateExternalServerStatus(fallbackAdapter->getSourceName(), "active");
        } else {
            std::cerr << Strings::FETCH_SCHEDULER_FALLBACK_NO_ARTICLES;
        }
    } catch (const std::exception& ex) {
        std::cerr << Strings::FETCH_SCHEDULER_FALLBACK_FAILED << ex.what() << "\n";
        AdminService::updateExternalServerStatus(fallbackAdapter->getSourceName(), "inactive");
    }
}
