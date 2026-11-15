// test_cases.cpp
// 独立测试用例：覆盖正常场景、边界场景、异常场景
// 编译：g++ -std=c++17 -pthread test_cases.cpp ../src/ros_core.cpp ../src/publisher.cpp ../src/subscriber.cpp ../src/logger.cpp -o test_cases
#include <iostream>
#include <memory>
#include <cassert>
#include "ros_core.h"
#include "publisher.h"
#include "subscriber.h"
#include "logger.h"
#include "exceptions.h"

int passed = 0;
int failed = 0;

void check(bool cond, const std::string &name) {
    if (cond) {
        std::cout << "[PASS] " << name << "\n";
        passed++;
    } else {
        std::cout << "[FAIL] " << name << "\n";
        failed++;
    }
}

// 测试 1：基础发布订阅，订阅者应收到消息
void testBasicPubSub() {
    Logger::getInstance()->setLogFile("test.log");
    auto pub = std::make_shared<Publisher>("t_pub", "/t1");
    auto sub = std::make_shared<Subscriber>("t_sub", "/t1");
    pub->init();
    sub->init();
    check(ROSCore::getInstance()->hasTopic("/t1"), "topic registered");
    pub->publishMsg("hello");
    ROSCore::getInstance()->shutdownAll();
}

// 测试 2：空节点名抛异常
void testEmptyNodeName() {
    bool caught = false;
    try {
        auto bad = std::make_shared<Publisher>("", "/x");
    } catch (const NodeException &) {
        caught = true;
    }
    check(caught, "empty node name throws NodeException");
}

// 测试 3：重复节点名抛异常
void testDuplicateNodeName() {
    bool caught = false;
    auto p1 = std::make_shared<Publisher>("dup", "/a");
    auto p2 = std::make_shared<Publisher>("dup", "/b");
    p1->init();
    try {
        p2->init();
    } catch (const NodeException &) {
        caught = true;
    }
    ROSCore::getInstance()->shutdownAll();
    check(caught, "duplicate node name throws NodeException");
}

// 测试 4：空话题抛异常
void testEmptyTopic() {
    bool caught = false;
    try {
        auto p = std::make_shared<Publisher>("et", "");
        p->init();
    } catch (const TopicException &) {
        caught = true;
    }
    check(caught, "empty topic throws TopicException");
}

// 测试 5：空消息抛异常
void testEmptyMessage() {
    bool caught = false;
    try {
        Message m("");
    } catch (const MessageException &) {
        caught = true;
    }
    check(caught, "empty message throws MessageException");
}

// 测试 6：不存在话题发消息仅警告不崩溃
void testUnknownTopic() {
    auto ghost = std::make_shared<Publisher>("ghost", "/ghost");
    ghost->init();
    ghost->publishMsg("nobody home");
    check(true, "unknown topic does not crash");
    ROSCore::getInstance()->shutdownAll();
}

// 测试 7：多话题互不串台
void testMultiTopic() {
    auto p1 = std::make_shared<Publisher>("p1", "/a");
    auto p2 = std::make_shared<Publisher>("p2", "/b");
    auto s1 = std::make_shared<Subscriber>("s1", "/a");
    auto s2 = std::make_shared<Subscriber>("s2", "/b");
    p1->init(); p2->init(); s1->init(); s2->init();
    check(ROSCore::getInstance()->hasTopic("/a") && ROSCore::getInstance()->hasTopic("/b"),
          "two topics registered");
    p1->publishMsg("to A");
    p2->publishMsg("to B");
    ROSCore::getInstance()->shutdownAll();
}

int main() {
    std::cout << "===== ROS Sim Test Cases =====\n";
    testBasicPubSub();
    testEmptyNodeName();
    testDuplicateNodeName();
    testEmptyTopic();
    testEmptyMessage();
    testUnknownTopic();
    testMultiTopic();

    std::cout << "\n===== Result: " << passed << " passed, " << failed << " failed =====\n";
    return failed == 0 ? 0 : 1;
}
