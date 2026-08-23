#ifndef NEWSAPIORGADAPTER_HPP
#define NEWSAPIORGADAPTER_HPP

#include "INewsApiAdapter.hpp"
#include "../../utils/Strings.hpp"

class NewsApiOrgAdapter : public INewsApiAdapter {
public:
    explicit NewsApiOrgAdapter(std::string apiKey)
        : INewsApiAdapter(std::move(apiKey)) {}
    nlohmann::json fetchArticles() override;
    std::string getSourceName() const override { return Strings::ADAPTER_NEWSAPIORG_SOURCE_NAME; }
};

#endif
