#include <nlohmann/json.hpp>
#include <stdexcept>
#include <iostream>
#include "TheNewsApiAdapter.hpp"
#include "../../utils/HttpClient.hpp"
#include "../../utils/Strings.hpp"
#include "../utils/JsonUtils.hpp"

using json = nlohmann::json;

nlohmann::json TheNewsApiAdapter::fetchArticles() {
    if (apiKey_.empty()) {
        throw std::runtime_error("TheNewsAPI API key is not configured. Update it via the Admin Panel.");
    }
    std::string apiUrl = Strings::ADAPTER_THENEWSAPI_URL + apiKey_ + Strings::ADAPTER_THENEWSAPI_PARAMS;

    std::cout << Strings::ADAPTER_LOG_URL << apiUrl << "\n";
    std::string response = HttpClient::get(apiUrl);

    auto data = nlohmann::json::parse(response);

    nlohmann::json formattedArticles = nlohmann::json::array();
    for (const auto& article : data["data"]) {
        formattedArticles.push_back({
            {"uuid",        JsonUtils::safeGet(article, "uuid")},
            {"title",       JsonUtils::safeGet(article, "title")},
            {"description", JsonUtils::safeGet(article, "description")},
            {"category",    article.contains("categories") && article["categories"].is_array() && !article["categories"].empty() && article["categories"][0].is_string()
                ? article["categories"][0].get<std::string>()
                : ""},
            {"language",    Strings::ADAPTER_DEFAULT_LANGUAGE},
            {"locale",      Strings::ADAPTER_DEFAULT_LOCALE},
            {"url",         JsonUtils::safeGet(article, "url")},
            {"image_url",   JsonUtils::safeGet(article, "image_url")},
            {"content",     JsonUtils::safeGet(article, "snippet")},
            {"source",      JsonUtils::safeGet(article, "source")}
        });
    }

    return formattedArticles;
}
