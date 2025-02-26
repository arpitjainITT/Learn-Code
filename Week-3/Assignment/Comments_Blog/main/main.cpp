#include "TumblrClient.hpp"
#include "JsonParser.hpp"
#include <iostream>
#include <stdexcept>

int main() {
    try {

        std::string blogName;
        std::cout << "Enter the Tumblr blog name: ";
        std::getline(std::cin, blogName);

        std::string rangeInput;
        std::cout << "Enter the post range (start-end): ";
        std::getline(std::cin, rangeInput);

        size_t dashPosition = rangeInput.find('-');
        if (dashPosition == std::string::npos) {
            std::cerr << "Invalid range format. Use start-end format." << std::endl;
            return 1;
        }

        int start = std::stoi(rangeInput.substr(0, dashPosition));
        int end = std::stoi(rangeInput.substr(dashPosition + 1));
        if (start < 1 || end < start) {
            std::cerr << "Invalid range values." << std::endl;
            return 1;
        }

        int totalPosts = end - start + 1;

        TumblrClient client(blogName);
        std::string response = client.fetchPosts(start - 1, totalPosts);

        nlohmann::json tumblrData = JsonParser::parseJsonResponse(response);

        JsonParser::displayBlogInfo(tumblrData);
        JsonParser::displayPosts(tumblrData, start);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
