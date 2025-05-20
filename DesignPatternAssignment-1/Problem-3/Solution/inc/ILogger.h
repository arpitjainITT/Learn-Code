#ifndef ILOGGERH
#define ILOGGERH

#include <string>

class ILogger {
public:
    virtual void logError(const std::string& msg) = 0;
    virtual void logDebug(const std::string& msg) = 0;
    virtual void logInfo(const std::string& msg) = 0;

    virtual ~ILogger() = default;
};

#endif
