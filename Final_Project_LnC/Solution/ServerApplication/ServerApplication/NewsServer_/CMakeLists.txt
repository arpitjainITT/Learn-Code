cmake_minimum_required(VERSION 3.14)
project(NewsAggregatorServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler warnings
add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)

# Dependencies
find_package(SQLite3  REQUIRED)
find_package(CURL     REQUIRED)
find_package(OpenSSL  REQUIRED)   # for SHA-256 password hashing (PasswordHasher.hpp)

# Include directories (each on its own line; do NOT nest link_directories inside here)
include_directories(
    ${PROJECT_SOURCE_DIR}/adapters
    ${PROJECT_SOURCE_DIR}/database
    ${PROJECT_SOURCE_DIR}/routes
    ${PROJECT_SOURCE_DIR}/services
    ${PROJECT_SOURCE_DIR}/scheduler
    ${PROJECT_SOURCE_DIR}/utils
    ${PROJECT_SOURCE_DIR}/utils/nlohmann
    /usr/local/include
)

link_directories(/usr/local/lib)

# Source files
set(SOURCES
    main.cpp

    database/DBManager.cpp
    database/DataBase.cpp

    adapters/NewsApiOrgAdapter.cpp
    adapters/TheNewsApiAdapter.cpp

    routes/RouteManager.cpp
    routes/admin_routes.cpp
    routes/article_routes.cpp
    routes/auth_routes.cpp
    routes/notification_routes.cpp

    scheduler/FetchScheduler.cpp

    services/AdminService.cpp
    services/ArticleService.cpp
    services/NotificationService.cpp
    services/UserService.cpp

    utils/CategoryClassifier.cpp
    utils/EmailSender.cpp
    utils/HttpClient.cpp
)

add_executable(NewsAggregatorServer ${SOURCES})

target_link_libraries(NewsAggregatorServer
    curl
    pthread
    pistache
    SQLite::SQLite3
    OpenSSL::SSL
    OpenSSL::Crypto
)
