#ifndef CATEGORYCLASSIFIER_HPP
#define CATEGORYCLASSIFIER_HPP

#include <string>

class CategoryClassifier {
public:
    static std::string classify(const std::string& title, const std::string& content);
};

#endif
