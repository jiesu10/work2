// subscriber.cpp
#include "subscriber.h"
#include "ros_core.h"
#include "logger.h"

Subscriber::Subscriber(std::string name, std::string topic)
    : Node(std::move(name)), topicName(std::move(topic)) {}

void Subscriber::init() {
    Logger::getInstance()->log(LogLevel::INFO,
        "Subscriber node init: " + getName());
    ROSCore::getInstance()->registerSubscriber(shared_from_this(), topicName);
}

void Subscriber::shutdown() {
    Logger::getInstance()->log(LogLevel::INFO,
        "Subscriber node shutdown: " + getName());
}

void Subscriber::onReceive(const Message &msg) {
    Logger::getInstance()->log(LogLevel::INFO,
        "Subscriber " + getName() + " recv | " + msg.getTimestamp() +
        " | " + msg.serialize());
}
