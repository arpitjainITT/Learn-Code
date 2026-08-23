#ifndef CONSOLEUTILS_HPP
#define CONSOLEUTILS_HPP

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <limits>

class ConsoleUtils {
public:
    static void clear();
    static int getValidatedInput(int min, int max);
    static void pause();
};

#endif
