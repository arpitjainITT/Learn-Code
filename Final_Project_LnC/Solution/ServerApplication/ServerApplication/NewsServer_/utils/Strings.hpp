#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <string>

namespace Strings {
    // Example constants (expand as you refactor)
    const std::string SERVER_START = "Starting server at ";
    const std::string SERVER_SHUTDOWN = "Shutting down server peacefully and gracefully on your order.....\n";
    const std::string DB_INIT_FAIL = "Failed to initialize database.\n";
    const std::string CREATE_ADMIN = "Creating admin on the start of application\n";
    const int SERVER_PORT = 9080;
    const std::string DB_PATH = "../../../../data/news9.db";
    const std::string SERVER_URL = "http://localhost:";

    // DBManager
    const std::string DB_OPEN_FAIL = "Failed to open DB: ";
    const std::string DB_SCHEMA_FAIL = "Schema creation failed: ";
    
    // DataBase
    const std::string DB_FETCH_ARTICLES_FAIL = "Failed to fetch articles.\n";
    const std::string DB_FETCH_REACTION_STATS_FAIL = "Failed to fetch reaction stats.\n";
    const std::string DB_INSERT_CATEGORY_FAIL = "Failed to insert category.\n";
    const std::string DB_PREPARE_INSERT_CATEGORY_FAIL = "Failed to prepare insert category statement.\n";
    const std::string DB_PREPARE_GET_CATEGORY_ID_FAIL = "Failed to prepare getCategoryId.\n";
    const std::string DB_UPDATE_CATEGORY_PREF_FAIL = "Failed to update category preference.\n";
    const std::string DB_UPDATE_KEYWORD_PREF_FAIL = "Failed to update keyword preference.\n";
    const std::string DB_PREPARE_SEARCH_QUERY_FAIL = "[Database] Failed to prepare search query.\n";
    const std::string DB_PREPARE_NOTIFICATION_FETCH_FAIL = "[Database] Failed to prepare notification fetch query. Error: ";
    const std::string DB_INSERT_NOTIFICATION_FAIL = "[Database] Failed to insert notification for user ";
    const std::string DB_INSERTING_NOTIFICATION = "[Database] Inserting notification for userId ";
    const std::string DB_AND_ARTICLE_ID = "and articleId";
    const std::string DB_AND_MESSAGE_IS = "and the message is :";
    const std::string DB_AT_LINE = "(at line";
    const std::string DB_NEW_ARTICLE_CATEGORY = "New article in category: ";
    const std::string DB_NEW_ARTICLE_KEYWORD = "New article matched keyword: ";
    const std::string DB_DEFAULT_CATEGORY = "General";
    const std::string DB_SAVE_ARTICLE_FAIL = "Failed to save article for user\n";
    const std::string DB_PREPARE_SAVE_ARTICLE_FAIL = "Prepare failed for saveArticleForUser\n";
    const std::string DB_FETCH_SAVED_ARTICLES_FAIL = "Failed to fetch saved articles for user\n";
    const std::string DB_DELETE_SAVED_ARTICLE_FAIL = "Failed to delete saved article\n";
    const std::string DB_DELETE_SAVED_ARTICLE_SUCCESS = "Article deleted successfully for user.\n";
    const std::string DB_LIKE_ARTICLE_FAIL = "Failed to like article.\n";
    const std::string DB_DISLIKE_ARTICLE_FAIL = "Failed to dislike article.\n";
    const std::string DB_ARTICLE_INSERTED        = "[Database] Article inserted: ";
    const std::string DB_DUPLICATE_ARTICLE_SKIPPED = "[Database] Duplicate article skipped: ";
    const std::string DB_DUPLICATE_ARTICLE = "[Database] Duplicate article: ";
    const std::string DB_FAILED_INSERT_ARTICLE = "[Database] Failed to insert article: ";
    const std::string DB_FAILED_PREPARE_INSERT = "[Database] Failed to prepare insert statement.\n";
    const std::string DB_NOTIFY_CALLED = "[Database] Notify Called for category ";
    const std::string DB_FAILED_MAP_ARTICLE_CATEGORY = "[Database] Failed to map article to category.\n";
    const std::string DB_FAILED_REGISTER_USER = "[Database] Failed to register user\n";
    const std::string DB_FAILED_CREATE_ADMIN = "[Database] Failed to create admin: ";
    const std::string DB_ADMIN_CREATED = "[Database] Default admin created successfully\n";
    const std::string DB_DEFAULT_ADMIN_EXISTS = "[Database] Default admin already exists\n";
    const std::string DB_FAILED_UPDATE_EXTERNAL_SERVER_API_KEY = "[Database] Failed to update external server API key\n";
    const std::string DB_FAILED_UPDATE_SERVER_STATUS = "[Database] Failed to update server status\n";
    const std::string DB_FAILED_UPDATE_SERVER_STATUS_BY_NAME = "[Database] Failed to update server status by name\n";
    const std::string DB_VALUE_OF_EMAIL = "Value of email: ";
    const std::string DB_VALUE_OF_PASSWORD = "Value of password: ";

