# News Aggregator System

A two-tier C++17 application that collects news from external APIs, stores them in
SQLite, and serves them via a REST API to a console client.

---

## Architecture

```
ClientApplication (console)
    │ HTTP/REST (libcurl → port 9080)
    ▼
ServerApplication (Pistache REST)
    ├── Routes (admin / article / auth / notification)
    ├── Services  (thin wrappers around the DB layer)
    ├── Database  (DataBase.cpp — SQLite via DBManager singleton)
    ├── Scheduler (FetchScheduler — background thread, every 3 h)
    │       └── Adapters (TheNewsApiAdapter / NewsApiOrgAdapter)
    └── Utils     (CategoryClassifier, EmailSender, HttpClient)
```

**Default admin account** (created automatically on first run):

| Field    | Value                  |
|----------|------------------------|
| Email    | `headmaster@news.com`  |
| Password | `headmaster1223`       |

Passwords are stored as **SHA-256 hex digests** — never in plaintext.

---

## Prerequisites

| Requirement | Minimum version | Install (Ubuntu/Debian) |
|---|---|---|
| GCC / G++ | 11 | `sudo apt install g++` |
| CMake | 3.14 | `sudo apt install cmake` |
| SQLite3 dev | any | `sudo apt install libsqlite3-dev` |
| libcurl dev | 7.x | `sudo apt install libcurl4-openssl-dev` |
| OpenSSL dev | 3.x | `sudo apt install libssl-dev` |
| Pistache | ≥ 0.0.5 | `sudo apt install libpistache-dev` |
| GoogleTest | (auto-downloaded by CMake) | internet required on first test build |

---

## Setup — API Keys

API keys are stored in the database (not in source code).

1. Start the server (see below).
2. Log in as admin: `headmaster@news.com` / `headmaster1223`.
3. Choose **3 — Update Server API Key** and set keys for:
   - **TheNewsAPI** — obtain at <https://www.thenewsapi.com/>
   - **NewsAPI.org** — obtain at <https://newsapi.org/>
4. Choose **4 — Update Server Status** → set the desired server to `active`.

The news scheduler fetches automatically every 3 hours.

## Setup — JWT Authentication

The server uses HS256 JWT tokens for authentication. Set the signing secret before starting:

```bash
export JWT_SECRET="your-long-random-secret-key-here"
```

**Generating a strong secret:**
```bash
openssl rand -hex 32
```

> If `JWT_SECRET` is not set the server starts and login still works, but the
> returned token will be an empty string and all protected endpoints will return
> `401 Unauthorized`.

**Token lifetime:** 24 hours. After expiry the user must log in again.

**Protected endpoints** (require `Authorization: Bearer <token>` header):
- All `/admin/*` routes — also require `role = admin`
- `POST /articles/save`, `DELETE /articles/save`
- `GET /articles/saved/:userId` (own saves only; admins can access any)
- `POST /articles/:id/like`, `POST /articles/:id/dislike`
- `POST /articles/:id/report`
- All `/notifications/*` routes

**Public endpoints** (no token required):
- `POST /auth/login`, `POST /auth/signup`
- `GET /articles`, `GET /articles/:id`, `GET /articles/categories`
- `GET /articles/category/:cat`, `GET /articles/search`
- `GET /articles/:id/reactions`

## Setup — Email Notifications

Set these environment variables before starting the server:

```bash
export EMAIL_FROM="your@gmail.com"
export EMAIL_APP_PASSWORD="your-gmail-app-password"
# Optional (defaults to Gmail SMTP):
export EMAIL_SMTP_URL="smtp://smtp.gmail.com:587"
```

Generate a Gmail App Password at: <https://myaccount.google.com/apppasswords>

---

## Build

### Server

```bash
cd Solution/ServerApplication/ServerApplication/NewsServer_
cmake -S . -B build
cmake --build build
```

Binary: `build/NewsAggregatorServer`

### Client

```bash
cd Solution/ClientApplication
cmake -S . -B build
cmake --build build
```

Binary: `build/NewsAggregatorClient`

