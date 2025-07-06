#ifndef STRINGS_H
#define STRINGS_H

#include <string>

namespace Strings {
    // Menu Titles
    const std::string SAVED_ARTICLES_TITLE = "==== SAVED ARTICLES ====\n";
    const std::string VIEW_SAVED_ARTICLES = "1. View Saved Articles\n";
    const std::string DELETE_ARTICLE = "2. Delete Article\n";
    const std::string BACK = "3. Back\n";
    const std::string ENTER_CHOICE = "Enter your choice: ";
    const std::string ENTER_ARTICLE_ID_TO_DELETE = "Enter Article ID to delete: ";
    const std::string NO_SAVED_ARTICLES = "No saved articles.\n";
    const std::string INVALID_CHOICE = "Invalid choice.\n";
    const std::string ARTICLE_DELETED = "Article deleted.\n";
    
    // Article Display
    const std::string ARTICLE_ID = "\nID: ";
    const std::string ARTICLE_TITLE = "Title: ";
    const std::string ARTICLE_CATEGORY = "Category: ";
    const std::string ARTICLE_SOURCE = "Source: ";
    const std::string ARTICLE_URL = "URL: ";
    const std::string ARTICLE_PUBLISHED_AT = "Published At: ";
    // Add more as needed for other menus and prompts

    // User Menu
    const std::string USER_MENU_WELCOME = "Welcome to the News Application, ";
    const std::string USER_MENU_SEPARATOR = "====================================\n";
    const std::string USER_MENU_HEADLINES = "1. Headlines\n";
    const std::string USER_MENU_SAVED_ARTICLES = "2. Saved Articles\n";
    const std::string USER_MENU_SEARCH = "3. Search\n";
    const std::string USER_MENU_NOTIFICATIONS = "4. Notifications\n";
    const std::string USER_MENU_LOGOUT = "5. Logout\n";
    const std::string USER_MENU_ENTER_CHOICE = "Enter choice: ";

    // Admin Menu
    const std::string ADMIN_MENU_TITLE = "========== ADMIN PANEL ==========\n";
    const std::string ADMIN_MENU_VIEW_SERVERS = "1. View External Servers\n";
    const std::string ADMIN_MENU_VIEW_DETAILS = "2. View Server Details\n";
    const std::string ADMIN_MENU_UPDATE_KEY = "3. Update Server API Key\n";
    const std::string ADMIN_MENU_UPDATE_STATUS = "4. Update Server Status\n";
    const std::string ADMIN_MENU_ADD_CATEGORY = "5. Add New Category\n";
    const std::string ADMIN_MENU_LOGOUT = "6. Logout\n";
    const std::string ADMIN_MENU_ENTER_CHOICE = "Enter your choice: ";
    const std::string ADMIN_MENU_ID = "ID: ";
    const std::string ADMIN_MENU_NAME = "Name: ";
    const std::string ADMIN_MENU_API_KEY = "API Key: ";
    const std::string ADMIN_MENU_STATUS = "Status: ";
    const std::string ADMIN_MENU_LAST_ACCESSED = "Last Accessed: ";
    const std::string ADMIN_MENU_ENTER_SERVER_ID = "Enter server ID: ";
    const std::string ADMIN_MENU_ENTER_NEW_KEY = "Enter new API key: ";
    const std::string ADMIN_MENU_API_KEY_UPDATED = "API key updated.\n";
    const std::string ADMIN_MENU_ENTER_NEW_STATUS = "Enter new status (active/inactive): ";
    const std::string ADMIN_MENU_STATUS_UPDATED = "Status updated.\n";
    const std::string ADMIN_MENU_ENTER_NEW_CATEGORY = "Enter new category name: ";
    const std::string ADMIN_MENU_CATEGORY_ADDED = "Category added.\n";

    // Headlines Menu
    const std::string HEADLINES_MENU_TITLE = "========== HEADLINES ==========\n";
    const std::string HEADLINES_MENU_TODAY = "1. Today\n";
    const std::string HEADLINES_MENU_DATE_RANGE = "2. Date range\n";
    const std::string HEADLINES_MENU_LOGOUT = "3. Logout\n";
    const std::string HEADLINES_MENU_ENTER_CHOICE = "Enter choice: ";
    const std::string HEADLINES_MENU_INVALID_CHOICE = "Invalid choice.\n";
    const std::string HEADLINES_TODAY_TITLE = "---- Today's Headlines ----\n";
    const std::string HEADLINES_TODAY_ALL = "1. All\n";
    const std::string HEADLINES_TODAY_BUSINESS = "2. Business\n";
    const std::string HEADLINES_TODAY_ENTERTAINMENT = "3. Entertainment\n";
    const std::string HEADLINES_TODAY_SPORTS = "4. Sports\n";
    const std::string HEADLINES_TODAY_TECHNOLOGY = "5. Technology\n";
    const std::string HEADLINES_TODAY_BACK = "6. Back\n";
    const std::string HEADLINES_ENTER_START_DATE = "Enter start date (YYYY-MM-DD): ";
    const std::string HEADLINES_ENTER_END_DATE = "Enter end date (YYYY-MM-DD): ";
    const std::string HEADLINES_NO_ARTICLES = "No articles found.\n";
    const std::string HEADLINES_SELECT_ARTICLE = "\nSelect article to view details and options (0 to go back): ";
    const std::string HEADLINES_SOURCE = "Source: ";
    const std::string HEADLINES_CATEGORY = "Category: ";
    const std::string HEADLINES_ARTICLE_OPTIONS_SAVE = "1. Save Article\n";
    const std::string HEADLINES_ARTICLE_OPTIONS_LIKE = "2. Like Article\n";
    const std::string HEADLINES_ARTICLE_OPTIONS_DISLIKE = "3. Dislike Article\n";
    const std::string HEADLINES_ARTICLE_OPTIONS_BACK = "4. Back\n";
    const std::string HEADLINES_ARTICLE_OPTIONS_ENTER_CHOICE = "Enter choice: ";
    const std::string HEADLINES_ARTICLE_SAVED = "Article saved.\n";
    const std::string HEADLINES_ARTICLE_LIKED = "Article liked.\n";
    const std::string HEADLINES_ARTICLE_DISLIKED = "Article disliked.\n";

