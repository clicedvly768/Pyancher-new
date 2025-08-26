
#pragma once
#include <string>
#include <curl/curl.h>

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
    std::string downloadFile(const std::string& url);
    bool downloadFileToDisk(const std::string& url, const std::string& outputPath);
};
