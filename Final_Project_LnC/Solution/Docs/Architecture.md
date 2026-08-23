# News Aggregator System — Architecture

## 1. System Overview

The News Aggregator is a two-tier C++17 application:

- **Server** — REST API (Pistache) + SQLite database + background news fetcher
- **Client** — Console UI that calls the server's REST API over HTTP

```
┌──────────────────────────────────────────────────────────────┐
│                    CLIENT APPLICATION                        │
│                      (Console UI)                           │
│  App → LoginMenu / UserMenu / AdminMenu / HeadlinesMenu     │
│              ↓  HTTP/REST  libcurl                          │
│         HttpClient  (Authorization: Bearer <JWT>)           │
└───────────────────────────┬──────────────────────────────────┘
                            │  HTTP  port 9080
┌───────────────────────────▼──────────────────────────────────┐
│                    SERVER APPLICATION                        │
│                    (Pistache REST)                          │
│                                                              │
│  ┌──────────────┐  ┌──────────────┐  ┌────────────────────┐ │
│  │ Auth Routes  │  │Article Routes│  │Notification Routes │ │
│  │ Admin Routes │  │              │  │                    │ │
│  └──────┬───────┘  └──────┬───────┘  └────────┬───────────┘ │
│         │                 │                    │             │
│  ┌──────▼─────────────────▼────────────────────▼──────────┐  │
│  │              Service Layer                             │  │
│  │  UserService  ArticleService  NotificationService      │  │
│  │  AdminService                                          │  │
│  └──────────────────────────┬──────────────────────────────┘ │
│                             │                                │
│  ┌──────────────────────────▼──────────────────────────────┐  │
│  │              Database Layer  (DataBase.cpp)             │  │
│  │            DBManager singleton (sqlite3*)               │  │
│  └──────────────────────────┬──────────────────────────────┘ │
│                             │                                │
│  ┌──────────────────────────▼──────────────────────────────┐  │
│  │             SQLite  (news9.db)                         │  │
│  │  15 tables: user, news_article, saved_news,            │  │
│  │  notifications, reports, categories, ...               │  │
│  └─────────────────────────────────────────────────────────┘ │
│                                                              │
│  ┌─────────────────────────────────────────────────────────┐  │
│  │          FetchScheduler  (background thread)           │  │
│  │   Every 3 hours → INewsApiAdapter → parse JSON        │  │
│  │   → CategoryClassifier → Database::storeArticle()     │  │
│  │   → notifyUsersIfMatched() → EmailSender              │  │
│  └─────────────────────────────────────────────────────────┘ │
└──────────────────────────────────────────────────────────────┘
                             │  HTTPS  libcurl
┌───────────────┐   ┌────────▼──────────────────────────────────┐
│ TheNewsAPI    │   │  Gmail SMTP  (EmailSender)                │
│ NewsAPI.org   │   └───────────────────────────────────────────┘
└───────────────┘
```

---

## 2. Server Layer Breakdown

### 2.1 Entry Point (`main.cpp`)

```
main()
  1. DBManager::initializeDB("news9.db")   → creates all 15 tables
  2. Database::createDefaultAdmin()         → inserts headmaster@news.com if absent
  3. FetchScheduler::start()                → background thread begins
  4. Pistache server starts on port 9080
  5. signal(SIGINT/SIGTERM) → graceful shutdown
```

### 2.2 Route Layer

Each route group lives in its own `.cpp` file. `RouteManager::setupRoutes()` wires them all to the Pistache router.

| File | URL prefix | Auth required |
|---|---|---|
| `auth_routes.cpp` | `/auth/*` | None (public) |
| `article_routes.cpp` | `/articles/*` | Mixed (see API docs) |
| `admin_routes.cpp` | `/admin/*` | Admin JWT |
| `notification_routes.cpp` | `/notifications/*` | User JWT |

### 2.3 JWT Authentication Flow

```
Client Login
    │  POST /auth/login  {email, password}
    ▼
auth_routes.cpp → UserService::login() → Database::authenticateUser()
    │  PasswordHasher::hash(password) compared against stored SHA-256 hash
    ▼
JwtUtils::generateToken(userId, role)
    │  HMAC-SHA256 signed with JWT_SECRET env var
    │  24-hour expiry
    ▼
Response: {status, user_id, role, token:"eyJ..."}
    │
    ▼
Client stores token → HttpClient::setAuthToken(token)
    │  All subsequent requests include:
    │  Authorization: Bearer eyJ...
    ▼
Protected routes call JwtUtils::requireAuth() or JwtUtils::requireAdmin()
    │  Validates signature, issuer, expiry
    │  Extracts user_id and role from claims
    ▼
userId from token is used — client cannot impersonate another user
```

### 2.4 News Fetch Pipeline

