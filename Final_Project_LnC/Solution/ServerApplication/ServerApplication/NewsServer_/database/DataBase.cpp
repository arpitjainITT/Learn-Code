#include "Database.hpp"
#include "DBManager.hpp"
#include "../utils/Strings.hpp"
#include <nlohmann/json.hpp>
#include <sqlite3.h>
#include <iostream>
#include "../utils/CategoryClassifier.hpp"

using json = nlohmann::json;

static void finalize(sqlite3_stmt* stmt) {
    if (stmt) sqlite3_finalize(stmt);
}

json Database::getAllArticles() {
    json result = json::array();
    sqlite3_stmt* stmt;
    const char* query = R"(
        SELECT a.id, a.title, a.description, a.url, a.source, c.category_type
        FROM news_article a
        LEFT JOIN news_article_category ac ON a.id = ac.news_id
        LEFT JOIN news_category c ON ac.category_id = c.id
        WHERE a.is_hidden = 0
          AND (c.id IS NULL OR c.id NOT IN (SELECT category_id FROM hidden_category))
          AND NOT EXISTS (
              SELECT 1 FROM filtered_keyword fk
              WHERE a.title LIKE '%' || fk.keyword || '%'
                 OR a.description LIKE '%' || fk.keyword || '%'
                 OR a.content LIKE '%' || fk.keyword || '%'
          )
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            json article = {
                {"id", sqlite3_column_int(stmt, 0)},
                {"title", (const char*)sqlite3_column_text(stmt, 1)},
                {"description", (const char*)sqlite3_column_text(stmt, 2)},
                {"url", (const char*)sqlite3_column_text(stmt, 3)},
                {"source", (const char*)sqlite3_column_text(stmt, 4)},
                {"category", sqlite3_column_text(stmt, 5) ? (const char*)sqlite3_column_text(stmt, 5) : Strings::DB_DEFAULT_CATEGORY}
            };
            result.push_back(article);
        }
    } else {
        std::cerr << Strings::DB_FETCH_ARTICLES_FAIL;
    }
    finalize(stmt);
    return result;
}

json Database::getArticlesByCategory(const std::string& category) {
    json result = json::array();
    sqlite3_stmt* stmt;
    const char* query = R"(
        SELECT a.id, a.title, a.description, a.url, a.source
        FROM news_article a
        LEFT JOIN news_article_category ac ON a.id = ac.news_id
        LEFT JOIN news_category c ON ac.category_id = c.id
        WHERE c.category_type = ?
          AND a.is_hidden = 0
          AND (c.id IS NULL OR c.id NOT IN (SELECT category_id FROM hidden_category))
          AND NOT EXISTS (
              SELECT 1 FROM filtered_keyword fk
              WHERE a.title LIKE '%' || fk.keyword || '%'
                 OR a.description LIKE '%' || fk.keyword || '%'
                 OR a.content LIKE '%' || fk.keyword || '%'
          )
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, category.c_str(), -1, SQLITE_STATIC);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            json article = {
                {"id", sqlite3_column_int(stmt, 0)},
                {"title", (const char*)sqlite3_column_text(stmt, 1)},
                {"description", (const char*)sqlite3_column_text(stmt, 2)},
                {"url", (const char*)sqlite3_column_text(stmt, 3)},
                {"source", (const char*)sqlite3_column_text(stmt, 4)}
            };
            result.push_back(article);
        }
    }
    finalize(stmt);
    return result;
}

json Database::getArticleById(int id) {
    json article;
    sqlite3_stmt* stmt;
    const char* query = R"(
        SELECT a.id, a.title, a.description, a.url, a.source, c.category_type
        FROM news_article a
        LEFT JOIN news_article_category ac ON a.id = ac.news_id
        LEFT JOIN news_category c ON ac.category_id = c.id
        WHERE a.id = ?
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            article = {
                {"id", sqlite3_column_int(stmt, 0)},
                {"title", (const char*)sqlite3_column_text(stmt, 1)},
                {"description", (const char*)sqlite3_column_text(stmt, 2)},
                {"url", (const char*)sqlite3_column_text(stmt, 3)},
                {"source", (const char*)sqlite3_column_text(stmt, 4)},
                {"category", sqlite3_column_text(stmt, 5) ? (const char*)sqlite3_column_text(stmt, 5) : Strings::DB_DEFAULT_CATEGORY}
            };
        }
    }
    finalize(stmt);
    return article;
}

void Database::saveArticleForUser(int userId, int articleId) {
    sqlite3_stmt* stmt;
    const char* query = R"(
        INSERT OR IGNORE INTO saved_news(user_id, news_id)
        VALUES (?, ?)
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_bind_int(stmt, 2, articleId);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << Strings::DB_SAVE_ARTICLE_FAIL;
        }
    } else {
        std::cerr << Strings::DB_PREPARE_SAVE_ARTICLE_FAIL;
    }
    finalize(stmt);
}

