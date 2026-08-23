#include "EmailSender.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <curl/curl.h>

// Read callback for libcurl SMTP — required for correct email delivery via UPLOAD mode.
struct EmailPayload {
    std::string data;
    size_t      offset = 0;
};

static size_t emailReadCallback(char* buf, size_t size, size_t nitems, void* userp) {
    auto* payload  = static_cast<EmailPayload*>(userp);
    size_t available = payload->data.size() - payload->offset;
    size_t toSend    = std::min(size * nitems, available);
    if (toSend == 0) return 0;
    std::memcpy(buf, payload->data.data() + payload->offset, toSend);
    payload->offset += toSend;
    return toSend;
}

bool EmailSender::sendEmail(const std::string& to,
                             const std::string& subject,
                             const std::string& body) {
    // Credentials are read from environment variables — never hardcode them in source.
    const char* envFrom = std::getenv("EMAIL_FROM");
    const char* envPass = std::getenv("EMAIL_APP_PASSWORD");
    const char* envSmtp = std::getenv("EMAIL_SMTP_URL");

    if (!envFrom || !envPass) {
        std::cerr << "[EmailSender] EMAIL_FROM and EMAIL_APP_PASSWORD environment variables "
                     "must be set to send email notifications.\n";
        return false;
    }

    std::string from    = envFrom;
    std::string pass    = envPass;
    std::string smtpUrl = envSmtp ? envSmtp : "smtp://smtp.gmail.com:587";

    // Build RFC-2822 message
    EmailPayload payload;
    payload.data =
        "To: "      + to      + "\r\n"
        "From: "    + from    + "\r\n"
        "Subject: " + subject + "\r\n"
        "\r\n"
        + body + "\r\n";

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "[EmailSender] curl_easy_init() failed.\n";
        return false;
    }

    struct curl_slist* recipients = nullptr;
    recipients = curl_slist_append(recipients, to.c_str());

    curl_easy_setopt(curl, CURLOPT_URL,           smtpUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM,     ("<" + from + ">").c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT,     recipients);
    curl_easy_setopt(curl, CURLOPT_USERNAME,      from.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD,      pass.c_str());
    curl_easy_setopt(curl, CURLOPT_USE_SSL,       (long)CURLUSESSL_ALL);
    // UPLOAD mode is required for SMTP — use READFUNCTION, not POSTFIELDS
    curl_easy_setopt(curl, CURLOPT_READFUNCTION,  emailReadCallback);
    curl_easy_setopt(curl, CURLOPT_READDATA,      &payload);
    curl_easy_setopt(curl, CURLOPT_UPLOAD,        1L);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE,    (long)payload.data.size());

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "[EmailSender] Failed to send email to " << to
                  << ": " << curl_easy_strerror(res) << "\n";
    }

    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);
    return res == CURLE_OK;
}

