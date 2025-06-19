#include "ConsoleUtils.h"
#include <cstdlib>

void ConsoleUtils::clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
