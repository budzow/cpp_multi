#pragma once

#include <string>
#include <mutex>

namespace io {

enum class LogLevel { DEBUG, INFO, WARNING, ERROR };

/// Thread-safe singleton logger that writes to stdout.
class Logger {
public:
    static Logger& instance();

    void setLevel(LogLevel level);
    void log(LogLevel level, const std::string& message);

    void debug(const std::string& msg);
    void info(const std::string& msg);
    void warning(const std::string& msg);
    void error(const std::string& msg);

private:
    Logger() = default;
    Logger(const Logger&)            = delete;
    Logger& operator=(const Logger&) = delete;

    LogLevel   m_level{LogLevel::INFO};
    std::mutex m_mutex;

    static std::string levelToString(LogLevel level);
    static std::string currentTimestamp();
};

} // namespace io
