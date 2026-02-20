#include "io/logger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace io {

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

void Logger::setLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_level = level;
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (level >= m_level) {
        std::cout << "[" << currentTimestamp() << "] "
                  << "[" << levelToString(level) << "] "
                  << message << "\n";
    }
}

void Logger::debug(const std::string& msg)   { log(LogLevel::DEBUG,   msg); }
void Logger::info(const std::string& msg)    { log(LogLevel::INFO,    msg); }
void Logger::warning(const std::string& msg) { log(LogLevel::WARNING, msg); }
void Logger::error(const std::string& msg)   { log(LogLevel::ERROR,   msg); }

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO ";
        case LogLevel::WARNING: return "WARN ";
        case LogLevel::ERROR:   return "ERROR";
    }
    return "?????";
}

std::string Logger::currentTimestamp() {
    const auto now = std::chrono::system_clock::now();
    const auto t   = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    // std::localtime is not thread-safe; wrap in the mutex of the caller.
    oss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

} // namespace io
