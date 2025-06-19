#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <pistache/net.h>
#include <iostream>
#include <csignal>
#include "routes/RouteManager.hpp"
#include "database/DBManager.hpp"
#include "scheduler/FetchScheduler.hpp"

using namespace Pistache;

class ServerApp {
public:
    explicit ServerApp(Pistache::Address addr)
        : httpEndpoint(std::make_shared<Http::Endpoint>(addr)) {}

    void init(size_t threads = 2) {
        auto opts = Http::Endpoint::options()
                        .threads(static_cast<int>(threads));
        httpEndpoint->init(opts);
        setupRoutes();
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serveThreaded();
    }

    void shutdown() {
        httpEndpoint->shutdown();
    }

private:
    void setupRoutes() {
        RouteManager::init(router);
    }

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};

std::shared_ptr<ServerApp> globalApp;

void handleSignal(int signal) {
    if (globalApp) {
        std::cout << "\nShutting down server peacefully and gracefully on your order....." << std::endl;
        globalApp->shutdown();
    }
    exit(0);
}

int main() {
    DBManager::init("../../../../data/news2.db");
    FetchScheduler::start(3 * 60 * 60); 
    Pistache::Port port(9080);
    Pistache::Address addr(Pistache::Ipv4::any(), port);

    std::cout << "Starting server at http://localhost:" << port << " ..." << std::endl;

    globalApp = std::make_shared<ServerApp>(addr);
    globalApp->init(4); // using 4 threads

    // Register signal handler for Ctrl+C and termination signals
    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);

    globalApp->start();

    // Keep main thread alive
    while (true) pause();

    return 0;
}
