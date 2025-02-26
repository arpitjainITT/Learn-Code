#ifndef TUMBLRCLIENT_HPP
#define TUMBLRCLIENT_HPP

#include <string>

class TumblrClient {
public:
    TumblrClient(const std::string& blogName);
    std::string fetchPosts(int start, int numPosts) const;

private:
    std::string blogName;
    std::string constructApiUrl(int start, int numPosts) const;
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* s);
};

#endif 
