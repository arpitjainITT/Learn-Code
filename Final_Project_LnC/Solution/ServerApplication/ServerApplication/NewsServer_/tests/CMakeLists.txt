cmake_minimum_required(VERSION 3.14)
project(NewsServerTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------------
find_package(SQLite3  REQUIRED)
find_package(CURL     REQUIRED)
find_package(OpenSSL  REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PISTACHE REQUIRED IMPORTED_TARGET libpistache)

# GoogleTest via FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

# nlohmann/json — prefer system install, otherwise fetch
find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
  FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
  )
  FetchContent_MakeAvailable(json)
endif()

# --------------------------------------------------------------------------
# Include directories (mirrors the main project)
# --------------------------------------------------------------------------
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/nlohmann
    /usr/local/include
)

link_directories(/usr/local/lib)

# --------------------------------------------------------------------------
# Production source files (exclude main.cpp — it defines main())
# --------------------------------------------------------------------------
set(PROD_SOURCES
    ../database/DBManager.cpp
    ../database/DataBase.cpp
    ../adapters/NewsApiOrgAdapter.cpp
    ../adapters/TheNewsApiAdapter.cpp
    ../routes/RouteManager.cpp
    ../routes/admin_routes.cpp
    ../routes/article_routes.cpp
    ../routes/auth_routes.cpp
    ../routes/notification_routes.cpp
    ../scheduler/FetchScheduler.cpp
    ../services/AdminService.cpp
    ../services/ArticleService.cpp
    ../services/NotificationService.cpp
    ../services/UserService.cpp
    ../utils/CategoryClassifier.cpp
    ../utils/EmailSender.cpp
    ../utils/HttpClient.cpp
)

# --------------------------------------------------------------------------
# Test source files
# --------------------------------------------------------------------------
set(TEST_SOURCES
    # TestMain.cpp provides main() — do NOT also link gtest_main
    TestMain.cpp

    # ── Service + DB layer tests (in-memory SQLite) ─────────────────────────
    services/ArticleServiceTest.cpp  # Article CRUD, dedup, search, hide/unhide
    services/UserServiceTest.cpp     # signup, login, password hashing
    services/AdminServiceTest.cpp    # servers, categories, keywords, moderation

    # ── Utility tests ─────────────────────────────────────────────────────
    utils/CategoryClassifierTest.cpp # CategoryClassifier::classify(title, content)
    utils/JwtUtilsTest.cpp           # JWT generate, verify, tamper, wrong secret
)

# --------------------------------------------------------------------------
# Test executable
# --------------------------------------------------------------------------
add_executable(NewsServerTests ${TEST_SOURCES} ${PROD_SOURCES})

target_link_libraries(NewsServerTests
    gtest        # gtest only (TestMain.cpp provides main())
    gmock
    ${SQLite3_LIBRARIES}
    nlohmann_json::nlohmann_json
    PkgConfig::PISTACHE
    curl
    pthread
    OpenSSL::SSL
    OpenSSL::Crypto
)

# --------------------------------------------------------------------------
# Test discovery
# --------------------------------------------------------------------------
include(GoogleTest)
gtest_discover_tests(NewsServerTests)

# Copy test data if needed
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testdata DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
