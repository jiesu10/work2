// ros_core.cpp
#include "ros_core.h"
#include "subscriber.h"
#include "logger.h"

ROSCore *ROSCore::instance = nullptr;
std::mutex ROSCore::mtx;

ROSCore *ROSCore::getInstance() {
    if (!instance) {
        std::lock_guard<std::mutex> lock(mtx);
        if (!instance) instance = new ROSCore();
    }
    return instance;
}

void ROSCore::checkNodeName(const std::string &name) {
    if (registeredNodeNames.count(name)) {
        throw NodeException("Duplicate node name: " + name);
    }
}

void ROSCore::checkTopic(const std::string &topic) {
    if (topic.empty()) {
        throw TopicException("Topic name cannot be empty!");
    }
}

void ROSCore::registerPublisher(NodePtr node, const std::string &topic) {
    checkTopic(topic);
    checkNodeName(node->getName());
    registeredNodeNames.insert(node->getName());
    topicPublishers[topic].insert(node->getName());
    allNodes.push_back(node);
    Logger::getInstance()->log(LogLevel::INFO,
        "Publisher " + node->getName() + " registered to topic: " + topic);
}

void ROSCore::registerSubscriber(NodePtr node, const std::string &topic) {
    checkTopic(topic);
    checkNodeName(node->getName());
    registeredNodeNames.insert(node->getName());
    topicSubscribers[topic].push_back(node);
    allNodes.push_back(node);
    Logger::getInstance()->log(LogLevel::INFO,
        "Subscriber " + node->getName() + " registered to topic: " + topic);
}

bool ROSCore::hasTopic(const std::string &topic) const {
    return topicSubscribers.count(topic) > 0;
}

void ROSCore::publish(const std::string &topic, const Message &msg) {
    if (!hasTopic(topic)) {
        Logger::getInstance()->log(LogLevel::WARN,
            "Topic not found: " + topic + ", message dropped");
        return;
    }
    auto subs = topicSubscribers[topic];
    for (auto &s : subs) {
        if (auto subPtr = std::dynamic_pointer_cast<Subscriber>(s)) {
            subPtr->onReceive(msg);
        }
    }
}

void ROSCore::shutdownAll() {
    Logger::getInstance()->log(LogLevel::INFO, "ROS core shutdown all nodes");
    for (auto &n : allNodes) {
        n->shutdown();
    }
    allNodes.clear();
    topicSubscribers.clear();
    topicPublishers.clear();
    registeredNodeNames.clear();
}
