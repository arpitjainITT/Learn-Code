#include "FetchScheduler.hpp"
#include "../services/ArticleService.hpp"
#include <thread>
#include <chrono>

void FetchScheduler::start(int intervalSeconds) {
    std::thread([=]() {
        while (true) {
            ArticleService::fetchAndStore();
            std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
        }
    }).detach();
}
