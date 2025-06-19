#include "NotificationService.h"
#include "../utils/HttpClient.h"
#include "../constants/APIEndpoints.h"
#include <iostream>

void NotificationService::viewNotifications(const User& user) {
    HttpClient::get(std::string(BASE_URL) + NOTIFICATION_VIEW + "?email=" + user.getEmail());
    std::cout << "Showing notifications (mocked):\n";
    std::cout << "- Tesla stock news\n- Virat Kohli scores\n";
}

void NotificationService::configureNotifications(const User& user) {
    int choice;
    std::string payload = "{\"email\": \"" + user.getEmail() + "\", ";

    std::cout << "Enable Business? (1=Yes, 0=No): ";
    std::cin >> choice;
    payload += "\"business\": " + std::to_string(choice) + ", ";

    std::cout << "Add keyword (leave blank to skip): ";
    std::string keyword;
    std::cin.ignore(); std::getline(std::cin, keyword);
    if (!keyword.empty()) payload += "\"keyword\": \"" + keyword + "\"";

    payload += "}";
    HttpClient::post(std::string(BASE_URL) + NOTIFICATION_CONFIGURE, payload);
    std::cout << "Notification settings updated.\n";
}
