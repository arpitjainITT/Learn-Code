// Challenge: Refactor this code to improve maintainability, readability, and performance

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <chrono>
#include <iostream>
#include <future>
#include <thread>
#include <algorithm>

enum class OrderStatus {
    Pending,
    Processing,
    Shipped,
    Delivered,
    Cancelled
};

struct CustomerData {
    std::string id;
    std::string name;
    std::string email;
    std::string address;
    std::string phone;
    std::string membershipLevel;
    std::vector<void*> orderHistory;
};

struct ProductData {
    std::string id;
    std::string name;
    double price;
    std::string description;
    std::string category;
    int stock;
    bool isActive;
};

struct OrderItem {
    std::string productId;
    int quantity;
    double price;
};

struct Order {
    std::string id;
    std::string customerId;
    std::vector<OrderItem> items;
    OrderStatus status;
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point updatedAt;
    double totalAmount;
    double discountApplied;
    double finalAmount;
};

std::vector<Order> activeOrders;
std::vector<std::string> processingQueue;

class OrderProcessor {
private:
    std::map<std::string, CustomerData> customers;
    std::map<std::string, ProductData> products;
    std::map<std::string, Order> orderDatabase;

public:
    std::future<bool> processOrder(const std::string& orderId) {
        return std::async(std::launch::async, [this, orderId]() {
            std::cout << "Processing order: " << orderId << std::endl;

            auto orderIt = orderDatabase.find(orderId);
            if (orderIt == orderDatabase.end()) {
                std::cerr << "Order not found" << std::endl;
                return false;
            }

            Order& order = orderIt->second;

            auto customerIt = customers.find(order.customerId);
            if (customerIt == customers.end()) {
                std::cerr << "Customer not found" << std::endl;
                return false;
            }

            for (const auto& item : order.items) {
                auto productIt = products.find(item.productId);
                if (productIt == products.end()) {
                    std::cerr << "Product " << item.productId << " not found" << std::endl;
                    return false;
                }
                if (productIt->second.stock < item.quantity) {
                    std::cerr << "Insufficient stock for product " << item.productId << std::endl;
                    return false;
                }
            }

            for (const auto& item : order.items) {
                auto productIt = products.find(item.productId);
                if (productIt != products.end()) {
                    productIt->second.stock -= item.quantity;
                }
            }

            double discount = 0.0;
            const auto& customer = customerIt->second;
            if (customer.membershipLevel == "gold") {
                discount = 0.1;
            } else if (customer.membershipLevel == "platinum") {
                discount = 0.15;
            } else if (customer.membershipLevel == "diamond") {
                discount = 0.2;
            }

            if (order.totalAmount > 1000) {
                discount += 0.05;
            }

            order.status = OrderStatus::Processing;
            order.discountApplied = order.totalAmount * discount;
            order.finalAmount = order.totalAmount - order.discountApplied;
            order.updatedAt = std::chrono::system_clock::now();

            orderDatabase[orderId] = order;
            activeOrders.push_back(order);
            processingQueue.push_back(orderId);

            sendCustomerNotification(customer.email, "Order " + orderId + " is being processed");
            sendAdminNotification("New order processing: " + orderId);

            return true;
        });
    }

    std::future<bool> cancelOrder(const std::string& orderId) {
        return std::async(std::launch::async, [this, orderId]() {
            std::cout << "Cancelling order: " << orderId << std::endl;

            auto orderIt = orderDatabase.find(orderId);
            if (orderIt == orderDatabase.end()) {
                std::cerr << "Order not found" << std::endl;
                return false;
            }

            Order& order = orderIt->second;

            auto customerIt = customers.find(order.customerId);
            if (customerIt == customers.end()) {
                std::cerr << "Customer not found" << std::endl;
                return false;
            }

            for (const auto& item : order.items) {
                auto productIt = products.find(item.productId);
                if (productIt != products.end()) {
                    productIt->second.stock += item.quantity;
                }
            }

            order.status = OrderStatus::Cancelled;
            order.updatedAt = std::chrono::system_clock::now();

            orderDatabase[orderId] = order;
            
            activeOrders.erase(
                std::remove_if(activeOrders.begin(), activeOrders.end(),
                    [&orderId](const Order& o) { return o.id == orderId; }),
                activeOrders.end()
            );
            
            processingQueue.erase(
                std::remove(processingQueue.begin(), processingQueue.end(), orderId),
                processingQueue.end()
            );

            sendCustomerNotification(customerIt->second.email, "Order " + orderId + " has been cancelled");
            sendAdminNotification("Order cancelled: " + orderId);

            return true;
        });
    }

    void sendCustomerNotification(const std::string& email, const std::string& message) {
        try {
            std::cout << "Sending email to " << email << ": " << message << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } catch (const std::exception& e) {
            std::cerr << "Failed to send customer notification" << std::endl;
        }
    }

    void sendAdminNotification(const std::string& message) {
        try {
            std::cout << "Admin notification: " << message << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        } catch (const std::exception& e) {
            std::cerr << "Failed to send admin notification" << std::endl;
        }
    }

    bool addCustomer(const CustomerData& customer) {
        if (customer.id.empty() || customer.email.empty()) {
            return false;
        }
        customers[customer.id] = customer;
        return true;
    }

    bool addProduct(const ProductData& product) {
        if (product.id.empty() || product.price < 0) {
            return false;
        }
        products[product.id] = product;
        return true;
    }

    std::string generateOrderReport(const std::chrono::system_clock::time_point& startDate,
                                  const std::chrono::system_clock::time_point& endDate) {
        std::string report = "Order Report\n";
        int totalOrders = 0;
        double totalRevenue = 0.0;

        for (const auto& [_, order] : orderDatabase) {
            if (order.createdAt >= startDate && order.createdAt <= endDate) {
                if (order.status != OrderStatus::Cancelled) {
                    totalOrders++;
                    totalRevenue += order.finalAmount;

                    auto customerIt = customers.find(order.customerId);
                    if (customerIt != customers.end()) {
                        report += "\nOrder ID: " + order.id;
                        report += "\nCustomer: " + customerIt->second.name;
                        report += "\nAmount: $" + std::to_string(order.finalAmount);
                        report += "\nStatus: " + std::to_string(static_cast<int>(order.status));
                        report += "\n-------------------";
                    }
                }
            }
        }

        report += "\n\nTotal Orders: " + std::to_string(totalOrders);
        report += "\nTotal Revenue: $" + std::to_string(totalRevenue);
        return report;
    }
};

int main() {
    OrderProcessor processor;

    CustomerData customer{
        "CUST1",
        "John Doe",
        "john@example.com",
        "123 Main St",
        "555-0123",
        "gold",
        {}
    };
    processor.addCustomer(customer);

    ProductData product{
        "PROD1",
        "Widget",
        99.99,
        "A fantastic widget",
        "gadgets",
        100,
        true
    };
    processor.addProduct(product);

    std::string orderId = "ORD123";
    auto future = processor.processOrder(orderId);
    bool result = future.get();
    std::cout << "Order processing " << (result ? "succeeded" : "failed") << std::endl;

    return 0;
}