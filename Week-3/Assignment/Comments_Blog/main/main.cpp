#include "TumblrClient.hpp"
#include "UserInputHandler.hpp"
#include "JsonParser.hpp"
#include <iostream>
#include <stdexcept>

int main() {
    try {

        std::string blogName;
        int start, end;
        
        UserInputHandler::getUserInput(blogName, start, end);

        int totalPosts = end - start + 1;

        TumblrClient client(blogName);
        std::string response = client.fetchPosts(start - 1, totalPosts);

        IJsonParser* parser = new JsonParser();
        nlohmann::json tumblrData = parser->parseJsonResponse(response);

        JsonParser::displayBlogInfo(tumblrData);
        JsonParser::displayPosts(tumblrData, start);

        delete parser;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
