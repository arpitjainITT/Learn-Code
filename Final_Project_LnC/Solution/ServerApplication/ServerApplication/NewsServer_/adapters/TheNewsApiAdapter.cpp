#include <nlohmann/json.hpp>
#include "TheNewsApiAdapter.hpp"
#include "../../utils/HttpClient.hpp"
#include "../../utils/Strings.hpp"

using json = nlohmann::json;

nlohmann::json TheNewsApiAdapter::fetchArticles() {
    std::string apiKey = Strings::ADAPTER_THENEWSAPI_KEY;
    std::string apiUrl = Strings::ADAPTER_THENEWSAPI_URL + apiKey + Strings::ADAPTER_THENEWSAPI_PARAMS;

    std::cout << Strings::ADAPTER_LOG_URL << apiUrl << "\n";
    std::string response = HttpClient::get(apiUrl);

    auto data = nlohmann::json::parse(response);

    auto safe_get = [](const json& j, const std::string& key) -> std::string {
        return (j.contains(key) && j[key].is_string()) ? j[key].get<std::string>() : "";
    };

    std::cout << Strings::ADAPTER_LOG_RECEIVED_RESPONSE << data.dump(2) << "\n";

    nlohmann::json formattedArticles = nlohmann::json::array();
    for (const auto& article : data["data"]) {
        formattedArticles.push_back({
            {"uuid", safe_get(article, "uuid")},
            {"title", safe_get(article, "title")},
            {"description", safe_get(article, "description")},
            {"category", article.contains("categories") && article["categories"].is_array() && !article["categories"].empty() && article["categories"][0].is_string()
                ? article["categories"][0].get<std::string>()
                : ""},
            {"language", Strings::ADAPTER_DEFAULT_LANGUAGE},
            {"locale", Strings::ADAPTER_DEFAULT_LOCALE},
            {"url", safe_get(article, "url")},
            {"image_url", safe_get(article, "urlToImage")},
            {"content", safe_get(article, "snippet")},
            {"source", safe_get(article, "source")}
        });
    }

    return formattedArticles;
}
