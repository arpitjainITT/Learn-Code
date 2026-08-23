#include <nlohmann/json.hpp>
#include <stdexcept>
#include <iostream>
#include "NewsApiOrgAdapter.hpp"
#include "../../utils/HttpClient.hpp"
#include "../../utils/Strings.hpp"
#include "../utils/JsonUtils.hpp"

using json = nlohmann::json;

nlohmann::json NewsApiOrgAdapter::fetchArticles() {
    if (apiKey_.empty()) {
        throw std::runtime_error("NewsAPI.org API key is not configured. Update it via the Admin Panel.");
    }
    std::string apiUrl = Strings::ADAPTER_NEWSAPIORG_URL + apiKey_;

    std::cout << Strings::ADAPTER_LOG_URL << apiUrl << "\n";
    std::string response = HttpClient::get(apiUrl);

    auto data = nlohmann::json::parse(response);

    nlohmann::json formattedArticles = nlohmann::json::array();
    for (const auto& article : data["articles"]) {
        formattedArticles.push_back({
            {"uuid",        ""},
            {"title",       JsonUtils::safeGet(article, "title")},
            {"description", JsonUtils::safeGet(article, "description")},
            {"category",    ""},  // default; classifier will assign
            {"language",    Strings::ADAPTER_DEFAULT_LANGUAGE},
            {"locale",      Strings::ADAPTER_DEFAULT_LOCALE},
            {"url",         JsonUtils::safeGet(article, "url")},
            {"image_url",   JsonUtils::safeGet(article, "urlToImage")},
            {"content",     JsonUtils::safeGet(article, "content")},
            {"source",      article.contains("source") && article["source"].contains("name") && article["source"]["name"].is_string()
                ? article["source"]["name"].get<std::string>()
                : ""}
        });
    }
    return formattedArticles;
}
