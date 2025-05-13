#include <iostream>
#include "DivisorMatcher.h"

int main() {
    int testCaseCount, number;

    std::cout << "Enter the number of times you want to run this : ";
    std::cin >> testCaseCount;

    DivisorMatcher matcher;

    while (testCaseCount--) {
        std::cout << "Enter the number: ";
        std::cin >> number;
        int result = matcher.countMatchingDivisors(number);
        std::cout << "Total Matching Divisors are : " << result << std::endl;
    }

    return 0;
}
