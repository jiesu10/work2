// logger.cpp
#include "logger.h"
#include <iostream>

Logger *Logger::instance = nullptr;
std::mutex Logger::mtx;

Logger *Logger::getInstance() {
    if (!instance) {
        std::lock_guard<std::mutex> lock(mtx);
        if (!instance) instance = new Logger();
    }
    return instance;
}

void Logger::setLogFile(const std::string &path) {
    std::lock_guard<std::mutex> lock(mtx);
    logPath = path;
    if (ofs.is_open()) ofs.close();
    std::filesystem::path p(path);
    if (p.has_parent_path()) {
        std::filesystem::create_directories(p.parent_path());
    }
    ofs.open(path, std::ios::app);
}

std::string Logger::currentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return std::string(buf);
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
    }
    return "UNKNOWN";
}

void Logger::log(LogLevel level, const std::string &msg) {
    std::lock_guard<std::mutex> lock(mtx);
    std::string line = "[" + currentTime() + "] [" + levelToString(level) + "] " + msg;
    if (level == LogLevel::ERROR) {
        std::cerr << line << "\n";
    } else {
        std::cout << line << "\n";
    }
    if (ofs.is_open()) {
        ofs << line << "\n";
        ofs.flush();
    }
}
