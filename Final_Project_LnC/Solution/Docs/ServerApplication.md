# News Aggregator — Server Application

## Overview

The server is a C++17 REST API built with **Pistache**, backed by **SQLite**, and
uses **libcurl** for outbound HTTP requests to external news APIs.

---

## Actual Folder Structure

```
NewsServer_/
├── CMakeLists.txt
├── main.cpp                        # Entry: DBManager → createDefaultAdmin → FetchScheduler → Pistache
├── adapters/
│   ├── INewsApiAdapter.hpp         # Abstract interface (apiKey constructor)
│   ├── NewsApiOrgAdapter.hpp/.cpp  # newsapi.org adapter
│   └── TheNewsApiAdapter.hpp/.cpp  # thenewsapi.com adapter
├── database/
│   ├── DBManager.hpp/.cpp          # Singleton sqlite3* wrapper; runs schema on startup
│   └── DataBase.hpp/.cpp           # All raw SQLite queries (~1300 lines)
├── routes/
│   ├── routeManager.hpp/.cpp       # Wires all route groups to Pistache::Router
│   ├── admin_routes.hpp/.cpp       # /admin/*
│   ├── article_routes.hpp/.cpp     # /articles/*
│   ├── auth_routes.hpp/.cpp        # /auth/login, /auth/signup
│   └── notification_routes.hpp/.cpp# /notifications/*
├── scheduler/
│   └── FetchScheduler.hpp/.cpp     # Background thread; reads API keys from DB; runs every 3 h
├── services/
│   ├── AdminService.hpp/.cpp       # Manages external servers, categories, keywords
│   ├── ArticleService.hpp/.cpp     # Article CRUD, reactions, reporting
│   ├── NotificationService.hpp/.cpp# User notification preferences
│   └── UserService.hpp/.cpp        # login / signup delegation
└── utils/
    ├── CategoryClassifier.hpp/.cpp # Keyword-based category classification
    ├── EmailSender.hpp/.cpp        # Gmail SMTP via libcurl (credentials from env vars)
    ├── HttpClient.hpp/.cpp         # libcurl GET wrapper
    ├── JsonUtils.hpp               # Shared safeGet() helper for JSON parsing
    ├── PasswordHasher.hpp          # SHA-256 via OpenSSL
    └── Strings.hpp                 # All string/int constants (no secrets)
```

---

## Key Design Decisions

| Area | Decision |
|---|---|
| HTTP framework | Pistache (thread-pool, 4 threads) |
| Database | SQLite 3 via singleton `DBManager` |
| Password storage | SHA-256 hash (never plaintext) |
| Authentication | JWT HS256 — secret from `JWT_SECRET` env var; 24 h expiry |
| API keys | Stored in `external_server` DB table; managed via Admin Panel |
| Email credentials | Read from `EMAIL_FROM`, `EMAIL_APP_PASSWORD` env vars |
| Category classification | Keyword matching in `CategoryClassifier::classify()` |
| News fetch strategy | Primary + fallback adapter; DB status drives which is tried first |

---

## REST API

### Auth
| Method | Endpoint | Description |
|---|---|---|
| POST | `/auth/login` | `{"email","password"}` → `{"status","user_id","role"}` |
| POST | `/auth/signup` | `{"username","email","password"}` → `{"status"}` |

### Articles
| Method | Endpoint | Description |
|---|---|---|
| GET | `/articles` | All articles (optionally `?userId=N` for personalized feed) |
| GET | `/articles/:id` | Single article |
| GET | `/articles/category/:cat` | Articles by category |
| GET | `/articles/categories` | List of categories |
| GET | `/articles/search` | `?keyword=&startDate=&endDate=&sort=` |
| POST | `/articles/save` | `{"user_id","article_id"}` |
| DELETE | `/articles/save` | `{"userId","articleId"}` |
| GET | `/articles/saved/:userId` | User's saved articles |
| POST | `/articles/:id/like` | `{"user_id"}` |
| POST | `/articles/:id/dislike` | `{"user_id"}` |
| GET | `/articles/:id/reactions` | Like/dislike counts |
| POST | `/articles/:id/report` | `{"user_id","reason"}` |

### Admin
| Method | Endpoint | Description |
|---|---|---|
| GET | `/admin/servers` | List configured news API servers |
| GET | `/admin/servers/:id` | Server details |
| PUT | `/admin/servers/:id/key` | `{"api_key"}` |
| PUT | `/admin/servers/:id/status` | `{"status"}` |
| POST | `/admin/category` | `{"category"}` |
| POST | `/admin/categories/hide` | `{"category"}` |
| POST | `/admin/categories/unhide` | `{"category"}` |
| GET | `/admin/hidden-categories` | Hidden categories list |
| POST | `/admin/keywords/filter` | `{"keyword"}` |
| POST | `/admin/keywords/unfilter` | `{"keyword"}` |
| GET | `/admin/filtered-keywords` | Filtered keywords list |
| POST | `/admin/articles/:id/hide` | Hide article |
| POST | `/admin/articles/:id/unhide` | Unhide article |
| GET | `/admin/reported-articles` | Articles with reports |

### Notifications
| Method | Endpoint | Description |
|---|---|---|
| POST | `/notifications/category` | `{"user_id","category","enabled"}` |
| POST | `/notifications/keyword` | `{"user_id","keyword","enabled"}` |
| GET | `/notifications/preferences/:userId` | Preferences |
| GET | `/notifications/:userId` | Delivered notifications |
| PUT | `/notifications/read/:userId` | Mark all as read |

---

## Build

```bash
cd NewsServer_
cmake -S . -B build
cmake --build build
./build/NewsAggregatorServer
```

## Run Tests

```bash
cd NewsServer_/tests
cmake -S . -B build
cmake --build build
./build/NewsServerTests
```
