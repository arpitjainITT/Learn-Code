cmake_minimum_required(VERSION 3.10)
project(NewsAggregatorClientTests)

set(CMAKE_CXX_STANDARD 17)

# Basic compiler settings
if(WIN32 OR MINGW)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
    add_definitions(-Dstrcasecmp=_stricmp)
endif()

# Find required packages
find_package(GTest REQUIRED)
find_package(CURL REQUIRED)

# Include our patched header directory FIRST to override system headers
include_directories(BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Then include the rest of the directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/
    ${CMAKE_CURRENT_SOURCE_DIR}/../services/
    ${CMAKE_CURRENT_SOURCE_DIR}/../ui/
    ${CMAKE_CURRENT_SOURCE_DIR}/../models/
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/
    ${CMAKE_CURRENT_SOURCE_DIR}/../constants/
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/nlohmann/
    ${CURL_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
)

# Test source files
set(TEST_SOURCES
    # ── Utility tests ──────────────────────────────────────────────────────
    utils/ValidatorTest.cpp       # Validator::isValidEmail/Password/Username
    utils/HttpClientTest.cpp      # HttpClient behaviour on failure paths
    utils/ConsoleUtilsTest.cpp    # ConsoleUtils::clear (only safe method)
    utils/PaginatorTest.cpp       # Paginator display logic with cin redirect

    # ── Model tests ────────────────────────────────────────────────────────
    models/UserTest.cpp
    models/ArticleTest.cpp
    models/NotificationTest.cpp

    # ── Constants tests ────────────────────────────────────────────────────
    constants/StringsTest.cpp     # Key string constants are defined/non-empty
    constants/APIEndpointsTest.cpp# Endpoint URLs composed correctly
)

# Main application source files
set(APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/App.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../services/AuthService.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../services/NewsService.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../services/NotificationService.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../services/SavedArticleService.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../services/AdminService.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../ui/LoginMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../ui/UserMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../ui/AdminMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../ui/HeadlinesMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../ui/SavedArticlesMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../ui/NotificationMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../ui/SearchMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/Validator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/HttpClient.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ConsoleUtils.cpp
)

# Create test executable
add_executable(ClientTests ${TEST_SOURCES} ${APP_SOURCES})

# Simple compiler flags
target_compile_options(ClientTests PRIVATE
    -fpermissive
)

# Link libraries
target_link_libraries(ClientTests
    GTest::gtest
    GTest::gtest_main
    ${CURL_LIBRARIES}
    pthread
)

# Enable testing
enable_testing()
add_test(NAME ClientTests COMMAND ClientTests)

# Set test properties
set_tests_properties(ClientTests PROPERTIES
    TIMEOUT 300
    ENVIRONMENT "GTEST_COLOR=1"
)