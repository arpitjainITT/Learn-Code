#include "JsonParser.hpp"
#include <iostream>

nlohmann::json JsonParser::parseJsonResponse(const std::string& jsonResponse) {
    const std::string prefix = "var tumblr_api_read = ";
    if (jsonResponse.compare(0, prefix.size(), prefix) != 0) {
        throw std::runtime_error("Unexpected response format.");
    }

    //Remove Prefix
    std::string jsonData = jsonResponse.substr(prefix.size());
    
    //Remove Trailing Whitespace and Semicolon
    while (!jsonData.empty() && (std::isspace(jsonData.back()) || jsonData.back() == ';')) {
        jsonData.pop_back();
    }

    return nlohmann::json::parse(jsonData);
}

void JsonParser::displayBlogInfo(const nlohmann::json& tumblrData) {
    if (tumblrData.contains("tumblelog")) {
        const auto& tumblelog = tumblrData["tumblelog"];
        std::cout << "Title: " << tumblelog.value("title", "N/A") << std::endl;
        std::cout << "Name: " << tumblelog.value("name", "N/A") << std::endl;
        std::cout << "Description: " << tumblelog.value("description", "N/A") << std::endl;
    } else {
        std::cerr << "Blog information not found." << std::endl;
    }
}

void JsonParser::displayPosts(const nlohmann::json& tumblrData, int start) {
    if (tumblrData.contains("posts")) {
        const auto& posts = tumblrData["posts"];
        int postNumber = start;
        for (const auto& post : posts) {
            std::cout << postNumber << ". ";
            if (post.contains("photo-url-1280")) {
                std::cout << post["photo-url-1280"].get<std::string>() << std::endl;
            } else if (post.contains("photo-url-500")) {
                std::cout << post["photo-url-500"].get<std::string>() << std::endl;
            } else {
                std::cout << "No image available." << std::endl;
            }
            ++postNumber;
        }
    } else {
        std::cerr << "No posts found in the specified range." << std::endl;
    }
}
