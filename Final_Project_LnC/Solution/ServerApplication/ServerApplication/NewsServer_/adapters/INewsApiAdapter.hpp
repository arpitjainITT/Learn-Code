#ifndef INEWSAPIADAPTER_HPP
#define INEWSAPIADAPTER_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <iostream>

class INewsApiAdapter {
public:
    virtual ~INewsApiAdapter() = default;
    virtual nlohmann::json fetchArticles() = 0;
    virtual std::string getSourceName() const = 0;
};

#endif