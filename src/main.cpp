// main.cpp
// 主程序：演示多发布者、多订阅者、多话题、复合消息、异常处理
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include "ros_core.h"
#include "publisher.h"
#include "subscriber.h"
#include "logger.h"
#include "exceptions.h"

void demoNormalFlow();
void demoSensorMessage();
void demoExceptionCases();

int main() {
    Logger::getInstance()->setLogFile("logs/ros_sim.log");
    Logger::getInstance()->log(LogLevel::INFO, "===== ROS Sim Start =====");

    try {
        // 场景 1：基础字符串消息，一个发布者两个订阅者
        demoNormalFlow();

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        ROSCore::getInstance()->shutdownAll();

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        // 场景 2：温湿度复合消息，两个话题
        demoSensorMessage();

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        ROSCore::getInstance()->shutdownAll();

        // 场景 3：异常边界测试（不崩溃）
        demoExceptionCases();

    } catch (const NodeException &e) {
        Logger::getInstance()->log(LogLevel::ERROR, std::string("NodeException: ") + e.what());
    } catch (const TopicException &e) {
        Logger::getInstance()->log(LogLevel::ERROR, std::string("TopicException: ") + e.what());
    } catch (const MessageException &e) {
        Logger::getInstance()->log(LogLevel::ERROR, std::string("MessageException: ") + e.what());
    } catch (const std::exception &e) {
        Logger::getInstance()->log(LogLevel::ERROR, std::string("Unknown exception: ") + e.what());
    }

    Logger::getInstance()->log(LogLevel::INFO, "===== ROS Sim Exit =====");
    return 0;
}

void demoNormalFlow() {
    Logger::getInstance()->log(LogLevel::INFO, "--- Scenario 1: basic string messages ---");
    auto pub = std::make_shared<Publisher>("pub_sensor", "/sensor/temp");
    auto sub1 = std::make_shared<Subscriber>("sub_display", "/sensor/temp");
    auto sub2 = std::make_shared<Subscriber>("sub_logger", "/sensor/temp");

    pub->init();
    sub1->init();
    sub2->init();

    pub->publishMsg("Temperature: 25.6 C");
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    pub->publishMsg("Temperature: 26.1 C");
}

void demoSensorMessage() {
    Logger::getInstance()->log(LogLevel::INFO, "--- Scenario 2: sensor composite messages ---");
    auto pubEnv = std::make_shared<Publisher>("pub_env", "/env/data");
    auto subMonitor = std::make_shared<Subscriber>("sub_monitor", "/env/data");
    auto subAlert = std::make_shared<Subscriber>("sub_alert", "/env/data");

    pubEnv->init();
    subMonitor->init();
    subAlert->init();

    pubEnv->publishSensorMsg(24.5, 58.0);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    pubEnv->publishSensorMsg(27.3, 63.5);
}

void demoExceptionCases() {
    Logger::getInstance()->log(LogLevel::INFO, "--- Scenario 3: exception edge cases ---");

    // 1. 空节点名
    try {
        auto bad = std::make_shared<Publisher>("", "/test");
    } catch (const NodeException &e) {
        Logger::getInstance()->log(LogLevel::ERROR, std::string("Caught: ") + e.what());
    }

    // 2. 重复节点名
    try {
        auto p1 = std::make_shared<Publisher>("dup_node", "/a");
        auto p2 = std::make_shared<Publisher>("dup_node", "/b");
        p1->init();
        p2->init();
    } catch (const NodeException &e) {
        Logger::getInstance()->log(LogLevel::ERROR, std::string("Caught: ") + e.what());
    }

    // 3. 空话题
    try {
        auto p = std::make_shared<Publisher>("empty_topic", "");
        p->init();
    } catch (const TopicException &e) {
        Logger::getInstance()->log(LogLevel::ERROR, std::string("Caught: ") + e.what());
    }

    // 4. 空消息
    try {
        Message msg("");
    } catch (const MessageException &e) {
        Logger::getInstance()->log(LogLevel::ERROR, std::string("Caught: ") + e.what());
    }

    // 5. 向不存在的话题发消息（仅警告，不崩溃）
    auto ghost = std::make_shared<Publisher>("ghost_pub", "/ghost/topic");
    ghost->init();
    ghost->publishMsg("Hello to nobody");
}