json Database::getSavedArticlesForUser(int userId) {
    json result = json::array();
    sqlite3_stmt* stmt;
    const char* query = R"(
        SELECT a.id, a.title, a.description, a.url, a.source,c.category_type
        FROM news_article a
        JOIN saved_news s ON a.id = s.news_id
        LEFT JOIN news_article_category ac ON a.id = ac.news_id
        LEFT JOIN news_category c ON ac.category_id = c.id
        WHERE s.user_id = ?
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            json article = {
                {"id", sqlite3_column_int(stmt, 0)},
                {"title", (const char*)sqlite3_column_text(stmt, 1)},
                {"description", (const char*)sqlite3_column_text(stmt, 2)},
                {"url", (const char*)sqlite3_column_text(stmt, 3)},
                {"source", (const char*)sqlite3_column_text(stmt, 4)},
                {"category", sqlite3_column_text(stmt, 5) ? (const char*)sqlite3_column_text(stmt, 5) : Strings::DB_DEFAULT_CATEGORY}
            };
            result.push_back(article);
        }
    } else {
        std::cerr << Strings::DB_FETCH_SAVED_ARTICLES_FAIL;
    }
    finalize(stmt);
    return result;
}

void Database::deleteSavedArticle(int userId, int articleId) {
    sqlite3_stmt* stmt;
    const char* query = R"(
        DELETE FROM saved_news
        WHERE user_id = ? AND news_id = ?
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_bind_int(stmt, 2, articleId);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << Strings::DB_DELETE_SAVED_ARTICLE_FAIL;
        }
        std::cerr << Strings::DB_DELETE_SAVED_ARTICLE_SUCCESS;
    }
    finalize(stmt);
}

void Database::likeArticle(int userId, int articleId) {
    sqlite3_stmt* stmt;
    const char* query = R"(
        INSERT INTO news_article_reaction(user_id, news_id, reaction_type)
        VALUES (?, ?, 'like')
        ON CONFLICT(user_id, news_id)
        DO UPDATE SET reaction_type = 'like', reacted_at = CURRENT_TIMESTAMP
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_bind_int(stmt, 2, articleId);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << Strings::DB_LIKE_ARTICLE_FAIL;
        }
    }
    finalize(stmt);
}

void Database::dislikeArticle(int userId, int articleId) {
    sqlite3_stmt* stmt;
    const char* query = R"(
        INSERT INTO news_article_reaction(user_id, news_id, reaction_type)
        VALUES (?, ?, 'dislike')
        ON CONFLICT(user_id, news_id)
        DO UPDATE SET reaction_type = 'dislike', reacted_at = CURRENT_TIMESTAMP
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_bind_int(stmt, 2, articleId);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << Strings::DB_DISLIKE_ARTICLE_FAIL;
        }
    }
    finalize(stmt);
}

json Database::searchArticles(const std::string &keyword, const std::string &startDate, const std::string &endDate, const std::string &sortBy){
    json result = json::array();
    sqlite3_stmt *stmt;

    std::string query = R"(
    SELECT a.id, a.title, a.description, a.url, a.source,
           COALESCE(c.category_type, 'Uncategorized') AS category,
           COUNT(CASE WHEN r.reaction_type = 'like' THEN 1 END) AS likes,
           COUNT(CASE WHEN r.reaction_type = 'dislike' THEN 1 END) AS dislikes
    FROM news_article a
    LEFT JOIN news_article_category ac ON a.id = ac.news_id
    LEFT JOIN news_category c ON ac.category_id = c.id
    LEFT JOIN news_article_reaction r ON a.id = r.news_id
    WHERE a.is_hidden = 0
      AND (c.id IS NULL OR c.id NOT IN (SELECT category_id FROM hidden_category))
      AND a.id NOT IN (
          SELECT id FROM news_article WHERE 
              (
                  SELECT COUNT(*) FROM filtered_keyword fk 
                  WHERE a.title LIKE '%' || fk.keyword || '%' OR a.description LIKE '%' || fk.keyword || '%' OR a.content LIKE '%' || fk.keyword || '%'
              ) > 0
      )
    )";

    if (!keyword.empty())
    {
        query += " AND (a.title LIKE ? OR a.description LIKE ? OR a.content LIKE ?)";
    }

    if (!startDate.empty())
    {
        query += " AND DATE(a.created_at) >= DATE(?)";
    }

    if (!endDate.empty())
    {
        query += " AND DATE(a.created_at) <= DATE(?)";
    }

    query += " GROUP BY a.id ";

    if (sortBy == "likes")
    {
        query += " ORDER BY likes DESC";
    }
    else if (sortBy == "dislikes")
    {
        query += " ORDER BY dislikes DESC";
    }
    else
    {
        query += " ORDER BY a.created_at DESC";
    }

    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << Strings::DB_PREPARE_SEARCH_QUERY_FAIL;
        return result;
    }

    int idx = 1;
    if (!keyword.empty())
    {
        std::string kwLike = "%" + keyword + "%";
        sqlite3_bind_text(stmt, idx++, kwLike.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, idx++, kwLike.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, idx++, kwLike.c_str(), -1, SQLITE_TRANSIENT);
    }
    if (!startDate.empty())
    {
        sqlite3_bind_text(stmt, idx++, startDate.c_str(), -1, SQLITE_TRANSIENT);
    }
    if (!endDate.empty())
    {
        sqlite3_bind_text(stmt, idx++, endDate.c_str(), -1, SQLITE_TRANSIENT);
    }
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        json article = {
            {"id", sqlite3_column_int(stmt, 0)},
            {"title", (const char*)sqlite3_column_text(stmt, 1)},
            {"description", (const char*)sqlite3_column_text(stmt, 2)},
            {"url", (const char*)sqlite3_column_text(stmt, 3)},
            {"source", (const char*)sqlite3_column_text(stmt, 4)},
            {"category", sqlite3_column_text(stmt, 5) ? (const char*)sqlite3_column_text(stmt, 5) : Strings::DB_DEFAULT_CATEGORY},
            {"likes", sqlite3_column_int(stmt, 6)},
            {"dislikes", sqlite3_column_int(stmt, 7)}
        };
        result.push_back(article);
    }
    finalize(stmt);
    return result;
}

