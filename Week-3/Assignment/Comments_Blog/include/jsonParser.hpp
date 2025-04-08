#ifndef JSONPARSER_HPP
#define JSONPARSER_HPP

#include "IJsonParser.hpp"

class JsonParser : public IJsonParser{
public:
    nlohmann::json parseJsonResponse(const std::string& jsonResponse) override;
    void displayBlogInfo(const nlohmann::json& tumblrData) override;
    void displayPosts(const nlohmann::json& tumblrData, int start) override;
};

#endif
