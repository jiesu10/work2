// logger.h
// 分级日志工具：INFO / WARN / ERROR，同时输出到控制台和日志文件
#ifndef ROS_SIM_LOGGER_H
#define ROS_SIM_LOGGER_H

#include <string>
#include <mutex>
#include <fstream>
#include <ctime>
#include <chrono>
#include <filesystem>

enum class LogLevel { INFO, WARN, ERROR };

class Logger {
public:
    static Logger *getInstance();
    void setLogFile(const std::string &path);
    void log(LogLevel level, const std::string &msg);

private:
    Logger() = default;
    static Logger *instance;
    static std::mutex mtx;
    std::ofstream ofs;
    std::string logPath;

    std::string levelToString(LogLevel level);
    std::string currentTime();
};

#endif // ROS_SIM_LOGGER_H
