#include "BookLibrary.h"
#include "../crow_all.h"

crow::json::wvalue bookToJson(const Book& book) {
    crow::json::wvalue x;
    x["id"] = book.id;
    x["title"] = book.title;
    x["author"] = book.author;
    return x;
}

int main() {
    crow::SimpleApp app;
    BookService bookService;

    CROW_ROUTE(app, "/")([] {
        return "Welcome to Arpit's Book Library API!";
    });

    CROW_ROUTE(app, "/books").methods(crow::HTTPMethod::GET)([&bookService]() {
        crow::json::wvalue result;
        auto books = bookService.getAllBooks();
        for (size_t i = 0; i < books.size(); ++i) {
            result[i] = bookToJson(books[i]);
        }
        return result;
    });

    CROW_ROUTE(app, "/books/<int>").methods(crow::HTTPMethod::GET)([&bookService](int id) {
        Book* book = bookService.getBookById(id);
        if (book != nullptr) {
            return crow::response{bookToJson(*book)};
        }
        return crow::response(404, "Book not found");
    });


    CROW_ROUTE(app, "/books").methods(crow::HTTPMethod::POST)([&bookService](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");

        Book newBook(body["id"].i(), body["title"].s(), body["author"].s());
        bookService.addBook(newBook);
        return crow::response{bookToJson(newBook)};
    });

    CROW_ROUTE(app, "/books/<int>").methods(crow::HTTPMethod::PUT)([&bookService](const crow::request& req, int id) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");

        Book updatedBook(id, body["title"].s(), body["author"].s());
        if (bookService.updateBook(id, updatedBook)) {
            return crow::response{bookToJson(updatedBook)};
        }
        return crow::response(404, "Book not found");
    });

    CROW_ROUTE(app, "/books/<int>").methods(crow::HTTPMethod::DELETE)([&bookService](int id) {
        if (bookService.deleteBook(id)) {
            return crow::response(200);
        }
        return crow::response(404, "Book not found");
    });

    app.port(18080).multithreaded().run();
}
