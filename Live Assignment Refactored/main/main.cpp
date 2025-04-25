#include "OrderProcessor.hpp"
#include "Customer.hpp"
#include "constants.hpp"
#include "Product.hpp"
#include "OutputHandler.hpp"

int main() {
    OrderProcessor processor;

    Customer customer;
    customer.getUserInput();
    processor.addCustomer(customer);

    Product product;
    product.getUserInput();
    processor.addProduct(product);

    std::string orderId = "ORD123";
    auto future = processor.processOrder(orderId);
    bool result = future.get();
    OutputHandler::displayMessage(cOrderProcessing + std::string(result ? cSucceeded : cFailed));

    return 0;
}
