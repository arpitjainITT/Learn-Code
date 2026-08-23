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

static void tryFetchWithAdapter(std::shared_ptr<INewsApiAdapter> adapter) {
    try {
        std::cout << "[FetchScheduler] Trying: " << adapter->getSourceName() << "\n";
        auto articles = adapter->fetchArticles();
        if (articles.empty()) {
            std::cerr << "[FetchScheduler] " << adapter->getSourceName() << " returned no articles.\n";
            AdminService::updateExternalServerStatus(adapter->getSourceName(), "inactive");
            return;
        }
        std::cout << "[FetchScheduler] Fetched " << articles.size()
                  << " articles from " << adapter->getSourceName() << "\n";
        for (const auto& article : articles) {
            ArticleService::storeArticle(article);
        }
        AdminService::updateExternalServerStatus(adapter->getSourceName(), "active");
    } catch (const std::exception& ex) {
        std::cerr << "[FetchScheduler] " << adapter->getSourceName()
                  << " failed: " << ex.what() << "\n";
        AdminService::updateExternalServerStatus(adapter->getSourceName(), "inactive");
    }
}

void FetchScheduler::fetchAndStoreArticles() {
    // Read API keys from the database so admins can rotate them via the Admin Panel.
    auto servers = AdminService::listExternalServers();

    std::string theNewsApiKey;
    std::string newsApiOrgKey;
    bool theNewsApiActive   = false;
    bool newsApiOrgActive   = false;

    for (const auto& server : servers) {
        std::string name   = server.value("server_name", "");
        std::string status = server.value("status", "");
        std::string key    = server.value("api_key", "");

        if (name == Strings::ADAPTER_THENEWSAPI_SOURCE_NAME) {
            theNewsApiKey    = key;
            theNewsApiActive = (status == "active");
        } else if (name == Strings::ADAPTER_NEWSAPIORG_SOURCE_NAME) {
            newsApiOrgKey    = key;
            newsApiOrgActive = (status == "active");
        }
    }

    // Build ordered list: active sources first, then inactive as fallback.
    std::vector<std::shared_ptr<INewsApiAdapter>> adapters;
    if (theNewsApiActive) {
        adapters.push_back(std::make_shared<TheNewsApiAdapter>(theNewsApiKey));
    }
    if (newsApiOrgActive) {
        adapters.push_back(std::make_shared<NewsApiOrgAdapter>(newsApiOrgKey));
    }
    // Add inactive servers as fallback candidates
    if (!theNewsApiActive) {
        adapters.push_back(std::make_shared<TheNewsApiAdapter>(theNewsApiKey));
    }
    if (!newsApiOrgActive) {
        adapters.push_back(std::make_shared<NewsApiOrgAdapter>(newsApiOrgKey));
    }

    for (auto& adapter : adapters) {
        tryFetchWithAdapter(adapter);
    }
}
