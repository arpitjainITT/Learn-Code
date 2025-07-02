#include "EmailSender.hpp"
#include "../services/UserService.hpp"
#include "../services/NotificationService.hpp"
#include <iostream>
#include <curl/curl.h>

using json = nlohmann::json;

void EmailSender::sendNotifications(int userId, const json& articles) {
    json preferences = NotificationService::getUserPreferences(userId);
    std::string email = "user@example.com"; // Replace with actual lookup

    std::string body = "Your personalized news:\n\n";
    for (const auto& article : articles) {
        std::string title = article.value("title", "");
        std::string desc = article.value("description", "");
        std::string category = article.value("category", "");
        std::string url = article.value("url", "");

        body += title + "\n" + desc + "\n" + category + "\n" + url + "\n\n";
    }

    sendEmail(email, "News Update", body);
}

bool EmailSender::sendEmail(const std::string& to, const std::string& subject, const std::string& body) {
    std::cout << "[EmailSender] Sending email to " << to << ":\nSubject: " << subject << "\n\n" << body << "\n";
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    std::string from = "arpit.jain@gmail.com";  
    std::string payload =
        "To: " + to + "\r\n" +
        "From: " + from + "\r\n" +
        "Subject: " + subject + "\r\n\r\n" +
        body + "\r\n";

    struct curl_slist* recipients = nullptr;
    recipients = curl_slist_append(recipients, to.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
    curl_easy_setopt(curl, CURLOPT_USERNAME, "arpit.jainl@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "euiohabvartymnko"); 
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, payload.length());

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);

    return res == CURLE_OK;
}
