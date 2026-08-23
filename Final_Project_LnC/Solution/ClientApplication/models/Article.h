#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

struct Article {
    int id;
    std::string title;
    std::string description;
    std::string category;
    std::string source;
    std::string url;
    std::string createdAt;

    Article() = default;

    Article(int id, const std::string& title, const std::string& description,
            const std::string& category, const std::string& source,
            const std::string& url, const std::string& createdAt)
        : id(id), title(title), description(description), category(category),
          source(source), url(url), createdAt(createdAt) {}
};

#endif 
