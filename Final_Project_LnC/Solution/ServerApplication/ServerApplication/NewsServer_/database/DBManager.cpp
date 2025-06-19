#include "DBManager.hpp"
#include <iostream>

sqlite3* DBManager::db = nullptr;

void DBManager::init(const std::string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open DB\n";
    }

    const char* createArticles =
        "CREATE TABLE IF NOT EXISTS articles ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "title TEXT, content TEXT, source TEXT, url TEXT, category TEXT, published_at TEXT);";

    sqlite3_exec(db, createArticles, 0, 0, nullptr);

    const char* createUsers =
    "CREATE TABLE IF NOT EXISTS users ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "username TEXT UNIQUE, "
    "email TEXT UNIQUE, "
    "password TEXT, "
    "role TEXT);";  // 'admin' or 'user' for now

    sqlite3_exec(db, createUsers, 0, 0, nullptr);

    const char* createSavedArticles =
    "CREATE TABLE IF NOT EXISTS saved_articles ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "user_id INTEGER, "
    "article_id INTEGER, "
    "saved_at TEXT DEFAULT CURRENT_TIMESTAMP, "
    "FOREIGN KEY(user_id) REFERENCES users(id), "
    "FOREIGN KEY(article_id) REFERENCES articles(id));";

    sqlite3_exec(db, createSavedArticles, 0, 0, nullptr);

    const char* createNotifications =
    "CREATE TABLE IF NOT EXISTS notifications ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "user_id INTEGER, "
    "title TEXT, "
    "message TEXT, "
    "timestamp TEXT, "
    "FOREIGN KEY(user_id) REFERENCES users(id));";

    sqlite3_exec(db, createNotifications, 0, 0, nullptr);

    const char* createKeywordPrefs =
        "CREATE TABLE IF NOT EXISTS keyword_preferences ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id INTEGER, "
        "keyword TEXT, "
        "FOREIGN KEY(user_id) REFERENCES users(id));";

    sqlite3_exec(db, createKeywordPrefs, 0, 0, nullptr);

    const char* createExternalSources =
    "CREATE TABLE IF NOT EXISTS external_sources ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "name TEXT, "
    "url TEXT, "
    "api_key TEXT, "
    "active INTEGER DEFAULT 1, "
    "last_accessed TEXT);";

    sqlite3_exec(db, createExternalSources, 0, 0, nullptr);

    const char* createCategories =
        "CREATE TABLE IF NOT EXISTS categories ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT UNIQUE);";

    sqlite3_exec(db, createCategories, 0, 0, nullptr);

    

}

sqlite3* DBManager::getDB() {
    return db;
}
