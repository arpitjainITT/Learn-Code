#include "AdminService.h"
#include "../utils/HttpClient.h"
#include "../constants/APIEndpoints.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

std::vector<ExternalServer> AdminService::getAllServers() {
    std::vector<ExternalServer> servers;

    try {
        std::string resStr = HttpClient::get(API::ALL_SERVERS);
        auto jsonArr = json::parse(resStr);

        for (const auto& item : jsonArr) {
            ExternalServer s;
            s.id = item["id"];
            s.name = item["name"];
            s.status = item["status"];
            s.lastAccessed = item["last_accessed"];
            servers.push_back(s);
        }
    } catch (...) {
        std::cerr << "Failed to fetch servers.\n";
    }

    return servers;
}

ExternalServer AdminService::getServerDetails(int id) {
    ExternalServer server;
    try {
        std::string resStr = HttpClient::get(API::SERVER_DETAILS + std::to_string(id));
        auto item = json::parse(resStr);
        server.id = item["id"];
        server.name = item["name"];
        server.status = item["status"];
        server.apiKey = item["api_key"];
        server.lastAccessed = item["last_accessed"];
    } catch (...) {
        std::cerr << "Failed to fetch server details.\n";
    }

    return server;
}

void AdminService::updateServerApiKey(int id, const std::string& newKey) {
    json body = { {"api_key", newKey} };
    HttpClient::put(API::UPDATE_API_KEY + std::to_string(id) + "/key", body.dump());
}

void AdminService::updateServerStatus(int id, const std::string& newStatus) {
    json body = { {"status", newStatus} };
    HttpClient::put(API::UPDATE_SERVER_STATUS + std::to_string(id) + "/status", body.dump());
}

void AdminService::addCategory(const std::string& categoryName) {
    json body = { {"category", categoryName} };
    HttpClient::post(API::ADD_CATEGORY, body.dump());
}