json Database::getReactionStats(int articleId) {
    json result;
    sqlite3_stmt* stmt;
    const char* query = R"(
        SELECT
            SUM(CASE WHEN reaction_type = 'like' THEN 1 ELSE 0 END) AS likes,
            SUM(CASE WHEN reaction_type = 'dislike' THEN 1 ELSE 0 END) AS dislikes
        FROM news_article_reaction
        WHERE news_id = ?
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, articleId);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            result["likes"] = sqlite3_column_int(stmt, 0);
            result["dislikes"] = sqlite3_column_int(stmt, 1);
        }
    } else {
        std::cerr << Strings::DB_FETCH_REACTION_STATS_FAIL;
    }
    finalize(stmt);
    return result;
}

void Database::addCategory(const std::string& category) {
    sqlite3_stmt* stmt;
    const char* query = R"(
        INSERT OR IGNORE INTO news_category (category_type)
        VALUES (?)
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, category.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << Strings::DB_INSERT_CATEGORY_FAIL;
        }
    } else {
        std::cerr << Strings::DB_PREPARE_INSERT_CATEGORY_FAIL;
    }
    finalize(stmt);
}

int Database::getCategoryId(const std::string& category) {
    int categoryId = -1;
    sqlite3_stmt* stmt;
    const char* query = R"(
        SELECT id FROM news_category WHERE category_type = ?
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, category.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            categoryId = sqlite3_column_int(stmt, 0);
        }
    } else {
        std::cerr << Strings::DB_PREPARE_GET_CATEGORY_ID_FAIL;
    }
    finalize(stmt);
    return categoryId;
}

void Database::setCategoryNotificationPreference(int userId, int categoryId, bool isEnabled) {
    sqlite3_stmt* stmt;
    const char* query = R"(
        INSERT INTO notification_category_pref (user_id, category_id, is_enabled)
        VALUES (?, ?, ?)
        ON CONFLICT(user_id, category_id)
        DO UPDATE SET is_enabled = excluded.is_enabled
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_bind_int(stmt, 2, categoryId);
        sqlite3_bind_int(stmt, 3, isEnabled ? 1 : 0);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << Strings::DB_UPDATE_CATEGORY_PREF_FAIL;
        }
    }
    finalize(stmt);
}

void Database::setKeywordNotificationPreference(int userId, const std::string& keyword, bool isEnabled) {
    sqlite3_stmt* stmt;
    const char* query = R"(
        INSERT INTO notification_keyword_pref (user_id, keyword, is_enabled)
        VALUES (?, ?, ?)
        ON CONFLICT(user_id, keyword)
        DO UPDATE SET is_enabled = excluded.is_enabled
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_bind_text(stmt, 2, keyword.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, isEnabled ? 1 : 0);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << Strings::DB_UPDATE_KEYWORD_PREF_FAIL;
        }
    }
    finalize(stmt);
}

json Database::getUserNotificationPreferences(int userId) {
    json result;
    result["categories"] = json::array();
    result["keywords"] = json::array();

    sqlite3_stmt* stmt1;
    const char* query1 = R"(
        SELECT c.category_type, p.is_enabled
        FROM notification_category_pref p
        JOIN news_category c ON p.category_id = c.id
        WHERE p.user_id = ?
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query1, -1, &stmt1, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt1, 1, userId);
        while (sqlite3_step(stmt1) == SQLITE_ROW) {
            result["categories"].push_back({
                {"category", (const char*)sqlite3_column_text(stmt1, 0)},
                {"enabled", sqlite3_column_int(stmt1, 1) == 1}
            });
        }
    }
    finalize(stmt1);

    sqlite3_stmt* stmt2;
    const char* query2 = R"(
        SELECT keyword, is_enabled
        FROM notification_keyword_pref
        WHERE user_id = ?
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query2, -1, &stmt2, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt2, 1, userId);
        while (sqlite3_step(stmt2) == SQLITE_ROW) {
            result["keywords"].push_back({
                {"keyword", (const char*)sqlite3_column_text(stmt2, 0)},
                {"enabled", sqlite3_column_int(stmt2, 1) == 1}
            });
        }
    }
    finalize(stmt2);

    return result;
}

