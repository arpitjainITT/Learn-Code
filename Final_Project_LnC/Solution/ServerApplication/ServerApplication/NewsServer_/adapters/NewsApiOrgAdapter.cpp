#include <nlohmann/json.hpp>
#include "NewsApiOrgAdapter.hpp"
#include "../../utils/HttpClient.hpp"

using json = nlohmann::json;

nlohmann::json NewsApiOrgAdapter::fetchArticles() {
    std::string apiKey = "cf2275c8dedd4fef9c4b5c49d2bd09bb";
    std::string apiUrl = "https://newsapi.org/v2/top-headlines?country=us&apiKey=" + apiKey;

    std::cout << "URL" << apiUrl << "\n";
    std::string response = HttpClient::get(apiUrl);

    auto data = nlohmann::json::parse(response);

    auto safe_get = [](const json& j, const std::string& key) -> std::string {
        return (j.contains(key) && j[key].is_string()) ? j[key].get<std::string>() : "";
    };

    std::cout << "[FetchScheduler] Received response: " << data.dump(2) << "\n";

    nlohmann::json formattedArticles = nlohmann::json::array();
    for (const auto& article : data["articles"]) {
        formattedArticles.push_back({
            {"uuid", ""},
            {"title", safe_get(article, "title")},
            {"description", safe_get(article, "description")},
            {"category", ""},  // default; classifier will assign
            {"language", "en"},
            {"locale", "us"},
            {"url", safe_get(article, "url")},
            {"image_url", safe_get(article, "urlToImage")},
            {"content", safe_get(article, "content")},
            {"source", article.contains("source") && article["source"].contains("name") && article["source"]["name"].is_string()
                ? article["source"]["name"].get<std::string>()
                : ""}
        });
    }
    // std::cout << "[FetchScheduler] Storing article: " << formattedArticles["title"] << " and the [FetchScheduler] Storing source name as " << formattedArticles["source"] << "\n";
    return formattedArticles;
}
