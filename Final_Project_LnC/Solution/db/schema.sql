-- ============================================================
-- News Aggregator System — Database Schema
-- Source of truth: DBManager::executeSchema() in database/DBManager.cpp
-- This file is for reference and manual initialization only.
-- The application creates all tables automatically on startup.
-- ============================================================

PRAGMA foreign_keys = ON;

-- ── Lookup tables ──────────────────────────────────────────────────────────

CREATE TABLE IF NOT EXISTS user_role (
    id   INTEGER PRIMARY KEY AUTOINCREMENT,
    type TEXT    NOT NULL UNIQUE
);
INSERT OR IGNORE INTO user_role(type) VALUES ('admin'), ('user');

CREATE TABLE IF NOT EXISTS server_status (
    id   INTEGER PRIMARY KEY AUTOINCREMENT,
    type TEXT    NOT NULL UNIQUE
);
INSERT OR IGNORE INTO server_status(type) VALUES ('active'), ('inactive');

-- ── Users ──────────────────────────────────────────────────────────────────

CREATE TABLE IF NOT EXISTS user (
    id                    INTEGER PRIMARY KEY AUTOINCREMENT,
    username              TEXT    NOT NULL UNIQUE,
    email                 TEXT    NOT NULL UNIQUE,
    -- Stored as SHA-256 hex digest (PasswordHasher::hash())
    password              TEXT    NOT NULL,
    role_id               INTEGER NOT NULL,
    notification_viewed_at DATETIME DEFAULT NULL,
    FOREIGN KEY (role_id) REFERENCES user_role(id)
);

-- ── External news API servers ──────────────────────────────────────────────

CREATE TABLE IF NOT EXISTS external_server (
    id               INTEGER PRIMARY KEY AUTOINCREMENT,
    server_name      TEXT    NOT NULL UNIQUE,
    api_url          TEXT    NOT NULL,
    -- API key managed via Admin Panel; not hardcoded
    api_key          TEXT    NOT NULL,
    server_status_id INTEGER NOT NULL,
    last_accessed    DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (server_status_id) REFERENCES server_status(id) ON DELETE CASCADE
);

-- ── Articles ───────────────────────────────────────────────────────────────

CREATE TABLE IF NOT EXISTS news_article (
    id           INTEGER  PRIMARY KEY AUTOINCREMENT,
    uuid         TEXT,
    title        TEXT     NOT NULL,
    description  TEXT,
    category     TEXT,
    language     TEXT,
    created_at   DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    locale       TEXT,
    url          TEXT     UNIQUE,
    image_url    TEXT,
    content      TEXT,
    source       TEXT,
    is_hidden    INTEGER  NOT NULL DEFAULT 0,
    report_count INTEGER  NOT NULL DEFAULT 0
);

-- ── Categories (populated by CategoryClassifier + Admin) ──────────────────

CREATE TABLE IF NOT EXISTS news_category (
    id            INTEGER PRIMARY KEY AUTOINCREMENT,
    category_type TEXT    NOT NULL UNIQUE
);

-- Many-to-many: article ↔ category
CREATE TABLE IF NOT EXISTS news_article_category (
    id          INTEGER  PRIMARY KEY AUTOINCREMENT,
    news_id     INTEGER  NOT NULL,
    category_id INTEGER  NOT NULL,
    created_at  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (news_id)     REFERENCES news_article(id)  ON DELETE CASCADE,
    FOREIGN KEY (category_id) REFERENCES news_category(id) ON DELETE CASCADE,
    UNIQUE(news_id, category_id)
);

-- ── User interactions ─────────────────────────────────────────────────────

CREATE TABLE IF NOT EXISTS saved_news (
    id       INTEGER  PRIMARY KEY AUTOINCREMENT,
    news_id  INTEGER  NOT NULL,
    user_id  INTEGER  NOT NULL,
    saved_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (news_id)  REFERENCES news_article(id) ON DELETE CASCADE,
    FOREIGN KEY (user_id)  REFERENCES user(id)         ON DELETE CASCADE,
    UNIQUE(user_id, news_id)
);

CREATE TABLE IF NOT EXISTS news_article_reaction (
    id            INTEGER  PRIMARY KEY AUTOINCREMENT,
    user_id       INTEGER  NOT NULL,
    news_id       INTEGER  NOT NULL,
    reaction_type TEXT     CHECK(reaction_type IN ('like', 'dislike')) NOT NULL,
    reacted_at    DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id)  REFERENCES user(id)         ON DELETE CASCADE,
    FOREIGN KEY (news_id)  REFERENCES news_article(id) ON DELETE CASCADE,
    UNIQUE(user_id, news_id)
);

-- ── Notifications ─────────────────────────────────────────────────────────

CREATE TABLE IF NOT EXISTS notification_keyword_pref (
    id         INTEGER  PRIMARY KEY AUTOINCREMENT,
    user_id    INTEGER  NOT NULL,
    keyword    TEXT     NOT NULL,
    is_enabled INTEGER  NOT NULL DEFAULT 1,
    created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE,
    UNIQUE(user_id, keyword)
);

CREATE TABLE IF NOT EXISTS notification_category_pref (
    id          INTEGER  PRIMARY KEY AUTOINCREMENT,
    user_id     INTEGER  NOT NULL,
    category_id INTEGER  NOT NULL,
    is_enabled  INTEGER  NOT NULL DEFAULT 0,
    created_at  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id)     REFERENCES user(id)         ON DELETE CASCADE,
    FOREIGN KEY (category_id) REFERENCES news_category(id) ON DELETE CASCADE,
    UNIQUE(user_id, category_id)
);

CREATE TABLE IF NOT EXISTS notifications (
    id         INTEGER  PRIMARY KEY AUTOINCREMENT,
    user_id    INTEGER,
    article_id INTEGER,
    title      TEXT,
    message    TEXT,
    timestamp  TEXT     DEFAULT CURRENT_TIMESTAMP,
    read       INTEGER  DEFAULT 0,
    FOREIGN KEY (user_id)    REFERENCES user(id)         ON DELETE CASCADE,
    FOREIGN KEY (article_id) REFERENCES news_article(id) ON DELETE CASCADE
);

-- ── Content moderation ────────────────────────────────────────────────────

CREATE TABLE IF NOT EXISTS reports (
    id         INTEGER  PRIMARY KEY AUTOINCREMENT,
    article_id INTEGER  NOT NULL,
    user_id    INTEGER  NOT NULL,
    reason     TEXT,
    timestamp  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (article_id) REFERENCES news_article(id) ON DELETE CASCADE,
    FOREIGN KEY (user_id)    REFERENCES user(id)         ON DELETE CASCADE
);

-- Admin-hidden categories (articles in these categories are excluded from results)
CREATE TABLE IF NOT EXISTS hidden_category (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    category_id INTEGER NOT NULL,
    FOREIGN KEY (category_id) REFERENCES news_category(id) ON DELETE CASCADE,
    UNIQUE(category_id)
);

-- Admin-filtered keywords (articles matching these keywords are excluded)
CREATE TABLE IF NOT EXISTS filtered_keyword (
    id      INTEGER PRIMARY KEY AUTOINCREMENT,
    keyword TEXT    NOT NULL UNIQUE
);