void Database::addNotificationsForCategory(int userId, const std::string& category) {
    sqlite3_stmt* stmt;
    const char* query = R"(
        SELECT a.id, a.title
        FROM news_article a
        LEFT JOIN news_article_category ac ON a.id = ac.news_id
        LEFT JOIN news_category c ON ac.category_id = c.id
        WHERE c.category_type = ?
    )";

    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, category.c_str(), -1, SQLITE_STATIC);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int articleId = sqlite3_column_int(stmt, 0);
            std::string title = (const char*)sqlite3_column_text(stmt, 1);

            sqlite3_stmt* insertStmt;
            const char* insertQuery = R"(
                INSERT OR IGNORE INTO notifications (user_id, article_id, title, message, read)
                VALUES (?, ?, ?, ?, 0)
            )";

            if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), insertQuery, -1, &insertStmt, nullptr) == SQLITE_OK) {
                std::string msg = "New article in category: " + category;
                sqlite3_bind_int(insertStmt, 1, userId);
                sqlite3_bind_int(insertStmt, 2, articleId);
                sqlite3_bind_text(insertStmt, 3, title.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(insertStmt, 4, msg.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_step(insertStmt);
            }
            finalize(insertStmt);
        }
    }
    finalize(stmt);
}

void Database::filterNotificationsByKeyword(int userId, const std::string& keyword) {
    // Delete existing notifications not matching keyword
    const char* deleteQuery = R"(
        DELETE FROM notifications
        WHERE user_id = ? AND article_id NOT IN (
            SELECT a.id
            FROM news_article a
            WHERE (a.title LIKE '%' || ? || '%' OR a.content LIKE '%' || ? || '%')
        )
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), deleteQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_bind_text(stmt, 2, keyword.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, keyword.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
    }
    finalize(stmt);
}


json Database::getDeliveredNotifications(int userId) {
    json result = json::array();

    sqlite3_stmt* stmt;
    const char* query = R"(
        SELECT a.id, a.title, a.description, a.url, a.source, c.category_type, n.read
        FROM notifications n
        JOIN news_article a ON n.article_id = a.id
        LEFT JOIN news_article_category ac ON a.id = ac.news_id
        LEFT JOIN news_category c ON ac.category_id = c.id
        WHERE n.user_id = ?
        ORDER BY n.timestamp DESC
    )";

    std::cout << Strings::DB_PREPARE_NOTIFICATION_FETCH_FAIL << userId << "\n";
    int rc = sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr);

    if (rc == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            json article = {
                {"id", sqlite3_column_int(stmt, 0)},
                {"title", (const char*)sqlite3_column_text(stmt, 1)},
                {"description", (const char*)sqlite3_column_text(stmt, 2)},
                {"url", (const char*)sqlite3_column_text(stmt, 3)},
                {"source", (const char*)sqlite3_column_text(stmt, 4)},
                {"category", sqlite3_column_text(stmt, 5) ? (const char*)sqlite3_column_text(stmt, 5) : Strings::DB_DEFAULT_CATEGORY},
                {"read", sqlite3_column_int(stmt, 6) == 1}
            };
            result.push_back(article);
        }
    } else {
        std::cerr << Strings::DB_PREPARE_NOTIFICATION_FETCH_FAIL << sqlite3_errmsg(DBManager::getInstance().getDB()) << "\n";
    }

    finalize(stmt);
    return result;
}

json Database::getUnreadNotifications(int userId) {
    json result = json::array();

    const char* query = R"(
        SELECT a.id, a.title, a.description, a.url, a.source, c.category_type, n.read
        FROM notifications n
        JOIN news_article a ON n.article_id = a.id
        LEFT JOIN news_article_category ac ON a.id = ac.news_id
        LEFT JOIN news_category c ON ac.category_id = c.id
        WHERE n.user_id = ? AND n.read = 0
        ORDER BY n.timestamp DESC
    )";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            json article = {
                {"id", sqlite3_column_int(stmt, 0)},
                {"title", (const char*)sqlite3_column_text(stmt, 1)},
                {"description", (const char*)sqlite3_column_text(stmt, 2)},
                {"url", (const char*)sqlite3_column_text(stmt, 3)},
                {"source", (const char*)sqlite3_column_text(stmt, 4)},
                {"category", sqlite3_column_text(stmt, 5) ? (const char*)sqlite3_column_text(stmt, 5) : Strings::DB_DEFAULT_CATEGORY},
                {"read", sqlite3_column_int(stmt, 6) == 1}
            };
            result.push_back(article);
        }
    }
    finalize(stmt);
    return result;
}

