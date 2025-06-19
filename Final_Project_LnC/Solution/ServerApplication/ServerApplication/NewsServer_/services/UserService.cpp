#include "UserService.hpp"
#include "../database/DBManager.hpp"
#include "../database/DataBase.hpp"
#include "../utils/nlohmann/json.hpp"
#include <sqlite3.h>

using namespace Pistache;

void UserService::signup(const Rest::Request& request, Http::ResponseWriter response) {
    try {
        auto body = nlohmann::json::parse(request.body(), nullptr, false);
        if (body.is_discarded() || !body.contains("username") || !body.contains("email") || !body.contains("password")) {
            response.send(Http::Code::Bad_Request, "Invalid JSON data");
            return;
        }

        std::string username = body["username"];
        std::string email = body["email"];
        std::string password = body["password"];
        std::string role = "user";  

        sqlite3* db = DBManager::getDB();
        const char* sql = "INSERT INTO users (username, email, password, role) VALUES (?, ?, ?, ?);";
        sqlite3_stmt* stmt;

        sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, role.c_str(), -1, SQLITE_TRANSIENT);

        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (rc != SQLITE_DONE) {
            response.send(Http::Code::Bad_Request, "User already exists or data invalid.");
        } else {
            response.send(Http::Code::Ok, "Signup successful");
        }
    } catch (const std::exception& ex) {
        response.send(Http::Code::Bad_Request, std::string("Error parsing request: ") + ex.what());
    }
}

void UserService::login(const Rest::Request& request, Http::ResponseWriter response) {
    try {
        auto body = nlohmann::json::parse(request.body(), nullptr, false);
        if (body.is_discarded() || !body.contains("email") || !body.contains("password")) {
            response.send(Http::Code::Bad_Request, "Invalid JSON data");
            return;
        }

        std::string email = body["email"];
        std::string password = body["password"];

        sqlite3* db = DBManager::getDB();
        const char* sql = "SELECT id, username, role FROM users WHERE email = ? AND password = ?;";
        sqlite3_stmt* stmt;

        sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

        int rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            sqlite3_finalize(stmt);

            nlohmann::json result = {
                {"message", "Login successful"},
                {"user_id", id},
                {"username", username},
                {"role", role}
            };

            response.send(Http::Code::Ok, result.dump(), MIME(Application, Json));
        } else {
            sqlite3_finalize(stmt);
            response.send(Http::Code::Unauthorized, "Invalid email or password");
        }
    } catch (const std::exception& ex) {
        response.send(Http::Code::Bad_Request, std::string("Error parsing request: ") + ex.what());
    }
}
