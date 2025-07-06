#include "DBManager.hpp"
#include <iostream>

DBManager::~DBManager() {
    close();
}

DBManager& DBManager::getInstance() {
    static DBManager instance;
    return instance;
}

bool DBManager::initializeDB(const std::string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open DB: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    return executeSchema();
}

void DBManager::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DBManager::executeSchema() {
    const char* schema = R"(
        PRAGMA foreign_keys = ON;

        CREATE TABLE IF NOT EXISTS user_role (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            type TEXT NOT NULL UNIQUE
        );

        INSERT OR IGNORE INTO user_role(type) VALUES ('admin'), ('user');

        CREATE TABLE IF NOT EXISTS user (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            email TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL,
            role_id INTEGER NOT NULL,
            notification_viewed_at DATETIME DEFAULT NULL,
            FOREIGN KEY (role_id) REFERENCES user_role(id)
        );

        CREATE TABLE IF NOT EXISTS server_status (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            type TEXT NOT NULL UNIQUE
        );

        INSERT OR IGNORE INTO server_status(type) VALUES ('active'), ('inactive');

        CREATE TABLE IF NOT EXISTS external_server (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            server_name TEXT NOT NULL,
            api_url TEXT NOT NULL,
            api_key TEXT NOT NULL,
            server_status_id INTEGER NOT NULL,
            last_accessed DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (server_status_id) REFERENCES server_status(id) ON DELETE CASCADE
        );

        CREATE TABLE IF NOT EXISTS news_article (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            uuid TEXT,
            title TEXT NOT NULL,
            description TEXT,
            category TEXT,
            language TEXT,
            created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
            locale TEXT,
            url TEXT UNIQUE,
            image_url TEXT,
            content TEXT,
            source TEXT
        );

        CREATE TABLE IF NOT EXISTS news_category (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            category_type TEXT NOT NULL UNIQUE
        );

        CREATE TABLE IF NOT EXISTS news_article_category (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            news_id INTEGER NOT NULL,
            category_id INTEGER NOT NULL,
            created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (news_id) REFERENCES news_article(id) ON DELETE CASCADE,
            FOREIGN KEY (category_id) REFERENCES news_category(id) ON DELETE CASCADE,
            UNIQUE(news_id, category_id)
        );

        CREATE TABLE IF NOT EXISTS saved_news (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            news_id INTEGER NOT NULL,
            user_id INTEGER NOT NULL,
            saved_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (news_id) REFERENCES news_article(id) ON DELETE CASCADE,
            FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE,
            UNIQUE(user_id, news_id)
        );

        CREATE TABLE IF NOT EXISTS news_article_reaction (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            news_id INTEGER NOT NULL,
            reaction_type TEXT CHECK(reaction_type IN ('like', 'dislike')) NOT NULL,
            reacted_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE,
            FOREIGN KEY (news_id) REFERENCES news_article(id) ON DELETE CASCADE,
            UNIQUE(user_id, news_id)
        );

        CREATE TABLE IF NOT EXISTS notification_keyword_pref (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            keyword TEXT NOT NULL,
            is_enabled INTEGER NOT NULL DEFAULT 1,
            created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE,
            UNIQUE(user_id, keyword)
        );

        CREATE TABLE IF NOT EXISTS notification_category_pref (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            category_id INTEGER NOT NULL,
            is_enabled INTEGER NOT NULL DEFAULT 0,
            created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE,
            FOREIGN KEY (category_id) REFERENCES news_category(id) ON DELETE CASCADE,
            UNIQUE(user_id, category_id)
        );

        CREATE TABLE IF NOT EXISTS notifications (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER,
            article_id INTEGER,
            title TEXT,
            message TEXT,
            timestamp TEXT DEFAULT CURRENT_TIMESTAMP,
            read INTEGER DEFAULT 0,
            FOREIGN KEY(user_id) REFERENCES user(id),
            FOREIGN KEY(article_id) REFERENCES news_article(id)
        );

    )";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, schema, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Schema creation failed: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}