void Database::insertNotification(int userId, int articleId, const std::string& title, const std::string& message) {
    sqlite3_stmt* stmt;
    std::cout << Strings::DB_INSERTING_NOTIFICATION << userId << " " << Strings::DB_AND_ARTICLE_ID << " " << articleId << ": " << title << " " << Strings::DB_AND_MESSAGE_IS << " " << message << " " << Strings::DB_AT_LINE << " " << __LINE__ << ")\n";
    const char* query = R"(
        INSERT INTO notifications (user_id, article_id, title, message, read, timestamp)
        VALUES (?, ?, ?, ?, 0, CURRENT_TIMESTAMP)
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_bind_int(stmt, 2, articleId); 
        sqlite3_bind_text(stmt, 3, title.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, message.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << Strings::DB_INSERT_NOTIFICATION_FAIL << userId << ".\n";
        }
    }
    finalize(stmt);
}

void Database::markNotificationsAsRead(int userId) {
    sqlite3_stmt* stmt;
    const char* query = R"(
        UPDATE notifications SET read = 1 WHERE user_id = ?
    )";

    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_step(stmt);
    }
    finalize(stmt);
}

void Database::notifyUsersIfMatched(const std::string& title, const std::string& content, const std::string& category, int articleId) {
    sqlite3* db = DBManager::getInstance().getDB();

    // === Notify by category ===
    const char* categoryQuery = R"(
        SELECT p.user_id
        FROM notification_category_pref p
        JOIN news_category c ON p.category_id = c.id
        WHERE c.category_type = ? AND p.is_enabled = 1
    )";

    sqlite3_stmt* catStmt;
    if (sqlite3_prepare_v2(db, categoryQuery, -1, &catStmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(catStmt, 1, category.c_str(), -1, SQLITE_STATIC);
        while (sqlite3_step(catStmt) == SQLITE_ROW) {
            int userId = sqlite3_column_int(catStmt, 0);

            // Check if already inserted
            const char* existsQuery = R"(
                SELECT 1 FROM notifications WHERE user_id = ? AND title = ?
            )";
            sqlite3_stmt* existsStmt;
            if (sqlite3_prepare_v2(db, existsQuery, -1, &existsStmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(existsStmt, 1, userId);
                sqlite3_bind_text(existsStmt, 2, title.c_str(), -1, SQLITE_TRANSIENT);
                if (sqlite3_step(existsStmt) != SQLITE_ROW) {
                    std::string message = Strings::DB_NEW_ARTICLE_CATEGORY + category;
                    std::cout << Strings::DB_NOTIFY_CALLED << category << "\n";
                    insertNotification(userId, articleId, title, message);
                }
            }
            finalize(existsStmt);
        }
    }
    finalize(catStmt);

    // === Notify by keyword ===
    const char* keywordQuery = R"(
        SELECT user_id, keyword
        FROM notification_keyword_pref
        WHERE is_enabled = 1
    )";
    sqlite3_stmt* kwStmt;
    if (sqlite3_prepare_v2(db, keywordQuery, -1, &kwStmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(kwStmt) == SQLITE_ROW) {
            int userId = sqlite3_column_int(kwStmt, 0);
            std::string keyword = (const char*)sqlite3_column_text(kwStmt, 1);

            if (title.find(keyword) != std::string::npos || content.find(keyword) != std::string::npos) {
                std::string message = Strings::DB_NEW_ARTICLE_KEYWORD + keyword;
                std::cout << Strings::DB_NOTIFY_CALLED << keyword << "\n";
                insertNotification(userId, articleId, title, message);
            }
        }
    }
    finalize(kwStmt);
}

json Database::getAllExternalServers() {
    json result = json::array();
    sqlite3_stmt* stmt;
    const char* query = R"(
        SELECT es.id, es.server_name, es.api_url, es.api_key, ss.type, es.last_accessed
        FROM external_server es
        JOIN server_status ss ON es.server_status_id = ss.id
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            json server = {
                {"id", sqlite3_column_int(stmt, 0)},
                {"server_name", (const char*)sqlite3_column_text(stmt, 1)},
                {"api_url", (const char*)sqlite3_column_text(stmt, 2)},
                {"api_key", (const char*)sqlite3_column_text(stmt, 3)},
                {"status", (const char*)sqlite3_column_text(stmt, 4)},
                {"last_accessed", (const char*)sqlite3_column_text(stmt, 5)}
            };
            result.push_back(server);
        }
    }
    finalize(stmt);
    return result;
}

