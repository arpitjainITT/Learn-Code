#ifndef JSONPARSER_HPP
#define JSONPARSER_HPP

#include "constants.hpp"
#include "json.hpp"

class JsonParser {
public:
    static nlohmann::json parseJsonResponse(const std::string& jsonResponse);
    static void displayBlogInfo(const nlohmann::json& tumblrData);
    static void displayPosts(const nlohmann::json& tumblrData, int start);
};

#endif
