#include "Divisor.h"
#include <cmath>

int Divisor::countDivisors(int number) {
    if (number <= 0) return 0;
    int count = 0;
    for (int i = 1; i <= std::sqrt(number); ++i) {
        if (number % i == 0) {
            count += (number / i == i) ? 1 : 2;
        }
    }
    return count;
}

int Divisor::reverseNumber(int number) {
    int reverse = 0;
    while (number > 0) {
        reverse = reverse * 10 + (number % 10);
        number /= 10;
    }
    return reverse;
}
