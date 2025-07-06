#include <nlohmann/json.hpp>
#include "NewsApiOrgAdapter.hpp"
#include "../../utils/HttpClient.hpp"
#include "../../utils/Strings.hpp"

using json = nlohmann::json;

nlohmann::json NewsApiOrgAdapter::fetchArticles() {
    std::string apiKey = Strings::ADAPTER_NEWSAPIORG_KEY;
    std::string apiUrl = Strings::ADAPTER_NEWSAPIORG_URL + apiKey;

    std::cout << Strings::ADAPTER_LOG_URL << apiUrl << "\n";
    std::string response = HttpClient::get(apiUrl);

    auto data = nlohmann::json::parse(response);

    auto safe_get = [](const json& j, const std::string& key) -> std::string {
        return (j.contains(key) && j[key].is_string()) ? j[key].get<std::string>() : "";
    };

    std::cout << Strings::ADAPTER_LOG_RECEIVED_RESPONSE << data.dump(2) << "\n";

    nlohmann::json formattedArticles = nlohmann::json::array();
    for (const auto& article : data["articles"]) {
        formattedArticles.push_back({
            {"uuid", ""},
            {"title", safe_get(article, "title")},
            {"description", safe_get(article, "description")},
            {"category", ""},  // default; classifier will assign
            {"language", Strings::ADAPTER_DEFAULT_LANGUAGE},
            {"locale", Strings::ADAPTER_DEFAULT_LOCALE},
            {"url", safe_get(article, "url")},
            {"image_url", safe_get(article, "urlToImage")},
            {"content", safe_get(article, "content")},
            {"source", article.contains("source") && article["source"].contains("name") && article["source"]["name"].is_string()
                ? article["source"]["name"].get<std::string>()
                : ""}
        });
    }
    return formattedArticles;
}
