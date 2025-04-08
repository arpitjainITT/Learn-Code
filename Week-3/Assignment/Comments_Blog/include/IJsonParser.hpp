#ifndef IJSONPARSER_HPP
#define IJSONPARSER_HPP

#include "constants.hpp"
#include "json.hpp"

class IJsonParser {
public:
    virtual ~IJsonParser() = default;

    virtual nlohmann::json parseJsonResponse(const std::string& jsonResponse) = 0;
    virtual displayBlogInfo(const nlohmann::json& tumblrData) = 0;
    virtual displayPosts(const nlohmann::json& tumblrData, int start) = 0;
};

#endif