json Database::getExternalServerById(int serverId) {
    json server;
    sqlite3_stmt* stmt;
    const char* query = R"(
        SELECT es.id, es.server_name, es.api_url, es.api_key, ss.type, es.last_accessed
        FROM external_server es
        JOIN server_status ss ON es.server_status_id = ss.id
        WHERE es.id = ?
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, serverId);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            server = {
                {"id", sqlite3_column_int(stmt, 0)},
                {"server_name", (const char*)sqlite3_column_text(stmt, 1)},
                {"api_url", (const char*)sqlite3_column_text(stmt, 2)},
                {"api_key", (const char*)sqlite3_column_text(stmt, 3)},
                {"status", (const char*)sqlite3_column_text(stmt, 4)},
                {"last_accessed", (const char*)sqlite3_column_text(stmt, 5)}
            };
        }
    }
    finalize(stmt);
    return server;
}

void Database::updateExternalServerApiKey(int serverId, const std::string& newApiKey) {
    sqlite3_stmt* stmt;
    const char* query = R"(
        UPDATE external_server SET api_key = ? WHERE id = ?
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, newApiKey.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, serverId);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << Strings::DB_FAILED_UPDATE_EXTERNAL_SERVER_API_KEY;
        }
    }
    finalize(stmt);
}

void Database::updateExternalServerStatus(int serverId, const std::string& status) {
    sqlite3_stmt* stmt;
    const char* query = R"(
        UPDATE external_server
        SET server_status_id = (SELECT id FROM server_status WHERE type = ?)
        WHERE id = ?
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, status.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, serverId);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << Strings::DB_FAILED_UPDATE_SERVER_STATUS;
        }
    }
    finalize(stmt);
}

void Database::updateExternalServerStatusByName(const std::string& serverName, const std::string& status) {
    sqlite3_stmt* stmt;
    const char* query = R"(
        UPDATE external_server
        SET server_status_id = (SELECT id FROM server_status WHERE type = ?),
            last_accessed = CURRENT_TIMESTAMP
        WHERE server_name = ?
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, status.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, serverName.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << Strings::DB_FAILED_UPDATE_SERVER_STATUS_BY_NAME;
        }
    }
    finalize(stmt);
}


int Database::getUserIdByEmail(const std::string& email) {
    sqlite3_stmt* stmt;
    int userId = -1;
    const char* query = R"(
        SELECT id FROM user WHERE email = ?
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            userId = sqlite3_column_int(stmt, 0);
        }
    }
    finalize(stmt);
    return userId;
}

bool Database::authenticateUser(const std::string& email, const std::string& password, int& userId, std::string& role) {
    sqlite3_stmt* stmt;
    const char* query = R"(
        SELECT u.id, ur.type
        FROM user u
        JOIN user_role ur ON u.role_id = ur.id
        WHERE u.email = ? AND u.password = ?
    )";
    bool success = false;
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
        std::cout << Strings::DB_VALUE_OF_EMAIL << email << std::endl;
        std::cout << Strings::DB_VALUE_OF_PASSWORD << password << std::endl;
        if (sqlite3_step(stmt) == SQLITE_ROW){
            userId = sqlite3_column_int(stmt, 0);
            role = (const char*)sqlite3_column_text(stmt, 1);
            success = true;
        }
    }
    finalize(stmt);
    return success;
}

bool Database::registerUser(const std::string& username, const std::string& email, const std::string& password) {
    sqlite3_stmt* stmt;
    const char* query = R"(
        INSERT INTO user (username, email, password, role_id)
        VALUES (?, ?, ?, (SELECT id FROM user_role WHERE type = 'user'))
    )";
    bool success = false;
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            success = true;
        } else {
            std::cerr << Strings::DB_FAILED_REGISTER_USER;
        }
    }
    finalize(stmt);
    return success;
}

void Database::createDefaultAdmin() {
    sqlite3* db = DBManager::getInstance().getDB();

    // Check if admin already exists
    sqlite3_stmt* stmt;
    const char* checkQuery = R"(
        SELECT id FROM user WHERE email = 'headmaster@news.com'
    )";
    if (sqlite3_prepare_v2(db, checkQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << Strings::DB_DEFAULT_ADMIN_EXISTS;
            sqlite3_finalize(stmt);
            return;
        }
    }
    sqlite3_finalize(stmt);

    // Insert default admin
    const char* insertQuery = R"(
        INSERT INTO user (username, email, password, role_id)
        VALUES ('Headmaster', 'headmaster@news.com', 'headmaster1223', 
            (SELECT id FROM user_role WHERE type = 'admin'))
    )";
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, insertQuery, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << Strings::DB_FAILED_CREATE_ADMIN << errMsg << "\n";
        sqlite3_free(errMsg);
    } else {
        std::cout << Strings::DB_ADMIN_CREATED;
    }
}

