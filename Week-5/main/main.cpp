#include "BookLibrary.h"
#include "Exceptions.h"
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
        try {
            Book* book = bookService.getBookById(id);
            return crow::response{bookToJson(*book)};
        } catch (const BookNotFoundException& ex) {
            return crow::response(404, ex.what());
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });


    CROW_ROUTE(app, "/books").methods(crow::HTTPMethod::POST)([&bookService](const crow::request& req) {
        try {
            auto body = crow::json::load(req.body);
            if (!body) return crow::response(400, "Invalid JSON");

            Book newBook(body["id"].i(), body["title"].s(), body["author"].s());
            bookService.addBook(newBook);
            return crow::response{bookToJson(newBook)};
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    CROW_ROUTE(app, "/books/<int>").methods(crow::HTTPMethod::PUT)([&bookService](const crow::request& req, int id) {
        try {
            auto body = crow::json::load(req.body);
            if (!body) return crow::response(400, "Invalid JSON");

            Book updatedBook(id, body["title"].s(), body["author"].s());
            bookService.updateBook(id, updatedBook);
            return crow::response{bookToJson(updatedBook)};
        } catch (const BookNotFoundException& ex) {
            return crow::response(404, ex.what());
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    CROW_ROUTE(app, "/books/<int>").methods(crow::HTTPMethod::DELETE)([&bookService](int id) {
        try {
            bookService.deleteBook(id);
            return crow::response(200, "Book deleted");
        } catch (const BookNotFoundException& ex) {
            return crow::response(404, ex.what());
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    app.port(18080).multithreaded().run();
}
