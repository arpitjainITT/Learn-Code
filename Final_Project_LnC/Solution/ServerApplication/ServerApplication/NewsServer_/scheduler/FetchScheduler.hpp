#ifndef FETCH_SCHEDULER_HPP
#define FETCH_SCHEDULER_HPP

#include <thread>
#include <atomic>
#include <condition_variable>

class FetchScheduler {
public:
    FetchScheduler();
    ~FetchScheduler();

    void start();
    void stop();

private:
    std::thread schedulerThread;
    std::atomic<bool> running;

    std::mutex mtx;
    std::condition_variable cv;

    void run();
    void fetchAndStoreArticles(); 
};

#endif
