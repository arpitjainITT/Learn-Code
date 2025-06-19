#include "App.h"
#include "../ui/LoginMenu.h"
#include "../utils/ConsoleUtils.h"

void App::run() {
    ConsoleUtils::clear();
    LoginMenu loginMenu;
    loginMenu.show();
}
