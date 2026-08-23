/**
 * ConsoleUtilsTest.cpp
 *
 * Only ConsoleUtils::clear() is safe to call in a test environment.
 * ConsoleUtils::pause() and getValidatedInput() both block on std::cin,
 * so they are not unit-tested here.
 */
#include <gtest/gtest.h>
#include "../../utils/ConsoleUtils.h"

TEST(ConsoleUtilsTest, Clear_DoesNotThrow) {
    EXPECT_NO_THROW(ConsoleUtils::clear());
}

TEST(ConsoleUtilsTest, Clear_CanBeCalledMultipleTimes) {
    // Repeated calls must not crash
    EXPECT_NO_THROW({
        ConsoleUtils::clear();
        ConsoleUtils::clear();
    });
}