void Database::storeArticle(const json& article) {
    sqlite3* db = DBManager::getInstance().getDB();

    sqlite3_stmt* checkStmt;
    std::string category;
    std::string title;
    std::string content;
    const char* checkQuery = "SELECT id FROM news_article WHERE url = ?;";
    if (sqlite3_prepare_v2(db, checkQuery, -1, &checkStmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(checkStmt, 1, article["url"].get<std::string>().c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            sqlite3_finalize(checkStmt);
            std::cout << Strings::DB_DUPLICATE_ARTICLE_SKIPPED << article["url"] << "\n";
            return;
        }
    }
    sqlite3_finalize(checkStmt);

    sqlite3_stmt* insertStmt;
    const char* insertQuery = R"(
        INSERT INTO news_article (uuid, title, description, category, language, locale, url, image_url, content, source)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";
    if (sqlite3_prepare_v2(db, insertQuery, -1, &insertStmt, nullptr) == SQLITE_OK) {

        std::string uuid = article.contains("uuid") && !article["uuid"].is_null() ? article["uuid"].get<std::string>() : "";
        title = article.contains("title") && !article["title"].is_null() ? article["title"].get<std::string>() : "";
        std::string description = article.contains("description") && !article["description"].is_null() ? article["description"].get<std::string>() : "";
        std::string language = article.contains("language") && !article["language"].is_null() ? article["language"].get<std::string>() : "en";
        std::string locale = article.contains("locale") && !article["locale"].is_null() ? article["locale"].get<std::string>() : "us";
        std::string url = article.contains("url") && !article["url"].is_null() ? article["url"].get<std::string>() : "";
        std::string image_url = article.contains("image_url") && !article["image_url"].is_null() ? article["image_url"].get<std::string>() : "";
        content = article.contains("content") && !article["content"].is_null() ? article["content"].get<std::string>() : "";
        std::string source = article.contains("source") && !article["source"].is_null() ? article["source"].get<std::string>() : "";

        category = (!article.contains("category") || article["category"].is_null()) ? ""
            : (article["category"].is_string() && !article["category"].get<std::string>().empty() ? article["category"].get<std::string>() 
                : CategoryClassifier::classify(title, description));      //If category is there then fetch otherwise classify the category
        std::cout << "[Database] Classified category: " << category << "\n" << "[Database] Article Source: " << source << "\n";

        sqlite3_bind_text(insertStmt, 1, uuid.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(insertStmt, 2, title.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(insertStmt, 3, description.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(insertStmt, 4, category.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(insertStmt, 5, language.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(insertStmt, 6, locale.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(insertStmt, 7, url.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(insertStmt, 8, image_url.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(insertStmt, 9, content.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(insertStmt, 10, source.c_str(), -1, SQLITE_TRANSIENT);

        
         std::cout << Strings::DB_ARTICLE_INSERTED << article.value("title", "No Title") << " (at line " << __LINE__ << ")\n";

        if (sqlite3_step(insertStmt) != SQLITE_DONE) {
            std::string err = sqlite3_errmsg(db);
            if (err.find("UNIQUE constraint failed") != std::string::npos) {
                std::cout << Strings::DB_DUPLICATE_ARTICLE << article["url"] << "\n";
            } else {
                std::cerr << Strings::DB_FAILED_INSERT_ARTICLE << err << "\n";
                sqlite3_finalize(insertStmt);
                return;
            }
        }
    } else {
        std::cerr << Strings::DB_FAILED_PREPARE_INSERT;
        return;
    }
    sqlite3_finalize(insertStmt);
    std::cout << Strings::DB_ARTICLE_INSERTED << article.value("title", "No Title") << " (at line " << __LINE__ << ")\n";
    int articleId = sqlite3_last_insert_rowid(db);

    addCategory(category); 
    int categoryId = getCategoryId(category);
    if (categoryId != -1) {
        sqlite3_stmt* mapStmt;
        const char* mapQuery = R"(
            INSERT OR IGNORE INTO news_article_category (news_id, category_id)
            VALUES (?, ?)
        )";
        if (sqlite3_prepare_v2(db, mapQuery, -1, &mapStmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(mapStmt, 1, articleId);
            sqlite3_bind_int(mapStmt, 2, categoryId);
            if (sqlite3_step(mapStmt) != SQLITE_DONE) {
                std::cerr << Strings::DB_FAILED_MAP_ARTICLE_CATEGORY;
            }
        }
        sqlite3_finalize(mapStmt);
    }

    std::cout << Strings::DB_NOTIFY_CALLED << category << "\n";
    notifyUsersIfMatched(title, content, category, articleId);

     std::cout << Strings::DB_ARTICLE_INSERTED << article.value("title", "No Title") << " (at line " << __LINE__ << ")\n";

    sqlite3_stmt* updateStmt;
    const char* updateQuery = "UPDATE external_server SET last_accessed = CURRENT_TIMESTAMP WHERE id = ?";
    if (sqlite3_prepare_v2(db, updateQuery, -1, &updateStmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(updateStmt, 1, 1);
        sqlite3_step(updateStmt);
    }
    sqlite3_finalize(updateStmt);
}

void Database::reportArticle(int userId, int articleId, const std::string& reason) {
    sqlite3* db = DBManager::getInstance().getDB();
    // Insert report
    sqlite3_stmt* stmt;
    const char* insertQuery = R"(
        INSERT INTO reports (article_id, user_id, reason) VALUES (?, ?, ?)
    )";
    if (sqlite3_prepare_v2(db, insertQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, articleId);
        sqlite3_bind_int(stmt, 2, userId);
        sqlite3_bind_text(stmt, 3, reason.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
    }
    finalize(stmt);
    // Increment report_count
    const char* updateQuery = R"(
        UPDATE news_article SET report_count = report_count + 1 WHERE id = ?
    )";
    if (sqlite3_prepare_v2(db, updateQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, articleId);
        sqlite3_step(stmt);
    }
    finalize(stmt);
    // Auto-hide if threshold reached (threshold = 5)
    const char* checkQuery = R"(
        SELECT report_count FROM news_article WHERE id = ?
    )";
    int count = 0;
    if (sqlite3_prepare_v2(db, checkQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, articleId);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
    }
    finalize(stmt);
    if (count >= 5) {
        hideArticle(articleId);
    }
}

void Database::hideArticle(int articleId) {
    sqlite3_stmt* stmt;
    const char* query = "UPDATE news_article SET is_hidden = 1 WHERE id = ?";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, articleId);
        sqlite3_step(stmt);
    }
    finalize(stmt);
}

void Database::unhideArticle(int articleId) {
    sqlite3_stmt* stmt;
    const char* query = "UPDATE news_article SET is_hidden = 0 WHERE id = ?";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, articleId);
        sqlite3_step(stmt);
    }
    finalize(stmt);
}

void Database::hideCategory(const std::string& category) {
    int categoryId = getCategoryId(category);
    if (categoryId == -1) return;
    sqlite3_stmt* stmt;
    const char* query = "INSERT OR IGNORE INTO hidden_category (category_id) VALUES (?)";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, categoryId);
        sqlite3_step(stmt);
    }
    finalize(stmt);
}

void Database::unhideCategory(const std::string& category) {
    int categoryId = getCategoryId(category);
    if (categoryId == -1) return;
    sqlite3_stmt* stmt;
    const char* query = "DELETE FROM hidden_category WHERE category_id = ?";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, categoryId);
        sqlite3_step(stmt);
    }
    finalize(stmt);
}

void Database::addFilteredKeyword(const std::string& keyword) {
    sqlite3_stmt* stmt;
    const char* query = "INSERT OR IGNORE INTO filtered_keyword (keyword) VALUES (?)";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, keyword.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
    }
    finalize(stmt);
}

