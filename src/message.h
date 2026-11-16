#define _CRT_SECURE_NO_WARNINGS
// message.h
// 消息封装：基础字符串消息 + 温湿度复合消息，自动带时间戳
#ifndef ROS_SIM_MESSAGE_H
#define ROS_SIM_MESSAGE_H

#include <string>
#include <ctime>
#include <chrono>
#include "exceptions.h"

class Message {
private:
    std::string content;
    std::string timestamp;

    static std::string getTimeStr() {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        char buf[64];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
        return std::string(buf);
    }

public:
    explicit Message(std::string cnt)
        : content(std::move(cnt)), timestamp(getTimeStr()) {
        if (content.empty()) {
            throw MessageException("Message content cannot be empty!");
        }
    }

    virtual ~Message() = default;

    std::string getContent() const { return content; }
    std::string getTimestamp() const { return timestamp; }
    virtual std::string serialize() const { return content; }
};

// 复合消息：温度 + 湿度（加分项）
class SensorMessage : public Message {
private:
    double temperature;
    double humidity;

public:
    SensorMessage(double temp, double hum)
        : Message("sensor"), temperature(temp), humidity(hum) {}

    double getTemperature() const { return temperature; }
    double getHumidity() const { return humidity; }

    std::string serialize() const override {
        return "Temperature: " + std::to_string(temperature) +
               " C, Humidity: " + std::to_string(humidity) + " %";
    }
};

#endif // ROS_SIM_MESSAGE_H
