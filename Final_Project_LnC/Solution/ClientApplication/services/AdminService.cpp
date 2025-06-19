#include "AdminService.h"
#include "../utils/HttpClient.h"
#include "../constants/APIEndpoints.h"
#include <iostream>

void AdminService::viewServerStatus() {
    std::string response = HttpClient::get(std::string(BASE_URL) + ADMIN_LIST_SOURCES);
    std::cout << "=== External Server Status ===\n";
    std::cout << "1. News API - Active - last accessed: 21 Mar 2025\n";
    std::cout << "2. The News API - Inactive - last accessed: 20 Mar 2025\n";
}

void AdminService::viewServerDetails() {
    std::string response = HttpClient::get(std::string(BASE_URL) + ADMIN_VIEW_SOURCE);
    std::cout << "=== Server Details ===\n";
    std::cout << "1. News API - <API_KEY>\n";
    std::cout << "2. The News API - <API_KEY>\n";
}

void AdminService::updateServerDetails() {
    std::string id, newKey;
    std::cout << "Enter Server ID: ";
    std::cin >> id;
    std::cout << "Enter new API Key: ";
    std::cin >> newKey;

    std::string body = "{\"id\": \"" + id + "\", \"apiKey\": \"" + newKey + "\"}";
    HttpClient::put(std::string(BASE_URL) + ADMIN_UPDATE_SOURCE + "/" + id, body);

    std::cout << "Server API Key Updated.\n";
}

void AdminService::addCategory() {
    std::string category;
    std::cout << "Enter new category name: ";
    std::cin.ignore();
    std::getline(std::cin, category);
    if (category.empty()) {
        std::cout << "Category name cannot be empty.\n";
        return;
    }
    std::string body = "{\"category\": \"" + category + "\"}";
    HttpClient::post(std::string(BASE_URL) + ADMIN_ADD_CATEGORY, body);
    std::cout << "Category added.\n";
}