---

## Running

Start the **server first** (it creates the database automatically):

```bash
cd Solution/ServerApplication/ServerApplication/NewsServer_/build
./NewsAggregatorServer
# Output: Starting server at http://localhost:9080 ...
```

The database is created at `Solution/data/news9.db`.

In a **second terminal**, start the client:

```bash
cd Solution/ClientApplication/build
./NewsAggregatorClient
```

---

## Database

| Item | Value |
|---|---|
| Engine | SQLite 3 |
| File path | `Solution/data/news9.db` (relative to server build dir) |
| Schema reference | `Solution/db/schema.sql` |
| Created automatically | Yes — on every server startup |

To reset the database:
```bash
rm Solution/data/news9.db
./NewsAggregatorServer    # recreates schema + default admin
```

---

## Tests

### Build tests

**Client tests:**
```bash
cd Solution/ClientApplication/tests
cmake -S . -B build
cmake --build build
```

**Server tests** (downloads GoogleTest on first run — internet required):
```bash
cd Solution/ServerApplication/ServerApplication/NewsServer_/tests
cmake -S . -B build
cmake --build build
```

### Run all tests

```bash
# Client  (93 tests)
./Solution/ClientApplication/tests/build/ClientTests

# Server  (52 tests)
./Solution/ServerApplication/ServerApplication/NewsServer_/tests/build/NewsServerTests
```

### Run a specific test

```bash
./ClientTests --gtest_filter="ValidatorTest.*"
./NewsServerTests --gtest_filter="UserServiceTest.Login_*"
```

### Clean build (from scratch)

```bash
rm -rf Solution/ClientApplication/build
rm -rf Solution/ClientApplication/tests/build
rm -rf Solution/ServerApplication/ServerApplication/NewsServer_/build
rm -rf Solution/ServerApplication/ServerApplication/NewsServer_/tests/build
```

---

## Project Structure

```
Solution/
├── ClientApplication/          # Console client
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── core/                   # App.h / App.cpp — entry point & menu routing
│   ├── constants/              # APIEndpoints.h, Strings.h
│   ├── models/                 # Article.h, User.h, Notification.h
│   ├── services/               # HTTP-based service clients
│   ├── ui/                     # Console menus (Login, User, Admin, …)
│   ├── utils/                  # HttpClient, Validator, ConsoleUtils, Paginator
│   └── tests/                  # GoogleTest unit tests
│
├── ServerApplication/
│   └── ServerApplication/
│       └── NewsServer_/        # REST server
│           ├── CMakeLists.txt
│           ├── main.cpp
│           ├── adapters/       # INewsApiAdapter, TheNewsApiAdapter, NewsApiOrgAdapter
│           ├── database/       # DBManager (singleton), DataBase (queries)
│           ├── routes/         # Pistache route handlers
│           ├── scheduler/      # FetchScheduler (background thread)
│           ├── services/       # Thin service wrappers
│           ├── utils/          # HttpClient, EmailSender, CategoryClassifier,
│           │                   # JsonUtils, PasswordHasher, Strings
│           └── tests/          # GoogleTest integration tests
│
├── db/
│   └── schema.sql              # Reference schema (tables created automatically by server)
├── data/                       # Runtime data (news9.db created here)
├── Diagrams/                   # Draw.io architecture diagrams
└── Docs/                       # Architecture and API documentation
```

---

## Troubleshooting

| Symptom | Likely cause | Fix |
|---|---|---|
| `Failed to initialize database` | `data/` directory missing | `mkdir -p Solution/data` |
| `pistache: not found` | Pistache not installed | `sudo apt install libpistache-dev` |
| No articles fetched | API keys not configured or server marked inactive | Set keys via Admin Panel |
| Login fails after clean DB reset | Old plaintext password in DB | Delete `news9.db` and restart server |
| Email notifications not sent | `EMAIL_FROM`/`EMAIL_APP_PASSWORD` env vars not set | Set env vars before starting server |
| Client shows "Login failed" | Server not running | Start server first |
