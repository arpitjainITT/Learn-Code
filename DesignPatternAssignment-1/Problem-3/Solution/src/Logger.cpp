#include "Logger.h"
#include <iostream>
#include <stdexcept>

Logger* Logger::instance = nullptr;
std::once_flag Logger::initInstanceFlag;

Logger::Logger() {
    logFile.open("application.log", std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to open log file");
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger* Logger::getInstance() {
    std::call_once(initInstanceFlag, []() {
        instance = new Logger();
    });
    return instance;
}

void Logger::logError(const std::string& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    logFile << "[ERROR]: " << msg << std::endl;
}

void Logger::logDebug(const std::string& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    logFile << "[DEBUG]: " << msg << std::endl;
}

void Logger::logInfo(const std::string& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    logFile << "[INFO]: " << msg << std::endl;
}
