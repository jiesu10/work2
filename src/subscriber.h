#define _CRT_SECURE_NO_WARNINGS
// subscriber.h
// 订阅者节点：订阅话题，收到消息后触发回调
#ifndef ROS_SIM_SUBSCRIBER_H
#define ROS_SIM_SUBSCRIBER_H

#include <string>
#include "node.h"
#include "message.h"

class Subscriber : public Node {
private:
    std::string topicName;

public:
    Subscriber(std::string name, std::string topic);
    std::string getTopic() const { return topicName; }

    void init() override;
    void shutdown() override;
    void onReceive(const Message &msg);
};

#endif // ROS_SIM_SUBSCRIBER_H
