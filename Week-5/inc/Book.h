#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book {
public:
    int id;
    std::string title;
    std::string author;

    Book() = default;
    Book(int id, const std::string& title, const std::string& author);
};

#endif
