#ifndef THENEWSAPIADAPTER_HPP
#define THENEWSAPIADAPTER_HPP

#include "INewsApiAdapter.hpp"

class TheNewsApiAdapter : public INewsApiAdapter {
public:
    nlohmann::json fetchArticles() override;
    std::string getSourceName() const override { return "thenewsapi.com"; }
};

#endif
