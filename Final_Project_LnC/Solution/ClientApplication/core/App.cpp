#include "App.h"
#include "../ui/LoginMenu.h"
#include "../ui/UserMenu.h"
#include "../ui/AdminMenu.h"
#include "../services/AuthService.h"
#include "../utils/ConsoleUtils.h"

#include <iostream>
#include <string>

void App::run() {
    bool running = true;
    while (running) {
        showHomeMenu();
        int choice = ConsoleUtils::getValidatedInput(1, 3);
        switch (choice) {
            case 1:
                handleLogin();
                break;
            case 2:
                handleSignup();
                break;
            case 3:
                std::cout << "Exiting application. Goodbye!\n";
                running = false;
                break;
            default:
                std::cout << "Invalid choice.\n";
        }
    }
}

void App::showHomeMenu() {
    // ConsoleUtils::clear();
    std::cout << "===============================\n";
    std::cout << " Welcome to News Aggregator\n";
    std::cout << "===============================\n";
    std::cout << "1. Login\n";
    std::cout << "2. Sign up\n";
    std::cout << "3. Exit\n";
    std::cout << "Enter choice: ";
}

void App::handleLogin() {
    std::string email, password;
    std::cout << "Email: ";
    std::cin >> email;
    std::cout << "Password: ";
    std::cin >> password;

    User user;
    if (AuthService::login(email, password, user)) {
        if (user.getRole() == "admin") {
            showAdminMenu(user);
        } else {
            showUserMenu(user);
        }
    } else {
        std::cout << "Login failed. Invalid credentials.\n";
    }
}

void App::handleSignup() {
    std::string username, email, password;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Email: ";
    std::cin >> email;
    std::cout << "Password: ";
    std::cin >> password;

    if (AuthService::signup(username, email, password)) {
        std::cout << "User registered successfully. Please login.\n";
    } else {
        std::cout << "Signup failed. Email may already exist.\n";
    }
}

void App::showAdminMenu(const User& user) {
    AdminMenu menu(user);
    menu.display();
}

void App::showUserMenu(const User& user) {
    UserMenu menu(user);
    menu.display();
}
