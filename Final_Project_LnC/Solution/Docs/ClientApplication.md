# News Aggregator — Client Application

## Overview

The client is a C++17 console application that communicates with the server over
HTTP/REST using **libcurl**. It handles user authentication (including JWT token
storage), browsing news headlines, saving articles, managing notifications, and
admin operations.

---

## Actual Folder Structure

```
ClientApplication/
├── CMakeLists.txt
├── main.cpp                     # Entry point — constructs App and calls run()
├── constants/
│   ├── APIEndpoints.h           # All server endpoint URLs (BASE_URL + path)
│   └── Strings.h                # All display strings, regex patterns, min-lengths
├── core/
│   ├── App.h/.cpp               # Root orchestrator: home menu, login/signup, menu routing
├── models/
│   ├── Article.h                # Plain struct: id, title, description, category, source, url, createdAt
│   ├── User.h                   # Class with id, role, email, token (JWT stored here after login)
│   └── Notification.h           # Plain struct: id, title, message, category, url, source, read
├── services/                    # Each service makes HTTP calls via HttpClient
│   ├── AuthService.h/.cpp       # login() → POST /auth/login; extracts JWT, calls setAuthToken()
│   ├── NewsService.h/.cpp       # getArticles(), searchArticles(), saveArticle(), likeArticle(), etc.
│   ├── NotificationService.h/.cpp # getNotifications(), setCategoryPreference(), setKeywordPreference()
│   ├── SavedArticleService.h/.cpp # getSavedArticles(), deleteSavedArticle()
│   └── AdminService.h/.cpp      # getAllServers(), updateServerApiKey(), addCategory(), etc.
├── ui/                          # Console menus — each class has a display() loop
│   ├── BaseMenu.h               # Abstract base with display() and renderItem() interface
│   ├── LoginMenu.h/.cpp         # Not used directly — App handles login flow
│   ├── UserMenu.h/.cpp          # Main user menu: Headlines, Saved, Search, Notifications, Logout
│   ├── AdminMenu.h/.cpp         # Admin panel: servers, categories, keywords, moderation
│   ├── HeadlinesMenu.h/.cpp     # Browse Today / Date Range; article detail + save/like/dislike/report
│   ├── SearchMenu.h/.cpp        # Keyword + date range search; sort by likes/dislikes
│   ├── NotificationMenu.h/.cpp  # View notifications; configure category/keyword preferences
│   └── SavedArticlesMenu.h/.cpp # List and delete saved articles
└── utils/
    ├── HttpClient.h/.cpp        # libcurl wrapper: get(), post(), put(), deleteRequest(), setAuthToken()
    ├── Validator.h/.cpp         # isValidEmail(), isValidPassword(), isValidUsername()
    ├── ConsoleUtils.h/.cpp      # clear(), getValidatedInput(), pause()
    └── Paginator.h              # Template paginator; redirects cin for testability
```

---

## Authentication Flow (JWT)

After the JWT implementation, the full authentication flow is:

```
1. User enters email + password in App::handleLogin()
2. AuthService::login() → POST /auth/login → {"email":"...","password":"..."}
3. Server validates credentials, returns:
   {"status":"success","user_id":1,"role":"user","token":"eyJ..."}
4. AuthService stores token:
     user.setToken(token)
     HttpClient::setAuthToken(token)   ← injects Bearer header on ALL subsequent requests
5. All further HTTP calls automatically include:
     Authorization: Bearer eyJ...
6. Server validates the token on every protected endpoint
```

**Logout** clears the token by calling `HttpClient::setAuthToken("")` and returning to the home menu.

---

## Service Layer

Each service is a stateless class with static methods. All communication goes through `HttpClient`.

| Service | Key methods | Endpoint(s) |
|---|---|---|
| `AuthService` | `login()`, `signup()` | `POST /auth/login`, `POST /auth/signup` |
| `NewsService` | `getArticles()`, `searchArticles()`, `saveArticle()`, `likeArticle()`, `dislikeArticle()`, `reportArticle()` | `GET /articles`, `POST /articles/save`, etc. |
| `SavedArticleService` | `getSavedArticles()`, `deleteSavedArticle()` | `GET /articles/saved/:id`, `DELETE /articles/save` |
| `NotificationService` | `getNotifications()`, `setCategoryPreference()`, `setKeywordPreference()`, `getPreferences()`, `markNotificationAsRead()` | `/notifications/*` |
| `AdminService` | `getAllServers()`, `getServerDetails()`, `updateServerApiKey()`, `updateServerStatus()`, `addCategory()`, `getReportedArticles()`, `hideArticle()`, `hideCategory()`, `addFilteredKeyword()`, etc. | `/admin/*` |

---

## UI Layer

All menus follow the same pattern: constructor receives the current `User`, `display()` loops until the user chooses Logout/Back.

```
App::run()
    └─ showHomeMenu()  →  [1=Login, 2=Signup, 3=Exit]
            ↓
    handleLogin()  →  AuthService::login()  →  user.role == "admin" ?
            │                                           │
      showUserMenu()                            showAdminMenu()
            │                                           │
      UserMenu::display()                    AdminMenu::display()
      ├── HeadlinesMenu::display()           ├── List servers
      ├── SavedArticlesMenu::display()       ├── View server details
      ├── SearchMenu::display()              ├── Update API key / status
      └── NotificationMenu::display()        ├── Add category
                                             ├── Review reported articles
                                             ├── Hide/unhide article or category
                                             └── Manage filtered keywords
```

---

## HttpClient

`HttpClient` is a thin libcurl wrapper. All four HTTP methods return `std::string` — empty string on failure (no throws).

```cpp
HttpClient::setAuthToken("eyJ...");   // Stored in module-level static; injected as Bearer header
HttpClient::get(url);                 // GET with optional Bearer header
HttpClient::post(url, body);          // POST with JSON Content-Type + optional Bearer header
HttpClient::put(url, body);           // PUT
HttpClient::deleteRequest(url);       // DELETE
```

---

## Validator

| Method | Rule |
|---|---|
| `isValidEmail(str)` | Regex: `^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$` |
| `isValidPassword(str)` | `str.length() >= 6` (configured via `Strings::VALIDATOR_PASSWORD_MIN_LENGTH`) |
| `isValidUsername(str)` | `!str.empty()` |

---

## Key Design Decisions

| Area | Decision |
|---|---|
| HTTP | libcurl only — no Boost, no POCO |
| Token storage | `User::token` field + `HttpClient` static variable; cleared on logout |
| Error handling | Services catch all JSON parse exceptions; return empty containers on failure |
| Input validation | Done client-side before sending any HTTP request |
| Pagination | `Paginator<T>` template; works with any displayable type; redirects `std::cin` |
| No session persistence | Token lives in memory only; user must log in again after restart |

---

## Build

```bash
cd ClientApplication
cmake -S . -B build
cmake --build build
./build/NewsAggregatorClient
```

Requires the **server to be running first** on `http://localhost:9080`.

## Run Tests

```bash
cd ClientApplication/tests
cmake -S . -B build
cmake --build build
./build/ClientTests
```

Tests cover: `Validator`, `Paginator`, `HttpClient` (failure paths), `ConsoleUtils`,
models (`User`, `Article`, `Notification`), string constants, and API endpoint compositions.
Network-dependent service tests and stdin-blocking UI tests are intentionally excluded.