```
FetchScheduler (every 180 min)
    │
    ├─ Query DB: get active servers + API keys
    │
    ├─ TheNewsApiAdapter::fetchArticles()   or
    │  NewsApiOrgAdapter::fetchArticles()
    │      │  GET https://api.thenewsapi.com/...
    │      │  Parse JSON["data"] array
    │      │  Format to normalized article JSON
    │
    └─ For each article:
           ArticleService::storeArticle(article)
               │
               ├─ Dedup by URL (SELECT id WHERE url = ?)
               ├─ CategoryClassifier::classify(title, content)
               │    keyword matching → Technology / Business / Sports / ...
               ├─ INSERT INTO news_article
               ├─ INSERT INTO news_article_category
               └─ Database::notifyUsersIfMatched(title, content, category, id)
                       │
                       ├─ Check notification_category_pref
                       ├─ Check notification_keyword_pref
                       ├─ INSERT INTO notifications
                       └─ EmailSender::sendEmail() via Gmail SMTP
```

---

## 3. Client Layer Breakdown

### 3.1 Application Flow

```
App::run() [infinite loop]
    │
    ├─ showHomeMenu()          → print 1.Login  2.Signup  3.Exit
    │
    ├─ handleLogin()
    │    AuthService::login()  → POST /auth/login
    │    → extract token       → HttpClient::setAuthToken(token)
    │    → user.role == "admin"?
    │         YES → AdminMenu::display()
    │         NO  → UserMenu::display()
    │
    └─ handleSignup()
         AuthService::signup() → POST /auth/signup
```

### 3.2 User Menu Tree

```
UserMenu
├── 1. Headlines      → HeadlinesMenu
│       ├── 1. Today
│       │     └─ show all / by category → article detail
│       │           ├── 1. Save Article
│       │           ├── 2. Like
│       │           ├── 3. Dislike
│       │           ├── 4. Report
│       │           └── 5. Back
│       ├── 2. Date Range
│       └── 3. Back
├── 2. Saved Articles → SavedArticlesMenu
│       ├── 1. View Saved Articles
│       ├── 2. Delete Article
│       └── 3. Back
├── 3. Search         → SearchMenu
│       ├── 1. Search Articles (keyword + dates + sort)
│       └── 2. Back
├── 4. Notifications  → NotificationMenu
│       ├── 1. View Notifications
│       ├── 2. View Preferences
│       ├── 3. Configure Category Notifications
│       ├── 4. Configure Keyword Notifications
│       └── 5. Back
└── 5. Logout
```

### 3.3 Admin Menu Tree

```
AdminMenu
├──  1. View External Servers
├──  2. View Server Details
├──  3. Update Server API Key
├──  4. Update Server Status (active/inactive)
├──  5. Add New Category
├──  6. Review Reported Articles
├──  7. Hide / Unhide Article
├──  8. Hide / Unhide Category
├──  9. Manage Filtered Keywords
├── 10. Show Hidden Categories
├── 11. Show Filtered Keywords
└── 12. Logout
```

---

## 4. Database Schema (15 Tables)

```
user_role          server_status
    │                    │
user ───────────────────────────────────────────────
    │
    ├─────── saved_news ──────── news_article ──── news_article_category ── news_category
    │                                 │
    ├─────── news_article_reaction    │
    │                                 │
    ├─────── notification_category_pref (→ news_category)
    ├─────── notification_keyword_pref
    ├─────── notifications (→ news_article)
    │
    └─────── reports (→ news_article)

external_server (→ server_status)
hidden_category (→ news_category)
filtered_keyword
```

Key constraints:
- Passwords: SHA-256 hex digest (never plaintext)
- URLs: UNIQUE constraint prevents duplicate articles
- `is_hidden`: articles auto-hidden after 5 reports
- `report_count`: incremented per report; triggers auto-hide at threshold 5

---

## 5. Security Model

| Concern | Implementation |
|---|---|
| Password storage | SHA-256 via OpenSSL — no plaintext anywhere |
| Session tokens | HS256 JWT signed with `JWT_SECRET` env var — 24h expiry |
| API key storage | SQLite `external_server` table — managed via Admin Panel only |
| Email credentials | `EMAIL_FROM`, `EMAIL_APP_PASSWORD` env vars — never in source |
| SQL injection | All queries use `sqlite3_prepare_v2` + `sqlite3_bind_*` |
| Cross-user access | JWT `user_id` claim used server-side — client cannot spoof userId |
| Admin endpoints | Every admin route validates `role == "admin"` from JWT claim |

---

## 6. Technology Stack

| Component | Technology | Version |
|---|---|---|
| Language | C++ | 17 |
| Build system | CMake | ≥ 3.14 |
| HTTP server | Pistache | 0.5.9 |
| HTTP client | libcurl | 7.81+ |
| Database | SQLite | 3 |
| JSON | nlohmann/json | 3.11 (header-only, bundled) |
| Crypto | OpenSSL | 3.x |
| Testing | GoogleTest + GoogleMock | 1.12.1 (auto-downloaded) |
