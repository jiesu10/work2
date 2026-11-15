// publisher.h
// 发布者节点：绑定话题，向核心发送消息
#ifndef ROS_SIM_PUBLISHER_H
#define ROS_SIM_PUBLISHER_H

#include <string>
#include "node.h"
#include "message.h"

class Publisher : public Node {
private:
    std::string topicName;

public:
    Publisher(std::string name, std::string topic);
    std::string getTopic() const { return topicName; }

    void init() override;
    void shutdown() override;
    void publishMsg(const std::string &text);
    void publishSensorMsg(double temp, double hum);
};

#endif // ROS_SIM_PUBLISHER_H
