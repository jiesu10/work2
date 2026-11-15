// exceptions.h
// 自定义异常体系：节点异常、话题异常、消息异常
#ifndef ROS_SIM_EXCEPTIONS_H
#define ROS_SIM_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class NodeException : public std::runtime_error {
public:
    explicit NodeException(const std::string &msg) : std::runtime_error(msg) {}
};

class TopicException : public std::runtime_error {
public:
    explicit TopicException(const std::string &msg) : std::runtime_error(msg) {}
};

class MessageException : public std::runtime_error {
public:
    explicit MessageException(const std::string &msg) : std::runtime_error(msg) {}
};

#endif // ROS_SIM_EXCEPTIONS_H
