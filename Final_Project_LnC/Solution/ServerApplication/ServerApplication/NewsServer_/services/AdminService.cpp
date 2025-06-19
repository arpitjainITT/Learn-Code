#include "AdminService.hpp"
#include "../database/DBManager.hpp"
#include "../database/DataBase.hpp"
#include "../utils/nlohmann/json.hpp"
#include <sqlite3.h>

using namespace Pistache;

void AdminService::listSources(const Rest::Request& /*request*/, Http::ResponseWriter response) {
    sqlite3* db = DBManager::getDB();
    std::string sql = "SELECT id, name, active, last_accessed FROM external_sources;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    nlohmann::json result = nlohmann::json::array();
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        result.push_back({
            {"id", sqlite3_column_int(stmt, 0)},
            {"name", (const char*)sqlite3_column_text(stmt, 1)},
            {"active", sqlite3_column_int(stmt, 2)},
            {"last_accessed", (const char*)sqlite3_column_text(stmt, 3)}
        });
    }

    sqlite3_finalize(stmt);
    response.send(Http::Code::Ok, result.dump());
}

void AdminService::viewSourceDetails(const Rest::Request& /*request*/, Http::ResponseWriter response) {
    sqlite3* db = DBManager::getDB();
    std::string sql = "SELECT id, name, url, api_key FROM external_sources;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    nlohmann::json result = nlohmann::json::array();
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        result.push_back({
            {"id", sqlite3_column_int(stmt, 0)},
            {"name", (const char*)sqlite3_column_text(stmt, 1)},
            {"url", (const char*)sqlite3_column_text(stmt, 2)},
            {"api_key", (const char*)sqlite3_column_text(stmt, 3)}
        });
    }

    sqlite3_finalize(stmt);
    response.send(Http::Code::Ok, result.dump());
}

void AdminService::updateSource(const Rest::Request& request, Http::ResponseWriter response) {
    int id = std::stoi(request.param(":id").as<std::string>());
    auto body = nlohmann::json::parse(request.body());
    std::string apiKey = body["apiKey"];

    sqlite3* db = DBManager::getDB();
    std::string sql = "UPDATE external_sources SET api_key = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, apiKey.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, id);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        response.send(Http::Code::Ok, "API key updated.");
    } else {
        response.send(Http::Code::Internal_Server_Error, "Update failed.");
    }
}

void AdminService::addCategory(const Rest::Request& request, Http::ResponseWriter response) {
    auto body = nlohmann::json::parse(request.body());
    std::string name = body["name"];

    sqlite3* db = DBManager::getDB();
    std::string sql = "INSERT INTO categories (name) VALUES (?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        response.send(Http::Code::Ok, "Category added.");
    } else {
        response.send(Http::Code::Conflict, "Category already exists or failed.");
    }
}