    // Search Menu
    const std::string SEARCH_MENU_TITLE = "========== SEARCH ==========\n";
    const std::string SEARCH_MENU_SEARCH_ARTICLES = "1. Search Articles\n";
    const std::string SEARCH_MENU_BACK = "2. Back\n";
    const std::string SEARCH_MENU_ENTER_CHOICE = "Enter choice: ";
    const std::string SEARCH_MENU_ENTER_KEYWORD = "Enter keyword to search: ";
    const std::string SEARCH_MENU_ENTER_START_DATE = "Enter start date (YYYY-MM-DD): ";
    const std::string SEARCH_MENU_ENTER_END_DATE = "Enter end date (YYYY-MM-DD): ";
    const std::string SEARCH_MENU_SORT_BY = "Sort by (likes/dislikes/none): ";
    const std::string SEARCH_MENU_NO_MATCH = "No matching articles found.\n";
    const std::string SEARCH_MENU_RESULTS_TITLE = "\n=== Search Results ===\n";
    const std::string SEARCH_MENU_CATEGORY = "   Category: ";
    const std::string SEARCH_MENU_SOURCE = "   Source: ";
    const std::string SEARCH_MENU_PUBLISHED_AT = "   Published At: ";
    const std::string SEARCH_MENU_ENTER_ARTICLE_NUMBER = "\nEnter article number to save (0 to skip): ";
    const std::string SEARCH_MENU_ARTICLE_SAVED = "Article saved.\n";

    // Notification Menu
    const std::string NOTIF_MENU_TITLE = "========== NOTIFICATION MENU ==========\n";
    const std::string NOTIF_MENU_VIEW_NOTIFICATIONS = "1. View Notifications\n";
    const std::string NOTIF_MENU_VIEW_PREFERENCES = "2. View Preferences\n";
    const std::string NOTIF_MENU_CONFIGURE_CATEGORY = "3. Configure Category Notifications\n";
    const std::string NOTIF_MENU_CONFIGURE_KEYWORD = "4. Configure Keyword Notifications\n";
    const std::string NOTIF_MENU_BACK = "5. Back\n";
    const std::string NOTIF_MENU_ENTER_CHOICE = "Enter choice: ";
    const std::string NOTIF_NO_NEW = "No new notifications.\n";
    const std::string NOTIF_DELIVERED_TITLE = "=== Delivered Notifications ===\n";
    const std::string NOTIF_TITLE = "Title: ";
    const std::string NOTIF_MESSAGE = "Message: ";
    const std::string NOTIF_STATUS = "Status: ";
    const std::string NOTIF_READ = "Read";
    const std::string NOTIF_UNREAD = "Unread";
    const std::string NOTIF_MARKED_AS_READ = "(Marked as read)\n";
    const std::string NOTIF_CATEGORY_PREFS = "\n--- Category Preferences ---\n";
    const std::string NOTIF_KEYWORD_PREFS = "\n--- Keyword Preferences ---\n";
    const std::string NOTIF_ENABLED = "Enabled";
    const std::string NOTIF_DISABLED = "Disabled";
    const std::string NOTIF_ENTER_CATEGORY = "Enter category name: ";
    const std::string NOTIF_ENABLE_QUESTION = "Enable? (yes/no): ";
    const std::string NOTIF_CATEGORY_UPDATED = "Category preference updated.\n";
    const std::string NOTIF_ENTER_KEYWORD = "Enter keyword: ";
    const std::string NOTIF_KEYWORD_UPDATED = "Keyword preference updated.\n";

