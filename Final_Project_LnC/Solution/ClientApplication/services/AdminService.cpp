#include "AdminService.h"
#include "../utils/HttpClient.h"
#include "../constants/APIEndpoints.h"
#include "../constants/Strings.h"
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
            s.id = item.value("id", 0);
            s.name = item.value("server_name", "");
            s.status = item.value("status", "");
            s.lastAccessed = item.value("last_accessed", "");
            s.apiKey = item.value("api_key", "");
            servers.push_back(s);
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to fetch servers: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << Strings::ADMIN_SERVICE_FETCH_SERVERS_FAIL;
    }

    return servers;
}

ExternalServer AdminService::getServerDetails(int id) {
    ExternalServer server;
    try {
        std::string resStr = HttpClient::get(API::SERVER_DETAILS + std::to_string(id));
        auto item = json::parse(resStr);
        server.id = item.value("id", 0);
        server.name = item.value("server_name", "");
        server.status = item.value("status", "");
        server.apiKey = item.value("api_key", "");
        server.lastAccessed = item.value("last_accessed", "");
    } catch (const std::exception& e) {
        std::cerr << "Failed to fetch server details: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << Strings::ADMIN_SERVICE_FETCH_SERVER_DETAILS_FAIL;
    }

    return server;
}

void AdminService::updateServerApiKey(int id, const std::string& newKey) {
    try {
        json body = { {"api_key", newKey} };
        HttpClient::put(API::UPDATE_API_KEY + std::to_string(id) + "/key", body.dump());
        std::cout << "API key updated successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Failed to update API key: " << e.what() << std::endl;
    }
}

void AdminService::updateServerStatus(int id, const std::string& newStatus) {
    try {
        json body = { {"status", newStatus} };
        HttpClient::put(API::UPDATE_SERVER_STATUS + std::to_string(id) + "/status", body.dump());
        std::cout << "Server status updated successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Failed to update server status: " << e.what() << std::endl;
    }
}

void AdminService::addCategory(const std::string& categoryName) {
    json body = { {"category", categoryName} };
    HttpClient::post(API::ADD_CATEGORY, body.dump());
}

std::vector<ReportedArticle> AdminService::getReportedArticles() {
    std::vector<ReportedArticle> articles;
    try {
        std::string resStr = HttpClient::get(API::REPORTED_ARTICLES);
        auto jsonArr = json::parse(resStr);
        for (const auto& item : jsonArr) {
            ReportedArticle a;
            a.id = item.value("id", 0);
            a.title = item.value("title", "");
            a.reportCount = item.value("report_count", 0);
            a.isHidden = item.value("is_hidden", 0) != 0;
            a.numReports = item.value("num_reports", 0);
            articles.push_back(a);
        }
    } catch (...) {
        std::cerr << "Failed to fetch reported articles.\n";
    }
    return articles;
}

void AdminService::hideArticle(int articleId) {
    HttpClient::post(API::HIDE_ARTICLE + std::to_string(articleId) + "/hide", "{}");
}
void AdminService::unhideArticle(int articleId) {
    HttpClient::post(API::UNHIDE_ARTICLE + std::to_string(articleId) + "/unhide", "{}");
}
void AdminService::hideCategory(const std::string& category) {
    json body = { {"category", category} };
    HttpClient::post(API::HIDE_CATEGORY, body.dump());
}
void AdminService::unhideCategory(const std::string& category) {
    json body = { {"category", category} };
    HttpClient::post(API::UNHIDE_CATEGORY, body.dump());
}
void AdminService::addFilteredKeyword(const std::string& keyword) {
    json body = { {"keyword", keyword} };
    HttpClient::post(API::ADD_FILTERED_KEYWORD, body.dump());
}
void AdminService::removeFilteredKeyword(const std::string& keyword) {
    json body = { {"keyword", keyword} };
    HttpClient::post(API::REMOVE_FILTERED_KEYWORD, body.dump());
}
std::vector<std::string> AdminService::getHiddenCategories() {
    std::vector<std::string> categories;
    try {
        std::string resStr = HttpClient::get(API::HIDDEN_CATEGORIES);
        auto jsonArr = json::parse(resStr);
        for (const auto& item : jsonArr) {
            categories.push_back(item.get<std::string>());
        }
    } catch (...) {
        std::cerr << "Failed to fetch hidden categories.\n";
    }
    return categories;
}
std::vector<std::string> AdminService::getFilteredKeywords() {
    std::vector<std::string> keywords;
    try {
        std::string resStr = HttpClient::get(API::FILTERED_KEYWORDS);
        auto jsonArr = json::parse(resStr);
        for (const auto& item : jsonArr) {
            keywords.push_back(item.get<std::string>());
        }
    } catch (...) {
        std::cerr << "Failed to fetch filtered keywords.\n";
    }
    return keywords;
}