    // NewsFetcherManager
    const std::string NEWS_FETCHER_FETCHING = "[NewsFetcherManager] Fetching from: ";
    const std::string NEWS_FETCHER_ERROR = "[NewsFetcherManager] Error fetching from ";

    // Article Routes
    const std::string ROUTE_ARTICLE_SAVED = "Article saved.";
    const std::string ROUTE_ARTICLE_UNSAVED = "Article unsaved.";
    const std::string ROUTE_ARTICLE_LIKED = "Article liked.";
    const std::string ROUTE_ARTICLE_DISLIKED = "Article disliked.";

    // Notification Routes
    const std::string ROUTE_CATEGORY_PREF_UPDATED = "Category preference updated.";
    const std::string ROUTE_KEYWORD_PREF_UPDATED = "Keyword preference updated.";
    const std::string ROUTE_NOTIFICATIONS_MARKED_READ = "Notifications marked as read.";

// Auth Routes — valid JSON response bodies
    const std::string ROUTE_LOGIN_FAILED_LOG = "Login failed - Email: ";
    const std::string ROUTE_LOGIN_INVALID_CREDENTIALS = R"({"status": "invalid credentials"})";
    const std::string ROUTE_SIGNUP_USER_REGISTERED    = R"({"status": "user registered"})";
    const std::string ROUTE_SIGNUP_FAILED             = R"({"status": "registration failed"})";

    // JWT
    const std::string JWT_MISSING_TOKEN  = R"({"status":"unauthorized","message":"Valid Bearer token required"})";
    const std::string JWT_FORBIDDEN      = R"({"status":"forbidden","message":"Admin access required"})";
    const std::string JWT_SECRET_NOT_SET = "[JwtUtils] JWT_SECRET environment variable is not set. Set it before starting the server.\n";

    // FetchScheduler
    const std::string FETCH_SCHEDULER_FETCHING = "[FetchScheduler] Fetching articles...\n";
    const std::string FETCH_SCHEDULER_TRY_PRIMARY = "[FetchScheduler] Trying primary source: NewsAPI.org...\n";
    const std::string FETCH_SCHEDULER_PRIMARY_FETCHED = "[FetchScheduler] Articles fetched from NewsAPI.org\n";
    const std::string FETCH_SCHEDULER_STORING_ARTICLE = "[FetchScheduler] Storing article: ";
    const std::string FETCH_SCHEDULER_FROM_SOURCE = " from source: ";
    const std::string FETCH_SCHEDULER_PRIMARY_NO_ARTICLES = "[FetchScheduler] NewsAPI.org returned no articles.\n";
    const std::string FETCH_SCHEDULER_PRIMARY_FAILED = "[FetchScheduler] NewsAPI.org failed: ";
    const std::string FETCH_SCHEDULER_TRY_FALLBACK = "[FetchScheduler] Trying fallback source: TheNewsAPI.com...\n";
    const std::string FETCH_SCHEDULER_FALLBACK_FETCHED = "[FetchScheduler] Articles fetched from TheNewsAPI.com\n";
    const std::string FETCH_SCHEDULER_FALLBACK_NO_ARTICLES = "[FetchScheduler] TheNewsAPI.com also returned no articles.\n";
    const std::string FETCH_SCHEDULER_FALLBACK_FAILED = "[FetchScheduler] TheNewsAPI.com failed: ";

    // Notification Service
    const std::string NOTIF_SERVICE_FAILED_RESOLVE_CATEGORY = "[NotificationService] Failed to resolve category: ";

    // Admin Service
    const std::string ADMIN_SERVICE_FAILED_UPDATE_API_KEY = "[AdminService] Failed to update API key.\n";
    const std::string ADMIN_SERVICE_FAILED_UPDATE_STATUS = "[AdminService] Failed to update server status.\n";
    const std::string ADMIN_SERVICE_FAILED_UPDATE_STATUS_BY_NAME = "[AdminService] Failed to update server status by name.\n";
    const std::string ADMIN_SERVICE_FAILED_ADD_CATEGORY = "[AdminService] Failed to add category.\n";

    // HttpClient
    const std::string HTTP_CLIENT_USER_AGENT = "User-Agent: NewsAggregatorClient/1.0";

    // Adapters — API keys are stored in the database (external_server table).
    // Set them via the Admin Panel; do NOT hardcode credentials here.
    const std::string ADAPTER_THENEWSAPI_URL = "https://api.thenewsapi.com/v1/news/top?api_token=";
    const std::string ADAPTER_THENEWSAPI_PARAMS = "&locale=us&limit=3";
    const std::string ADAPTER_THENEWSAPI_SOURCE_NAME = "TheNewsAPI";

    const std::string ADAPTER_NEWSAPIORG_URL = "https://newsapi.org/v2/top-headlines?country=us&apiKey=";
    const std::string ADAPTER_NEWSAPIORG_SOURCE_NAME = "NewsAPI.org";
    
    const std::string ADAPTER_LOG_URL = "URL";
    const std::string ADAPTER_LOG_RECEIVED_RESPONSE = "[FetchScheduler] Received response: ";
    
    const std::string ADAPTER_DEFAULT_LANGUAGE = "en";
    const std::string ADAPTER_DEFAULT_LOCALE = "us";
    
}

#endif // STRINGS_HPP 