    // Login Menu
    const std::string LOGIN_MENU_TITLE = "=== News Aggregator ===\n";
    const std::string LOGIN_MENU_LOGIN = "1. Login\n";
    const std::string LOGIN_MENU_SIGNUP = "2. Sign Up\n";
    const std::string LOGIN_MENU_EXIT = "3. Exit\n";
    const std::string LOGIN_MENU_CHOICE = "Choice: ";
    const std::string LOGIN_MENU_ENTER_EMAIL = "Enter email: ";
    const std::string LOGIN_MENU_ENTER_PASSWORD = "Enter password: ";
    const std::string LOGIN_MENU_ENTER_USERNAME = "Enter username: ";
    const std::string LOGIN_MENU_LOGIN_FAILED = "Login failed.\n";
    const std::string LOGIN_MENU_INVALID_EMAIL = "Invalid email format.\n";
    const std::string LOGIN_MENU_INVALID_PASSWORD = "Password must be at least 6 characters.\n";
    const std::string LOGIN_MENU_SIGNUP_SUCCESS = "Signup successful! Please login.\n";
    const std::string LOGIN_MENU_SIGNUP_FAILED = "Signup failed.\n";
    const std::string LOGIN_MENU_EXITING = "Exiting...\n";
    const std::string LOGIN_MENU_INVALID_CHOICE = "Invalid choice.\n";

    // Core App
    const std::string APP_EXITING = "Exiting application. Goodbye!\n";
    const std::string APP_INVALID_CHOICE = "Invalid choice.\n";
    const std::string APP_HOME_SEPARATOR = "===============================\n";
    const std::string APP_HOME_WELCOME = " Welcome to News Aggregator\n";
    const std::string APP_HOME_LOGIN = "1. Login\n";
    const std::string APP_HOME_SIGNUP = "2. Sign up\n";
    const std::string APP_HOME_EXIT = "3. Exit\n";
    const std::string APP_HOME_ENTER_CHOICE = "Enter choice: ";
    const std::string APP_LOGIN_EMAIL = "Email: ";
    const std::string APP_LOGIN_PASSWORD = "Password: ";
    const std::string APP_LOGIN_FAILED = "Login failed. Invalid credentials.\n";
    const std::string APP_SIGNUP_USERNAME = "Username: ";
    const std::string APP_SIGNUP_EMAIL = "Email: ";
    const std::string APP_SIGNUP_PASSWORD = "Password: ";
    const std::string APP_SIGNUP_SUCCESS = "User registered successfully. Please login.\n";
    const std::string APP_SIGNUP_FAILED = "Signup failed. Email may already exist.\n";

    // Utils
    const std::string UTILS_INVALID_INPUT = "Invalid input. Try again: ";
    const std::string UTILS_PRESS_ENTER = "Press Enter to continue...";

    // HTTP Client
    const std::string HTTP_LOG_METHOD_URL = "[{}] {}\n"; 
    const std::string HTTP_LOG_BODY = "Body: {}\n";
    const std::string HTTP_LOG_AUTH = "Auth: Bearer {}\n";
    const std::string HTTP_ERROR_REQUEST_FAILED = "{} request failed: {}\n";
    const std::string HTTP_ERROR_CURL_INIT = "Failed to initialize CURL\n";
    const std::string HTTP_METHOD_GET = "GET";
    const std::string HTTP_METHOD_POST = "POST";
    const std::string HTTP_METHOD_PUT = "PUT";
    const std::string HTTP_METHOD_DELETE = "DELETE";
    const std::string HTTP_HEADER_CONTENT_TYPE_JSON = "Content-Type: application/json";
    const std::string HTTP_HEADER_AUTH_BEARER = "Authorization: Bearer ";

    // Validator
    const std::string VALIDATOR_EMAIL_REGEX = R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)";
    const int VALIDATOR_PASSWORD_MIN_LENGTH = 6;

    // Notification Service
    const std::string NOTIF_SERVICE_FETCH_PREFS_FAIL = "Failed to fetch notification preferences.\n";
    const std::string NOTIF_SERVICE_FETCH_NOTIFS_FAIL = "Failed to fetch notifications.\n";
    const std::string NOTIF_SERVICE_MARK_READ_FAIL = "Failed to mark notification as read.\n";
    const std::string NOTIF_SERVICE_DEFAULT_CATEGORY = "General";

    // News Service
    const std::string NEWS_SERVICE_PARSE_LIST_FAIL = "Failed to parse article list.\n";
    const std::string NEWS_SERVICE_SEARCH_FAIL = "Failed to search articles.\n";
    const std::string NEWS_SERVICE_DEFAULT_CATEGORY = "General";

    // Saved Article Service
    const std::string SAVED_ARTICLE_SERVICE_FETCH_FAIL = "Failed to fetch saved articles.\n";
    const std::string SAVED_ARTICLE_SERVICE_DELETE_FAIL = "Failed to delete saved article.\n";
    const std::string SAVED_ARTICLE_SERVICE_DEFAULT_CATEGORY = "General";

    // Auth Service
    const std::string AUTH_SERVICE_LOGIN_FAIL = "Login failed. Response: ";
    const std::string AUTH_SERVICE_SIGNUP_FAIL = "Signup failed. Response: ";

    // Admin Service
    const std::string ADMIN_SERVICE_FETCH_SERVERS_FAIL = "Failed to fetch servers.\n";
    const std::string ADMIN_SERVICE_FETCH_SERVER_DETAILS_FAIL = "Failed to fetch server details.\n";
}
#endif 