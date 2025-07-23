#include "EmailSender.hpp"
#include "../services/UserService.hpp"
#include "../services/NotificationService.hpp"
#include <iostream>
#include <curl/curl.h>

using json = nlohmann::json;


bool EmailSender::sendEmail(const std::string& to, const std::string& subject, const std::string& body) {
    // Use your real Gmail and App Password here
    std::string from     = "arpit9660288717@gmail.com";         // your Gmail
    std::string smtpUser = "arpit9660288717@gmail.com";         // same as from
    std::string smtpPass = "dojw npls jwux hnrb";            // generated from App Passwords
    std::string smtpUrl  = "smtp://smtp.gmail.com:587";         // Gmail SMTP URL

    std::cout << "[" << from << "]" << " Sending email to " << to << ":\nSubject: " << subject << "\n\n" << body << "\n";

    CURL* curl = curl_easy_init();
    if (!curl) return false;

    std::string payload =
        "To: " + to + "\r\n" +
        "From: " + from + "\r\n" +
        "Subject: " + subject + "\r\n\r\n" +
        body + "\r\n";

    struct curl_slist* recipients = nullptr;
    recipients = curl_slist_append(recipients, to.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, smtpUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
    curl_easy_setopt(curl, CURLOPT_USERNAME, smtpUser.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, smtpPass.c_str());
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, payload.length());

    CURLcode res = curl_easy_perform(curl);
    std::cout << "CURL result: " << curl_easy_strerror(res) << "\n";
    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);

    return res == CURLE_OK;
}
