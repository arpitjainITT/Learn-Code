#include "AdminService.hpp"
#include "../database/Database.hpp"
#include <iostream>

using json = nlohmann::json;

json AdminService::listExternalServers() {
    return Database::getAllExternalServers();
}

json AdminService::viewExternalServerDetails(int serverId) {
    return Database::getExternalServerById(serverId);
}

bool AdminService::updateExternalServerApiKey(int serverId, const std::string& newKey) {
    try {
        Database::updateExternalServerApiKey(serverId, newKey);
        return true;
    } catch (...) {
        std::cerr << "[AdminService] Failed to update API key.\n";
        return false;
    }
}

bool AdminService::updateExternalServerStatus(int serverId, const std::string& newStatus) {
    try {
        Database::updateExternalServerStatus(serverId, newStatus);
        return true;
    } catch (...) {
        std::cerr << "[AdminService] Failed to update server status.\n";
        return false;
    }
}

bool AdminService::updateExternalServerStatus(const std::string& serverName, const std::string& status) {
    try {
        Database::updateExternalServerStatusByName(serverName, status);
        return true;
    } catch (...) {
        std::cerr << "[AdminService] Failed to update server status by name.\n";
        return false;
    }
}

bool AdminService::addCategory(const std::string& category) {
    try {
        Database::addCategory(category);
        return true;
    } catch (...) {
        std::cerr << "[AdminService] Failed to add category.\n";
        return false;
    }
}
