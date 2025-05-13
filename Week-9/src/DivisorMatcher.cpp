#include "DivisorMatcher.h"
#include "Divisor.h"

int DivisorMatcher::countMatchingDivisors(int number) {
    int count = 0;
    for (int i = 1; i <= number; ++i) {
        if (Divisor::countDivisors(i) == Divisor::countDivisors(Divisor::reverseNumber(i))) {
            ++count;
        }
    }
    return count;
}
