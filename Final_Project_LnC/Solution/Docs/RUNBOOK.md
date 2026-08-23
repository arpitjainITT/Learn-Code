# News Aggregator System — Developer Runbook

> **Purpose:** This document contains every command needed to configure, build,
> run, test, and maintain the News Aggregator System.  
> Follow each section in order for a first-time setup.

---

## Table of Contents

1. [System Requirements](#1-system-requirements)
2. [Install Dependencies](#2-install-dependencies)
3. [Clone and Navigate](#3-clone-and-navigate)
4. [Environment Configuration](#4-environment-configuration)
5. [Build — Server Application](#5-build--server-application)
6. [Build — Client Application](#6-build--client-application)
7. [Database Initialization](#7-database-initialization)
8. [Run the Server](#8-run-the-server)
9. [Run the Client](#9-run-the-client)
10. [Configure API Keys (Admin Setup)](#10-configure-api-keys-admin-setup)
11. [Build and Run All Tests](#11-build-and-run-all-tests)
12. [Run Specific Tests](#12-run-specific-tests)
13. [Incremental Rebuild](#13-incremental-rebuild)
14. [Clean Build from Scratch](#14-clean-build-from-scratch)
15. [Verify No Compiler Warnings](#15-verify-no-compiler-warnings)
16. [Check Git Status Before Committing](#16-check-git-status-before-committing)
17. [Database Management](#17-database-management)
18. [Troubleshooting](#18-troubleshooting)
19. [Default Credentials Reference](#19-default-credentials-reference)
20. [Environment Variables Reference](#20-environment-variables-reference)

---

## 1. System Requirements

| Item | Minimum |
|---|---|
| OS | Ubuntu 20.04+ / Debian 11+ / WSL2 |
| Compiler | GCC 11+ or Clang 13+ |
| CMake | 3.14+ |
| Internet | Required for first test build (GoogleTest auto-download) |

---

## 2. Install Dependencies

Run once on a fresh system:

```bash
sudo apt update && sudo apt install -y \
    g++ \
    cmake \
    pkg-config \
    libsqlite3-dev \
    libcurl4-openssl-dev \
    libssl-dev \
    libpistache-dev
```

Verify each dependency is present:

```bash
g++ --version
cmake --version
pkg-config --modversion libpistache
sqlite3 --version
curl --version | head -1
openssl version
```

> **Note:** If `libpistache-dev` is not available via apt, install from source:
> ```bash
> git clone https://github.com/pistacheio/pistache.git
> cd pistache && mkdir build && cd build
> cmake .. -DCMAKE_BUILD_TYPE=Release
> make -j4 && sudo make install
> ```

---

## 3. Clone and Navigate

```bash
# Navigate to the project solution directory
cd Final_Project_LnC/Solution
```

All commands in this runbook assume you are in `Final_Project_LnC/Solution/` unless otherwise noted.

---

## 4. Environment Configuration

These variables must be set **before** starting the server. Add them to your shell profile (`~/.bashrc` or `~/.zshrc`) to persist across sessions.

### 4.1 JWT Secret (Required for authentication)

```bash
# Generate a cryptographically strong random secret
export JWT_SECRET="$(openssl rand -hex 32)"

# Verify it is set
echo "JWT_SECRET is: ${JWT_SECRET:0:8}..."
```

> Without `JWT_SECRET`, the server starts but login returns an empty token
> and all protected endpoints return `401 Unauthorized`.

### 4.2 Email Notifications (Optional)

```bash
export EMAIL_FROM="youraddress@gmail.com"
export EMAIL_APP_PASSWORD="your-gmail-app-password"

# Optional — defaults to Gmail SMTP if not set
export EMAIL_SMTP_URL="smtp://smtp.gmail.com:587"
```

> Generate a Gmail App Password at: https://myaccount.google.com/apppasswords  
> Without these, the server runs normally but email notifications are disabled.

### 4.3 Persist Environment Variables

```bash
# Append to ~/.bashrc (or ~/.zshrc)
echo 'export JWT_SECRET="$(openssl rand -hex 32)"' >> ~/.bashrc
echo 'export EMAIL_FROM="youraddress@gmail.com"'   >> ~/.bashrc
echo 'export EMAIL_APP_PASSWORD="xxxx xxxx xxxx xxxx"' >> ~/.bashrc
source ~/.bashrc
```

---

## 5. Build — Server Application

```bash
cd ServerApplication/ServerApplication/NewsServer_

# Configure (first time or after CMakeLists.txt changes)
cmake -S . -B build

# Build
cmake --build build

# Verify the binary exists
ls -lh build/NewsAggregatorServer
```

**Expected output:**
```
[100%] Built target NewsAggregatorServer
-rwxrwxr-x 1 user user 1.8M Aug 23 10:00 build/NewsAggregatorServer
```

---

## 6. Build — Client Application

```bash
cd ClientApplication

# Configure
cmake -S . -B build

# Build
cmake --build build

# Verify the binary exists
ls -lh build/NewsAggregatorClient
```

---

## 7. Database Initialization

The database is created **automatically** when the server starts for the first time. No manual steps are required.

The database file is created at:
```
Final_Project_LnC/Solution/data/news9.db
```

Ensure the `data/` directory exists:
```bash
mkdir -p Final_Project_LnC/Solution/data
```

---

## 8. Run the Server

**Always start the server before the client.**

```bash
cd ServerApplication/ServerApplication/NewsServer_/build

# Set environment variables if not in shell profile
export JWT_SECRET="your-jwt-secret-here"

# Run server (stays in foreground; Ctrl+C to stop)
./NewsAggregatorServer
```

**Expected startup output:**
```
Creating admin on the start of application
[Database] Default admin already exists
Starting server at http://localhost:9080 ...
[FetchScheduler] Fetching articles...
```

To run the server in the background:
```bash
./NewsAggregatorServer &
SERVER_PID=$!
echo "Server running with PID: $SERVER_PID"
```

To stop a background server:
```bash
kill $SERVER_PID
# or
pkill NewsAggregatorServer
```

### Verify the server is running

```bash
curl -s http://localhost:9080/articles | head -c 200
```

If the server is running correctly, you will see a JSON array (empty `[]` if no articles yet, or article objects if news has been fetched).

---

## 9. Run the Client

Open a **second terminal** with the server already running:

```bash
cd ClientApplication/build
./NewsAggregatorClient
```

**Initial screen:**
```
===============================
 Welcome to News Aggregator
1. Login
2. Sign up
3. Exit
Enter choice:
```

### First-Time Login (Admin)

```
Choice: 1
Email: headmaster@news.com
Password: headmaster1223
```

> After login, the client automatically stores the JWT token and attaches it to all subsequent requests.

### Sign Up as a New User

```
Choice: 2
Username: alice
Email: alice@example.com
Password: securepassword
```

---

## 10. Configure API Keys (Admin Setup)

After logging in as admin, configure at least one external news API server so the scheduler can fetch articles.

```
Admin Menu → Option 3: Update Server API Key
Enter server ID: 1          (1 = TheNewsAPI, 2 = NewsAPI.org)
Enter new API key: YOUR_API_KEY_HERE

Admin Menu → Option 4: Update Server Status
Enter server ID: 1
Enter new status: active
```

**Obtaining API keys:**
- **TheNewsAPI:** https://www.thenewsapi.com/ — free tier available
- **NewsAPI.org:** https://newsapi.org/ — free for development (server-side blocked on free tier; TheNewsAPI recommended)

After setting the key and status to `active`, the scheduler will fetch articles at the next 3-hour interval or at the next server restart.

---

## 11. Build and Run All Tests

### Server Tests

> Requires internet on first run to download GoogleTest.

```bash
cd ServerApplication/ServerApplication/NewsServer_/tests

# Configure
cmake -S . -B build

# Build
cmake --build build

# Run all tests
./build/NewsServerTests
```

**Expected:**
```
[==========] Running 66 tests from 5 test suites.
[  PASSED  ] 66 tests.
```

### Client Tests

```bash
cd ClientApplication/tests

# Configure
cmake -S . -B build

# Build
cmake --build build

# Run all tests
./build/ClientTests
```

**Expected:**
```
[==========] Running 93 tests from 9 test suites.
[  PASSED  ] 93 tests.
```

### Run Both Test Suites (single command)

```bash
# From Solution/ directory
./ServerApplication/ServerApplication/NewsServer_/tests/build/NewsServerTests && \
./ClientApplication/tests/build/ClientTests
echo "All tests passed: $?"
```

---

## 12. Run Specific Tests

### Filter by test suite

```bash
# Server — run only JWT tests
./ServerApplication/ServerApplication/NewsServer_/tests/build/NewsServerTests \
    --gtest_filter="JwtUtilsTest.*"

# Server — run only user auth tests
./ServerApplication/ServerApplication/NewsServer_/tests/build/NewsServerTests \
    --gtest_filter="UserServiceTest.*"

# Client — run only Validator tests
./ClientApplication/tests/build/ClientTests \
    --gtest_filter="ValidatorTest.*"
```

### Filter by test name pattern

```bash
# All tests with "Login" in the name
./ServerApplication/ServerApplication/NewsServer_/tests/build/NewsServerTests \
    --gtest_filter="*Login*"

# All "Fail" tests
./ServerApplication/ServerApplication/NewsServer_/tests/build/NewsServerTests \
    --gtest_filter="*Fail*"
```

### Verbose output

```bash
./ServerApplication/ServerApplication/NewsServer_/tests/build/NewsServerTests \
    --gtest_filter="JwtUtilsTest.*" --gtest_print_time=1
```

### List all available tests without running

```bash
./ServerApplication/ServerApplication/NewsServer_/tests/build/NewsServerTests \
    --gtest_list_tests

./ClientApplication/tests/build/ClientTests \
    --gtest_list_tests
```

---

## 13. Incremental Rebuild

After modifying source files, only changed files are recompiled:

```bash
# Server
cmake --build ServerApplication/ServerApplication/NewsServer_/build

# Client
cmake --build ClientApplication/build

# Server tests
cmake --build ServerApplication/ServerApplication/NewsServer_/tests/build

# Client tests
cmake --build ClientApplication/tests/build
```

---

## 14. Clean Build from Scratch

Use this when CMakeLists.txt changes, or when the build is in an inconsistent state.

```bash
# Remove all build directories
rm -rf ServerApplication/ServerApplication/NewsServer_/build
rm -rf ServerApplication/ServerApplication/NewsServer_/tests/build
rm -rf ClientApplication/build
rm -rf ClientApplication/tests/build

# Rebuild everything
cd ServerApplication/ServerApplication/NewsServer_
cmake -S . -B build && cmake --build build

cd ../tests
cmake -S . -B build && cmake --build build

cd ../../../ClientApplication
cmake -S . -B build && cmake --build build

cd tests
cmake -S . -B build && cmake --build build
```

---

## 15. Verify No Compiler Warnings

Both CMakeLists files include `-Wall -Wextra -Wpedantic`. A clean build should produce no warnings:

```bash
cmake --build ServerApplication/ServerApplication/NewsServer_/build 2>&1 | grep -c "warning:"
# Expected output: 0

cmake --build ClientApplication/build 2>&1 | grep -c "warning:"
# Expected output: 0
```

---

## 16. Check Git Status Before Committing

```bash
cd ..   # go to Learn&Code/ (git root)

# See all changed files
git status

# See what changed (summary)
git diff --stat

# Check no secrets are in changed files
git diff | grep -E "password|secret|api_key|token" | grep "^\+" | grep -v "JWT_SECRET\|env"

# Stage all changes in the Solution directory
git add Final_Project_LnC/Solution/

# Review what will be committed
git status --short Final_Project_LnC/Solution/
```

**Never commit:**
- Real API keys (should not be in source — stored in DB)
- `JWT_SECRET` values
- Gmail credentials
- The `data/news9.db` database file
- `build/` directories

---

## 17. Database Management

### View database location
```bash
ls -lh Final_Project_LnC/Solution/data/news9.db
```

### Inspect tables
```bash
sqlite3 Final_Project_LnC/Solution/data/news9.db ".tables"
```

### Check article count
```bash
sqlite3 Final_Project_LnC/Solution/data/news9.db \
    "SELECT COUNT(*) as total_articles FROM news_article;"
```

### Check user accounts
```bash
sqlite3 Final_Project_LnC/Solution/data/news9.db \
    "SELECT id, username, email, role_id FROM user;"
```

### Check external server status
```bash
sqlite3 Final_Project_LnC/Solution/data/news9.db \
    "SELECT es.server_name, ss.type as status, es.last_accessed
     FROM external_server es
     JOIN server_status ss ON es.server_status_id = ss.id;"
```

### Reset the database (delete and restart server to recreate)
```bash
rm Final_Project_LnC/Solution/data/news9.db
# Then restart the server — schema and default admin are re-created automatically
```

> **Warning:** Resetting the database removes all users, articles, and settings.

### Apply reference schema manually (for inspection only)
```bash
sqlite3 /tmp/test.db < Final_Project_LnC/Solution/db/schema.sql
sqlite3 /tmp/test.db ".tables"
```

---

## 18. Troubleshooting

### Server does not start

**Symptom:** `Failed to initialize database.`
```bash
# Fix: create the data directory
mkdir -p Final_Project_LnC/Solution/data
# Then restart the server
```

**Symptom:** `pistache: command not found` (build error)
```bash
sudo apt install libpistache-dev
```

**Symptom:** Port 9080 already in use
```bash
# Find and kill the process using port 9080
sudo lsof -i :9080
kill <PID>
```

---

### Client cannot connect to server

**Symptom:** All service calls return empty results.
```bash
# Verify server is running
curl -s http://localhost:9080/articles
# Should return [] or article data, not connection refused

# If connection refused, start the server first
```

---

### Login returns empty token / 401 on all requests

**Symptom:** Login succeeds but protected endpoints return `401 Unauthorized`.
```bash
# JWT_SECRET is not set — verify
echo $JWT_SECRET

# Fix: set the variable and restart the server
export JWT_SECRET="$(openssl rand -hex 32)"
./NewsAggregatorServer
```

---

### No articles appear after setting API key

**Symptom:** Headlines menu shows nothing after configuring a server.

- The scheduler runs every **3 hours**. Restart the server to trigger an immediate fetch.
- Check the server console output for fetch errors.
- Verify the server status is set to `active` (Admin Menu → Option 1).
- TheNewsAPI free tier supports server-side requests. NewsAPI.org free tier blocks them.

---

### Email notifications not being sent

```bash
# Verify environment variables are set
echo $EMAIL_FROM
echo ${EMAIL_APP_PASSWORD:0:4}...

# Gmail requires an App Password, not your regular password
# Generate one at: https://myaccount.google.com/apppasswords
```

---

### Tests fail with "Failed to open in-memory SQLite database"

```bash
# Verify SQLite3 is installed
sqlite3 --version

# Reinstall if needed
sudo apt install libsqlite3-dev
cd ServerApplication/ServerApplication/NewsServer_/tests
cmake -S . -B build && cmake --build build
```

---

### Build fails with "jwt" or "openssl" errors

```bash
# Verify OpenSSL is installed
openssl version
pkg-config --libs openssl

# Reinstall if needed
sudo apt install libssl-dev

# Reconfigure and rebuild
rm -rf ServerApplication/ServerApplication/NewsServer_/build
cmake -S ServerApplication/ServerApplication/NewsServer_ -B ServerApplication/ServerApplication/NewsServer_/build
cmake --build ServerApplication/ServerApplication/NewsServer_/build
```

---

## 19. Default Credentials Reference

| Account | Email | Password | Role |
|---|---|---|---|
| Default Admin | `headmaster@news.com` | `headmaster1223` | admin |

> **Change the admin password** through the database after first login:
> ```bash
> # Compute SHA-256 hash of new password
> echo -n "newpassword" | openssl dgst -sha256
> # Then update in DB
> sqlite3 data/news9.db "UPDATE user SET password='<hash>' WHERE email='headmaster@news.com';"
> ```

---

## 20. Environment Variables Reference

| Variable | Required | Default | Description |
|---|---|---|---|
| `JWT_SECRET` | **Yes** (for auth) | None | Signing key for HS256 JWT tokens. Set to a long random string. |
| `EMAIL_FROM` | No | None | Gmail address to send notification emails from |
| `EMAIL_APP_PASSWORD` | No | None | Gmail App Password (not your login password) |
| `EMAIL_SMTP_URL` | No | `smtp://smtp.gmail.com:587` | SMTP server URL |

---

*End of Runbook*
