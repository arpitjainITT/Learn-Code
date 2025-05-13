#include <gtest/gtest.h>
#include "../inc/Divisor.h"
#include "../inc/DivisorMatcher.h"


TEST(DivisorTest, CountDivisorsBasic) {
    EXPECT_EQ(Divisor::countDivisors(1), 1);
    EXPECT_EQ(Divisor::countDivisors(6), 4);   // 1, 2, 3, 6
    EXPECT_EQ(Divisor::countDivisors(10), 4);  // 1, 2, 5, 10
    EXPECT_EQ(Divisor::countDivisors(28), 6);  // 1, 2, 4, 7, 14, 28
}

TEST(DivisorTest, ReverseNumberBasic) {
    EXPECT_EQ(Divisor::reverseNumber(123), 321);
    EXPECT_EQ(Divisor::reverseNumber(100), 1);
    EXPECT_EQ(Divisor::reverseNumber(1), 1);
    EXPECT_EQ(Divisor::reverseNumber(10), 1);
}

TEST(DivisorMatcherTest, CountMatchingDivisorsSimpleCases) {
    DivisorMatcher matcher;

    EXPECT_EQ(matcher.countMatchingDivisors(0), 0);
    EXPECT_EQ(matcher.countMatchingDivisors(1), 1);  //1
    EXPECT_NE(matcher.countMatchingDivisors(2), 1);  //1,2 
    EXPECT_NE(matcher.countMatchingDivisors(3), 1);  //1,2,3
    EXPECT_EQ(matcher.countMatchingDivisors(15), 12);
    EXPECT_EQ(matcher.countMatchingDivisors(100), 36);
}
