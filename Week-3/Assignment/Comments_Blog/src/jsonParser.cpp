#include "JsonParser.hpp"
#include <iostream>

nlohmann::json JsonParser::parseJsonResponse(const std::string& jsonResponse) {

    if (jsonResponse.compare(0, cPrefix.size(), cPrefix) != 0) {
        throw std::runtime_error(cUnexpectedResponseFormat);
    }

    //Remove Prefix
    std::string jsonData = jsonResponse.substr(cPrefix.size());
    
    //Remove Trailing Whitespace and Semicolon
    while (!jsonData.empty() && (std::isspace(jsonData.back()) || jsonData.back() == ';')) {
        jsonData.pop_back();
    }

    return nlohmann::json::parse(jsonData);
}

void JsonParser::displayBlogInfo(const nlohmann::json& tumblrData) {
    if (tumblrData.contains(cTumbleLog)) {
        const auto& tumblelog = tumblrData[cTumbleLog];
        std::cout << "Title: " << tumblelog.value(cTitle, cNotApplicable) << std::endl;
        std::cout << "Name: " << tumblelog.value(cName, cNotApplicable) << std::endl;
        std::cout << "Description: " << tumblelog.value(cDescription, cNotApplicable) << std::endl;
    } else {
        std::cerr << cBlogInformationNotFound << std::endl;
    }
}

void JsonParser::displayPosts(const nlohmann::json& tumblrData, int start) {
    if (tumblrData.contains(cPosts)) {
        const auto& posts = tumblrData[cPosts];
        int postNumber = start;
        for (const auto& post : posts) {
            std::cout << postNumber << ". ";
            if (post.contains(cPhotoUrl1280)) {
                std::cout << post[cPhotoUrl1280].get<std::string>() << std::endl;
            } else if (post.contains(cPhotoUrl500)) {
                std::cout << post[cPhotoUrl500].get<std::string>() << std::endl;
            } else {
                std::cout << cNoImageAvailable << std::endl;
            }
            ++postNumber;
        }
    } else {
        std::cerr << cNoPostsFound << std::endl;
    }
}
