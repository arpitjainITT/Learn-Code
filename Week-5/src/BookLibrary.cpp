#include "BookLibrary.h"
#include "Exceptions.h"

bool BookService::addBook(const Book& book) {
    books.push_back(book);
    return true;
}

Book* BookService::getBookById(int id) {
    for (auto& book : books) {
        if (book.id == id) {
            return &book;
        }
    }
    throw BookNotFoundException("Book not found with id: " + std::to_string(id));
}

std::vector<Book> BookService::getAllBooks() const {
    return books;
}

bool BookService::updateBook(int id, const Book& updatedBook) {
    for (auto& book : books) {
        if (book.id == id) {
            book = updatedBook;
            return true;
        }
    }
    throw BookNotFoundException("Cannot update. Book not found with id: " + std::to_string(id));
}

bool BookService::deleteBook(int id) {
    for (auto it = books.begin(); it != books.end(); ++it) {
        if (it->id == id) {
            books.erase(it);
            return true;
        }
    }
    throw BookNotFoundException("Cannot delete. Book not found with id: " + std::to_string(id));
}
