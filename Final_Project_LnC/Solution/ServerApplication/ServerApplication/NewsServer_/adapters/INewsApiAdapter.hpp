#ifndef INEWSAPIADAPTER_HPP
#define INEWSAPIADAPTER_HPP

#include <nlohmann/json.hpp>
#include <string>

class INewsApiAdapter {
public:
    explicit INewsApiAdapter(std::string apiKey) : apiKey_(std::move(apiKey)) {}
    virtual ~INewsApiAdapter() = default;
    virtual nlohmann::json fetchArticles() = 0;
    virtual std::string getSourceName() const = 0;

protected:
    std::string apiKey_;
};

#endif