#include "OrderProcessor.hpp"
#include "OutputHandler.hpp"
#include <iostream>
#include <algorithm>

bool OrderProcessor::addCustomer(const Customer& customer) {
    if (customer.id.empty() || customer.email.empty()) return false;
    customers[customer.id] = customer;
    return true;
}

bool OrderProcessor::addProduct(const Product& product) {
    if (product.id.empty() || product.price < 0) return false;
    products[product.id] = product;
    return true;
}

std::future<bool> OrderProcessor::processOrder(const std::string& orderId) {
    return std::async(std::launch::async, [this, orderId]() {
        auto it = orderDatabase.find(orderId);
        if (it == orderDatabase.end()) return false;

        Order& order = it->second;
        order.status = OrderStatus::Processing;
        activeOrders.push_back(order);
        processingQueue.push_back(orderId);

        OutputHandler::sendCustomerNotification(customers[order.customerId].email, cOrder + orderId + cIsBeingProcessed);
        return true;
    });
}

std::future<bool> OrderProcessor::cancelOrder(const std::string& orderId) {
    return std::async(std::launch::async, [this, orderId]() {
        auto it = orderDatabase.find(orderId);
        if (it == orderDatabase.end()) return false;

        Order& order = it->second;
        order.status = OrderStatus::Cancelled;

        OutputHandler::sendCustomerNotification(customers[order.customerId].email, cOrder + orderId + cCancelled);
        return true;
    });
}
