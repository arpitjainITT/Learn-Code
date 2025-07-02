#ifndef NEWSAPIORGADAPTER_HPP
#define NEWSAPIORGADAPTER_HPP

#include "INewsApiAdapter.hpp"

class NewsApiOrgAdapter : public INewsApiAdapter {
public:
    nlohmann::json fetchArticles() override;
    std::string getSourceName() const override { return "newsapi.org"; }
};

#endif
