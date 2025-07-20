#include "2.h"
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <curl/curl.h>

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp) {
    std::string *payload = static_cast<std::string*>(userp);
    if (payload->empty()) return 0;

    size_t copy_size = std::min(size * nmemb, payload->size());
    memcpy(ptr, payload->data(), copy_size);
    *payload = payload->substr(copy_size);
    return copy_size;
}

std::string UserServer::GenerateVerifiCode() {
    std::string code;
    code.reserve(6);
    srand((unsigned int)time(NULL));
    for (int i = 0; i < 6; ++i) {
        code.push_back((rand() % 10) + '0');
    }
    return code;
}

void UserServer::SendCodeToEmail(const std::string &code, const std::string &email) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "curl init failed!" << std::endl;
        return;
    }

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.qq.com:465");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_USERNAME, "jiaguoyangyang@qq.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "quhtahmkvwxdcbic");
    curl_easy_setopt(curl, CURLOPT_LOGIN_OPTIONS, "AUTH=LOGIN");

    std::string from = "From: <jiaguoyangyang@qq.com>\r\n";
    std::string to = "To: <" + email + ">\r\n";
    std::string type = "Content-Type: text/html;\r\n";
    std::string subject = "Subject: Chatroom Verification Code\r\n";
    std::string body = "你的验证码：<p style=\"font-size:48px; color:black;\">" + code + "</p>\r\n";

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, from.c_str());
    headers = curl_slist_append(headers, to.c_str());

    std::string payload = from + to + type + subject + "\r\n" + body;

    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "<jiaguoyangyang@qq.com>");
    struct curl_slist *recipients = NULL;
    recipients = curl_slist_append(recipients, email.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &payload);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "发送失败: " << curl_easy_strerror(res) << std::endl;
    } else {
        std::cout << "发送成功!" << std::endl;
    }

    curl_slist_free_all(recipients);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}
