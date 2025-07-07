#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

int main(int argc, char **argv) {
    std::cout << "Running NewsServer unit tests..." << std::endl;
    
    // Initialize Google Test
    testing::InitGoogleTest(&argc, argv);
    
    // Initialize Google Mock
    testing::InitGoogleMock(&argc, argv);
    
    // Run all tests
    return RUN_ALL_TESTS();
}