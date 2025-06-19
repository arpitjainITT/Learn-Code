#include "Database.hpp"
#include "DBManager.hpp"
#include <nlohmann/json.hpp>
#include <sqlite3.h>
#include <iostream>

using json = nlohmann::json;

json Database::getAllArticles() {
    json result = json::array();
    sqlite3_stmt* stmt;
    const char* query = "SELECT * FROM articles;";
    if (sqlite3_prepare_v2(DBManager::getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            json article = {
                {"id", sqlite3_column_int(stmt, 0)},
                {"title", (const char*)sqlite3_column_text(stmt, 1)},
                {"content", (const char*)sqlite3_column_text(stmt, 2)},
                {"source", (const char*)sqlite3_column_text(stmt, 3)},
                {"url", (const char*)sqlite3_column_text(stmt, 4)},
                {"category", (const char*)sqlite3_column_text(stmt, 5)},
                {"published_at", (const char*)sqlite3_column_text(stmt, 6)}
            };
            result.push_back(article);
        }
    }
    sqlite3_finalize(stmt);
    return result;
}

json Database::getArticlesByCategory(const std::string& category) {
    json result = json::array();
    sqlite3_stmt* stmt;
    const char* query = "SELECT * FROM articles WHERE category = ?;";
    if (sqlite3_prepare_v2(DBManager::getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, category.c_str(), -1, SQLITE_STATIC);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            json article = {
                {"id", sqlite3_column_int(stmt, 0)},
                {"title", (const char*)sqlite3_column_text(stmt, 1)},
                {"content", (const char*)sqlite3_column_text(stmt, 2)},
                {"source", (const char*)sqlite3_column_text(stmt, 3)},
                {"url", (const char*)sqlite3_column_text(stmt, 4)},
                {"category", (const char*)sqlite3_column_text(stmt, 5)},
                {"published_at", (const char*)sqlite3_column_text(stmt, 6)}
            };
            result.push_back(article);
        }
    }
    sqlite3_finalize(stmt);
    return result;
}

json Database::getArticleById(int id) {
    json result = nullptr;
    sqlite3_stmt* stmt;
    const char* query = "SELECT * FROM articles WHERE id = ?;";
    if (sqlite3_prepare_v2(DBManager::getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            result = {
                {"id", sqlite3_column_int(stmt, 0)},
                {"title", (const char*)sqlite3_column_text(stmt, 1)},
                {"content", (const char*)sqlite3_column_text(stmt, 2)},
                {"source", (const char*)sqlite3_column_text(stmt, 3)},
                {"url", (const char*)sqlite3_column_text(stmt, 4)},
                {"category", (const char*)sqlite3_column_text(stmt, 5)},
                {"published_at", (const char*)sqlite3_column_text(stmt, 6)}
            };
        }
    }
    sqlite3_finalize(stmt);
    return result;
}

void Database::saveArticleForUser(int userId, int articleId) {
    sqlite3_stmt* stmt;
    const char* query = "INSERT INTO saved_articles (user_id, article_id) VALUES (?, ?);";
    if (sqlite3_prepare_v2(DBManager::getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_bind_int(stmt, 2, articleId);
        sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
}

json Database::getSavedArticlesForUser(int userId) {
    json result = json::array();
    sqlite3_stmt* stmt;
    const char* query =
        "SELECT a.id, a.title, a.content, a.source, a.url, a.category, a.published_at "
        "FROM articles a INNER JOIN saved_articles s ON a.id = s.article_id "
        "WHERE s.user_id = ?;";
    if (sqlite3_prepare_v2(DBManager::getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            json article = {
                {"id", sqlite3_column_int(stmt, 0)},
                {"title", (const char*)sqlite3_column_text(stmt, 1)},
                {"content", (const char*)sqlite3_column_text(stmt, 2)},
                {"source", (const char*)sqlite3_column_text(stmt, 3)},
                {"url", (const char*)sqlite3_column_text(stmt, 4)},
                {"category", (const char*)sqlite3_column_text(stmt, 5)},
                {"published_at", (const char*)sqlite3_column_text(stmt, 6)}
            };
            result.push_back(article);
        }
    }
    sqlite3_finalize(stmt);
    return result;
}

void Database::deleteSavedArticle(int savedId) {
    sqlite3_stmt* stmt;
    const char* query = "DELETE FROM saved_articles WHERE id = ?;";
    if (sqlite3_prepare_v2(DBManager::getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, savedId);
        sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
}

void Database::storeArticle(const json& article) {
    sqlite3_stmt* checkStmt;
    const char* checkQuery = "SELECT id FROM articles WHERE url = ?;";
    
    if (sqlite3_prepare_v2(DBManager::getDB(), checkQuery, -1, &checkStmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(checkStmt, 1, article["url"].get<std::string>().c_str(), -1, SQLITE_STATIC);

        // If article already exists, skip insertion
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            sqlite3_finalize(checkStmt);
            std::cout << "[Database] Duplicate article skipped: " << article["url"] << "\n";
            return;
        }
    }
    sqlite3_finalize(checkStmt);

    // Insert new article
    sqlite3_stmt* stmt;
    const char* insertQuery =
        "INSERT INTO articles (title, content, source, url, category, published_at) "
        "VALUES (?, ?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(DBManager::getDB(), insertQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        std::string title = article["title"].get<std::string>();
        std::string content = article["content"].get<std::string>();
        std::string source = article["source"].get<std::string>();
        std::string url = article["url"].get<std::string>();
        std::string category = article["category"].get<std::string>();
        std::string published_at = article["published_at"].get<std::string>();

        sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, content.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, source.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, url.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, category.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, published_at.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "[Database] Failed to insert article: " << sqlite3_errmsg(DBManager::getDB()) << "\n";
        } else {
            std::cout << "[Database] Article inserted: " << article["title"] << "\n";
        }
    } else {
        std::cerr << "[Database] Failed to prepare insert statement.\n";
    }

    sqlite3_finalize(stmt);
}

