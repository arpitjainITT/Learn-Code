#include "AdminService.hpp"
#include "../database/DataBase.hpp"
#include <iostream>
#include "../utils/Strings.hpp"

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
        std::cerr << Strings::ADMIN_SERVICE_FAILED_UPDATE_API_KEY;
        return false;
    }
}

bool AdminService::updateExternalServerStatus(int serverId, const std::string& newStatus) {
    try {
        Database::updateExternalServerStatus(serverId, newStatus);
        return true;
    } catch (...) {
        std::cerr << Strings::ADMIN_SERVICE_FAILED_UPDATE_STATUS;
        return false;
    }
}

bool AdminService::updateExternalServerStatus(const std::string& serverName, const std::string& status) {
    try {
        Database::updateExternalServerStatusByName(serverName, status);
        return true;
    } catch (...) {
        std::cerr << Strings::ADMIN_SERVICE_FAILED_UPDATE_STATUS_BY_NAME;
        return false;
    }
}

bool AdminService::addCategory(const std::string& category) {
    try {
        Database::addCategory(category);
        return true;
    } catch (...) {
        std::cerr << Strings::ADMIN_SERVICE_FAILED_ADD_CATEGORY;
        return false;
    }
}

void AdminService::hideCategory(const std::string& category) {
    Database::hideCategory(category);
}
void AdminService::unhideCategory(const std::string& category) {
    Database::unhideCategory(category);
}
void AdminService::addFilteredKeyword(const std::string& keyword) {
    Database::addFilteredKeyword(keyword);
}
void AdminService::removeFilteredKeyword(const std::string& keyword) {
    Database::removeFilteredKeyword(keyword);
}
nlohmann::json AdminService::getHiddenCategories() {
    return Database::getHiddenCategories();
}
nlohmann::json AdminService::getFilteredKeywords() {
    return Database::getFilteredKeywords();
}
