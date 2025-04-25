#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include "constants.hpp"

class Product {
public:
    std::string productID;
    std::string productName;
    double productPrice;
    std::string productDescription;
    std::string productCategory;
    int productStock;
    bool isActive;

    Product() = default;
    void getUserInput();  
};

#endif 
