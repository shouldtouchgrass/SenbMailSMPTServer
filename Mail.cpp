#define CURL_STATICLIB
#include <iostream>
#include <string>
#include "Curl/curl.h"

#ifdef _DEBUG
#pragma comment (lib,"Curl/libcurl_a_debug.lib")
#else
#pragma comment (lib,"Curl/libcurl_a.lib")
#endif
#pragma comment (lib,"Normaliz.lib")
#pragma comment (lib,"Ws2_32.lib")
#pragma comment (lib,"Wldap32.lib")
#pragma comment (lib,"Crypt32.lib")
#pragma comment (lib,"advapi32.lib")

size_t writeCallback(char* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(contents, totalSize);
    return totalSize;
}
void sendEmail(const std::string& from, const std::string& password, const std::string& to, const std::string& subject, const std::string& body) {
    CURL* curl;
    CURLcode res = CURLE_OK;
    std::string smtpServer = "smtps://smtp.gmail.com";
    std::string smtpPort = "465";
    std::string url = smtpServer + ":" + smtpPort;

    curl = curl_easy_init();
    if (curl) {
        // Thiết lập các thông tin cần thiết cho việc gửi email
        curl_easy_setopt(curl, CURLOPT_USERNAME, from.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from.c_str());

        struct curl_slist* recipients = NULL;
        recipients = curl_slist_append(recipients, to.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        std::string emailData = "From: " + from + "\r\n";
        emailData += "To: " + to + "\r\n";
        emailData += "Subject: " + subject + "\r\n";
        emailData += "\r\n" + body + "\r\n";
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)emailData.size());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &emailData);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

        // Gửi email
        res = curl_easy_perform(curl);

        // Kiểm tra kết quả
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Giải phóng tài nguyên
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }
}
int main() {
    std::string from = "";// your mail
    std::string password = "";// add your app password 
    std::string to = "";// mail received 
    std::string subject = "Test Email";
    std::string body = "This is a test email.";

    sendEmail(from, password, to, subject, body);

    return 0;
}