void Database::removeFilteredKeyword(const std::string& keyword) {
    sqlite3_stmt* stmt;
    const char* query = "DELETE FROM filtered_keyword WHERE keyword = ?";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, keyword.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
    }
    finalize(stmt);
}

nlohmann::json Database::getReportedArticles() {
    nlohmann::json result = nlohmann::json::array();
    sqlite3_stmt* stmt;
    const char* query = R"(
        SELECT a.id, a.title, a.report_count, a.is_hidden, COUNT(r.id) as num_reports
        FROM news_article a
        LEFT JOIN reports r ON a.id = r.article_id
        WHERE a.report_count > 0
        GROUP BY a.id
        ORDER BY num_reports DESC
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            nlohmann::json article = {
                {"id", sqlite3_column_int(stmt, 0)},
                {"title", (const char*)sqlite3_column_text(stmt, 1)},
                {"report_count", sqlite3_column_int(stmt, 2)},
                {"is_hidden", sqlite3_column_int(stmt, 3)},
                {"num_reports", sqlite3_column_int(stmt, 4)}
            };
            result.push_back(article);
        }
    }
    finalize(stmt);
    return result;
}

nlohmann::json Database::getHiddenCategories() {
    nlohmann::json result = nlohmann::json::array();
    sqlite3_stmt* stmt;
    const char* query = R"(
        SELECT c.category_type
        FROM hidden_category h
        JOIN news_category c ON h.category_id = c.id
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            result.push_back((const char*)sqlite3_column_text(stmt, 0));
        }
    }
    finalize(stmt);
    return result;
}

nlohmann::json Database::getFilteredKeywords() {
    nlohmann::json result = nlohmann::json::array();
    sqlite3_stmt* stmt;
    const char* query = "SELECT keyword FROM filtered_keyword";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            result.push_back((const char*)sqlite3_column_text(stmt, 0));
        }
    }
    finalize(stmt);
    return result;
}

nlohmann::json Database::getAllCategories() {
    nlohmann::json result = nlohmann::json::array();
    sqlite3_stmt* stmt;
    const char* query = R"(
        SELECT c.category_type
        FROM news_category c
        WHERE c.id NOT IN (SELECT category_id FROM hidden_category)
        ORDER BY c.category_type
    )";
    if (sqlite3_prepare_v2(DBManager::getInstance().getDB(), query, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            result.push_back((const char*)sqlite3_column_text(stmt, 0));
        }
    }
    finalize(stmt);
    return result;
}
