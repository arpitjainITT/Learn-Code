#include "Product.hpp"
#include "InputHandler.hpp"
#include <iostream>

void Product::getUserInput() {
    productID = InputHandler::getStringInput(cEnter + cProduct + cID);
    productName = InputHandler::getStringInput(cEnter + cProduct + cName);
    productPrice = InputHandler::getDoubleInput(cEnter + cProduct + cPrice);
    productDescription = InputHandler::getStringInput(cEnter + cProduct + cDescription);
    productCategory = InputHandler::getStringInput(cEnter + cProduct + cCategory);
    productStock = static_cast<int>(InputHandler::getDoubleInput(cEnter + cStockQuantiry));
    isActive = InputHandler::getStringInput(cIsActive) == cYes;
}
