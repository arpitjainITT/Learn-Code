#include "EmailSender.hpp"
#include <curl/curl.h>

bool EmailSender::send(const std::string& to, const std::string& subject, const std::string& body) {
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
