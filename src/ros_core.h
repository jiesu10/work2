// ros_core.h
// ROS 核心调度器（单例）：维护话题-发布者/订阅者映射，注册、转发、全局关闭
#ifndef ROS_SIM_ROS_CORE_H
#define ROS_SIM_ROS_CORE_H

#include <string>
#include <map>
#include <vector>
#include <set>
#include <memory>
#include <mutex>
#include "node.h"
#include "message.h"
#include "exceptions.h"

class ROSCore {
public:
    using NodePtr = std::shared_ptr<Node>;
    using SubscriberList = std::vector<NodePtr>;

private:
    static ROSCore *instance;
    static std::mutex mtx;

    std::map<std::string, SubscriberList> topicSubscribers;
    std::map<std::string, std::set<std::string>> topicPublishers;
    std::set<std::string> registeredNodeNames;   // 重复节点名校验
    std::vector<NodePtr> allNodes;

    ROSCore() = default;

public:
    ROSCore(const ROSCore &) = delete;
    ROSCore &operator=(const ROSCore &) = delete;

    static ROSCore *getInstance();

    void registerPublisher(NodePtr node, const std::string &topic);
    void registerSubscriber(NodePtr node, const std::string &topic);
    void publish(const std::string &topic, const Message &msg);
    void shutdownAll();
    bool hasTopic(const std::string &topic) const;

private:
    void checkNodeName(const std::string &name);
    void checkTopic(const std::string &topic);
};

#endif // ROS_SIM_ROS_CORE_H
