#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <pistache/net.h>
#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>

#include "routes/RouteManager.hpp"
#include "database/DBManager.hpp"
#include "database/DataBase.hpp"
#include "scheduler/FetchScheduler.hpp"

using namespace Pistache;

class ServerApp {
public:
    explicit ServerApp(Pistache::Address addr)
        : httpEndpoint(std::make_shared<Http::Endpoint>(addr)) {}

    void init(size_t threads = 2) {
        auto opts = Http::Endpoint::options().threads(static_cast<int>(threads));
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
        RouteManager::setupRoutes(router);  
    }

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};

std::shared_ptr<ServerApp> globalApp;
FetchScheduler scheduler;  
std::atomic<bool> isShuttingDown(false);

void handleSignal(int signal) {
    if (isShuttingDown.exchange(true)) {
        return; // Already shutting down
    }

    std::cout << "\nShutting down server peacefully and gracefully on your order.....\n";

    if (globalApp) {
        globalApp->shutdown(); // STOP HTTP endpoint
    }

    scheduler.stop(); // STOP scheduler

    exit(0);
}

int main() {
    if (!DBManager::getInstance().initializeDB("../../../../data/news9.db")) {
        std::cerr << "Failed to initialize database.\n";
        return 1;
    }

    //Creating admin on the start of application
    Database::createDefaultAdmin();
    
    scheduler.start();

    Pistache::Port port(9080);
    Pistache::Address addr(Pistache::Ipv4::any(), port);

    std::cout << "Starting server at http://localhost:" << port << " ..." << std::endl;

    globalApp = std::make_shared<ServerApp>(addr);
    globalApp->init(4);

    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);

    globalApp->start();

    while (true) pause();  // Wait indefinitely until signal is caught
    return 0;
}
