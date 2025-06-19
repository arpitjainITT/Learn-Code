#include "ArticleService.hpp"
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <sstream>
#include "../database/DBManager.hpp"
#include "../database/Database.hpp"
#include "../utils/CategoryClassifier.hpp"

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void ArticleService::getAllArticles(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    try {
        json articles = Database::getAllArticles();
        response.send(Pistache::Http::Code::Ok, articles.dump());
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, std::string("Error fetching articles: ") + e.what());
    }
}

void ArticleService::getArticlesByCategory(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    try {
        auto category = request.param(":category").as<std::string>();
        json articles = Database::getArticlesByCategory(category);
        response.send(Pistache::Http::Code::Ok, articles.dump());
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, std::string("Error fetching articles by category: ") + e.what());
    }
}

void ArticleService::getArticleById(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    try {
        int articleId = std::stoi(request.param(":id").as<std::string>());
        json article = Database::getArticleById(articleId);
        if (article.is_null()) {
            response.send(Pistache::Http::Code::Not_Found, "Article not found");
        } else {
            response.send(Pistache::Http::Code::Ok, article.dump());
        }
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, std::string("Error fetching article: ") + e.what());
    }
}

void ArticleService::saveArticle(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    try {
        auto body = json::parse(request.body(), nullptr, false);
        if (body.is_discarded() || !body.contains("user_id") || !body.contains("article_id")) {
            response.send(Pistache::Http::Code::Bad_Request, "Invalid JSON data");
        }

        int userId = body["user_id"];
        int articleId = body["article_id"];

        Database::saveArticleForUser(userId, articleId);

        std::string message = "Article " + std::to_string(articleId) + " saved for user " + std::to_string(userId);
        response.send(Pistache::Http::Code::Ok, message);
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, std::string("Error saving article: ") + e.what());
    }
}

void ArticleService::getSavedArticles(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    try {
        auto userIdStr = request.query().get("user_id");
        if (!userIdStr.has_value()) {
            response.send(Pistache::Http::Code::Bad_Request, "Missing user_id parameter");
        }

        int userId = std::stoi(userIdStr.value());

        json saved = Database::getSavedArticlesForUser(userId);

        response.send(Pistache::Http::Code::Ok, saved.dump());
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Bad_Request, std::string("Error fetching saved articles: ") + e.what());
    }
}

void ArticleService::deleteSavedArticle(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    try {
        int savedId = std::stoi(request.param(":id").as<std::string>());
        Database::deleteSavedArticle(savedId);
        std::string message = "Deleted saved article with ID " + std::to_string(savedId);
        response.send(Pistache::Http::Code::Ok, message);
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Bad_Request, std::string("Error deleting saved article: ") + e.what());
    }
}

void ArticleService::fetchAndStore() {
    try {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        std::string apiKey = "cf2275c8dedd4fef9c4b5c49d2bd09bb";
        std::string url = "https://newsapi.org/v2/top-headlines?country=us&apiKey=" + apiKey;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl) {
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "User-Agent: NewsAggregator/1.0");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if (res != CURLE_OK) {
                throw std::runtime_error("CURL fetch failed: " + std::string(curl_easy_strerror(res)));
            }

            json response = json::parse(readBuffer);
            std::cout << "[FetchScheduler] API Response: " << readBuffer << std::endl;
            if (!response.contains("articles")) {
                throw std::runtime_error("Invalid response format: 'articles' key missing");
            }

            auto safe_get = [](const json& j, const std::string& key) -> std::string {
                return (j.contains(key) && j[key].is_string()) ? j[key].get<std::string>() : "";
            };

            for (const auto& item : response["articles"]) {
                std::string title = safe_get(item, "title");
                std::string content = safe_get(item, "content");

                std::string category = CategoryClassifier::classify(title, content);

                json article = {
                    {"title", title},
                    {"content", content},
                    {"source", item.contains("source") && item["source"].contains("name") && item["source"]["name"].is_string() ? item["source"]["name"].get<std::string>() : ""},
                    {"url", safe_get(item, "url")},
                    {"category", category},
                    {"published_at", safe_get(item, "publishedAt")}
                };

                Database::storeArticle(article);
            }

            std::cout << "[FetchScheduler] Articles fetched and stored successfully.\n";
        }

        curl_global_cleanup();
    } catch (const std::exception& e) {
        std::cerr << "[FetchScheduler] Failed to fetch/store articles: " << e.what() << "\n";
    }
}

