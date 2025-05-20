#ifndef LOGGERH
#define LOGGERH

#include "ILogger.h"
#include <fstream>
#include <mutex>

class Logger : public ILogger {
public:
    static Logger* getInstance();

    void logError(const std::string& msg) override;
    void logDebug(const std::string& msg) override;
    void logInfo(const std::string& msg) override;

private:
    Logger();                              
    ~Logger();                            
    Logger(const Logger&) = delete;        
    Logger& operator=(const Logger&) = delete;

    static Logger* instance;
    static std::once_flag initInstanceFlag;

    std::ofstream logFile;
    std::mutex mutex_;
};


#endif