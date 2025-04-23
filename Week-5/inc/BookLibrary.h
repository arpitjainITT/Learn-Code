#ifndef BOOKLIBRARY_H
#define BOOKLIBRARY_H

#include "Book.h"
#include <vector>
#include <optional>

class BookService {
public:
    bool addBook(const Book& book);
    Book* getBookById(int id);
    std::vector<Book> getAllBooks() const;
    bool updateBook(int id, const Book& updatedBook);
    bool deleteBook(int id);

private:
    std::vector<Book> books;
};

#endif
