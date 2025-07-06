#ifndef THENEWSAPIADAPTER_HPP
#define THENEWSAPIADAPTER_HPP

#include "INewsApiAdapter.hpp"
#include "../../utils/Strings.hpp"

class TheNewsApiAdapter : public INewsApiAdapter {
public:
    nlohmann::json fetchArticles() override;
    std::string getSourceName() const override { return Strings::ADAPTER_THENEWSAPI_SOURCE_NAME; }
};

#endif
