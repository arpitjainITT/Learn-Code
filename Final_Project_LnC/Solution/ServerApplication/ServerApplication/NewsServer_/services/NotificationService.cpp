#include "NotificationService.hpp"
#include "../database/DBManager.hpp"
#include "../database/DataBase.hpp"
#include "../utils/nlohmann/json.hpp"
#include <sqlite3.h>

using namespace Pistache;
using json = nlohmann::json;

void NotificationService::configureCategory(int userId, const std::string& category, bool enabled, Pistache::Http::ResponseWriter&& response) {
    sqlite3* db = DBManager::getDB();
    sqlite3_stmt* stmt;

    const char* sql = R"(
        INSERT INTO notification_preferences (user_id, category, enabled)
        VALUES (?, ?, ?)
        ON CONFLICT(user_id, category) DO UPDATE SET enabled = excluded.enabled;
    )";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_bind_text(stmt, 2, category.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, enabled ? 1 : 0);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            response.send(Http::Code::Ok, "Category preference updated.");
        } else {
            response.send(Http::Code::Internal_Server_Error, "Failed to update preference.");
        }

        sqlite3_finalize(stmt);
    } else {
        response.send(Http::Code::Internal_Server_Error, "Failed to prepare statement.");
    }
}

void NotificationService::configureKeyword(int userId, const std::string& keyword, Pistache::Http::ResponseWriter&& response) {
    sqlite3* db = DBManager::getDB();
    sqlite3_stmt* stmt;

    const char* sql = R"(
        INSERT INTO keyword_preferences (user_id, keyword)
        VALUES (?, ?);
    )";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_bind_text(stmt, 2, keyword.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            response.send(Http::Code::Ok, "Keyword added.");
        } else {
            response.send(Http::Code::Internal_Server_Error, "Failed to add keyword.");
        }

        sqlite3_finalize(stmt);
    } else {
        response.send(Http::Code::Internal_Server_Error, "Failed to prepare statement.");
    }
}

void NotificationService::getUserPreferences(int userId, Pistache::Http::ResponseWriter&& response) {
    sqlite3* db = DBManager::getDB();
    json result;
    result["user_id"] = userId;

    // Fetch categories
    const char* categorySql = "SELECT category FROM notification_preferences WHERE user_id = ? AND enabled = 1;";
    sqlite3_stmt* categoryStmt;
    std::vector<std::string> categories;

    if (sqlite3_prepare_v2(db, categorySql, -1, &categoryStmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(categoryStmt, 1, userId);
        while (sqlite3_step(categoryStmt) == SQLITE_ROW) {
            const unsigned char* categoryText = sqlite3_column_text(categoryStmt, 0);
            if (categoryText)
                categories.emplace_back(reinterpret_cast<const char*>(categoryText));
        }
    }
    sqlite3_finalize(categoryStmt);
    result["categories"] = categories;

    // Fetch keywords
    const char* keywordSql = "SELECT keyword FROM keyword_preferences WHERE user_id = ?;";
    sqlite3_stmt* keywordStmt;
    std::vector<std::string> keywords;

    if (sqlite3_prepare_v2(db, keywordSql, -1, &keywordStmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(keywordStmt, 1, userId);
        while (sqlite3_step(keywordStmt) == SQLITE_ROW) {
            const unsigned char* keywordText = sqlite3_column_text(keywordStmt, 0);
            if (keywordText)
                keywords.emplace_back(reinterpret_cast<const char*>(keywordText));
        }
    }
    sqlite3_finalize(keywordStmt);
    result["keywords"] = keywords;

    response.send(Http::Code::Ok, result.dump());
}

