#ifndef ORDER_HPP
#define ORDER_HPP

#include "constants.hpp"
#include <vector>
#include <chrono>

enum class OrderStatus {
    Pending,
    Processing,
    Shipped,
    Delivered,
    Cancelled
};

struct OrderItem {
    std::string productId;
    int quantity;
    double price;
};

class Order {
public:
    std::string id;
    std::string customerId;
    std::vector<OrderItem> items;
    OrderStatus status;
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point updatedAt;
    double totalAmount;
    double discountApplied;
    double finalAmount;

    Order(const std::string& id, const std::string& customerId, const std::vector<OrderItem>& items);
};

#endif 
