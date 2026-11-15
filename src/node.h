// node.h
// 节点抽象基类：所有发布者/订阅者的父类，继承 enable_shared_from_this
#ifndef ROS_SIM_NODE_H
#define ROS_SIM_NODE_H

#include <string>
#include <memory>
#include "exceptions.h"

class Node : public std::enable_shared_from_this<Node> {
protected:
    std::string nodeName;

public:
    explicit Node(std::string name) : nodeName(std::move(name)) {
        if (nodeName.empty()) {
            throw NodeException("Node name cannot be empty!");
        }
    }
    virtual ~Node() = default;

    std::string getName() const { return nodeName; }
    virtual void init() = 0;
    virtual void shutdown() = 0;
};

#endif // ROS_SIM_NODE_H
