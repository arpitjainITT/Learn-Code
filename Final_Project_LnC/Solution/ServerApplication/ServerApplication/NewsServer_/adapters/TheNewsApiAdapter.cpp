#include <nlohmann/json.hpp>
#include "TheNewsApiAdapter.hpp"
#include "../../utils/HttpClient.hpp"

using json = nlohmann::json;

nlohmann::json TheNewsApiAdapter::fetchArticles() {
    std::string apiKey = "E02tf7DTesEzIvEVbtezYUADgbAn4WQd9EWXrKXY";
    std::string apiUrl = "https://api.thenewsapi.com/v1/news/top?api_token=" + apiKey + "&locale=us&limit=3";

    std::cout << "URL" << apiUrl << "\n";
    std::string response = HttpClient::get(apiUrl);

    auto data = nlohmann::json::parse(response);

    auto safe_get = [](const json& j, const std::string& key) -> std::string {
        return (j.contains(key) && j[key].is_string()) ? j[key].get<std::string>() : "";
    };

    std::cout << "[FetchScheduler] Received response: " << data.dump(2) << "\n";

    nlohmann::json formattedArticles = nlohmann::json::array();
    for (const auto& article : data["data"]) {
        formattedArticles.push_back({
            {"uuid", safe_get(article, "uuid")},
            {"title", safe_get(article, "title")},
            {"description", safe_get(article, "description")},
            {"category", article.contains("categories") && article["categories"].is_array() && !article["categories"].empty() && article["categories"][0].is_string()
                ? article["categories"][0].get<std::string>()
                : ""},
            {"language", "en"},
            {"locale", "us"},
            {"url", safe_get(article, "url")},
            {"image_url", safe_get(article, "urlToImage")},
            {"content", safe_get(article, "snippet")},
            {"source", safe_get(article, "source")}
        });
    }

    return formattedArticles;
}
