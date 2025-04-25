#ifndef ORDER_PROCESSOR_HPP
#define ORDER_PROCESSOR_HPP

#include "constants.hpp"
#include <map>
#include <vector>
#include <future>
#include "Customer.hpp"
#include "Product.hpp"
#include "Order.hpp"

class OrderProcessor {
private:
    std::map<std::string, Customer> customers;
    std::map<std::string, Product> products;
    std::map<std::string, Order> orderDatabase;
    std::vector<Order> activeOrders;
    std::vector<std::string> processingQueue;

public:
    bool addCustomer(const Customer& customer);
    bool addProduct(const Product& product);
    std::future<bool> processOrder(const std::string& orderId);
    std::future<bool> cancelOrder(const std::string& orderId);
};

#endif 
