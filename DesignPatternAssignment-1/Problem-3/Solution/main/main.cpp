#include "Logger.h"

int main() {
    ILogger* logger = Logger::getInstance();

    logger->logInfo("Application started.");
    logger->logDebug("Debugging in progress...");
    logger->logError("A fatal error occurred!");

    return 0;
}
