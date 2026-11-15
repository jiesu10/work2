// publisher.cpp
#include "publisher.h"
#include "ros_core.h"
#include "logger.h"

Publisher::Publisher(std::string name, std::string topic)
    : Node(std::move(name)), topicName(std::move(topic)) {}

void Publisher::init() {
    Logger::getInstance()->log(LogLevel::INFO,
        "Publisher node init: " + getName());
    ROSCore::getInstance()->registerPublisher(shared_from_this(), topicName);
}

void Publisher::shutdown() {
    Logger::getInstance()->log(LogLevel::INFO,
        "Publisher node shutdown: " + getName());
}

void Publisher::publishMsg(const std::string &text) {
    Message msg(text);
    Logger::getInstance()->log(LogLevel::INFO,
        "Publisher " + getName() + " publish -> " + topicName + " : " + text);
    ROSCore::getInstance()->publish(topicName, msg);
}

void Publisher::publishSensorMsg(double temp, double hum) {
    SensorMessage msg(temp, hum);
    Logger::getInstance()->log(LogLevel::INFO,
        "Publisher " + getName() + " publish sensor -> " + topicName +
        " : " + msg.serialize());
    ROSCore::getInstance()->publish(topicName, msg);
}
