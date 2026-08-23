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
#include "utils/Strings.hpp"

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

    std::cout << Strings::SERVER_SHUTDOWN;

    if (globalApp) {
        globalApp->shutdown(); // STOP HTTP endpoint
    }

    scheduler.stop(); // STOP scheduler

    exit(0);
}

int main() {
    if (!DBManager::getInstance().initializeDB(Strings::DB_PATH)) {
        std::cerr << Strings::DB_INIT_FAIL;
        return 1;
    }

    //Creating admin on the start of application
    std::cout << Strings::CREATE_ADMIN;
    Database::createDefaultAdmin();
    
    scheduler.start();

    Pistache::Port port(Strings::SERVER_PORT);
    Pistache::Address addr(Pistache::Ipv4::any(), port);

    std::cout << Strings::SERVER_START << Strings::SERVER_URL << Strings::SERVER_PORT << " ..." << std::endl;

    globalApp = std::make_shared<ServerApp>(addr);
    globalApp->init(4);

    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);

    globalApp->start();

    while (true) pause();  // Wait indefinitely until signal is caught
    return 0;
}
