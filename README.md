# ROS 模拟通信系统（ros_sim）

纯 C++17 模拟 ROS「节点—话题—发布/订阅」通信机制，无需安装真实 ROS。

## 快速开始

### 编译运行
```bash
g++ -std=c++17 -pthread src/main.cpp src/ros_core.cpp \
    src/publisher.cpp src/subscriber.cpp src/logger.cpp -o ros_sim
./ros_sim
```

### 运行测试
```bash
g++ -std=c++17 -pthread src/test_cases.cpp src/ros_core.cpp \
    src/publisher.cpp src/subscriber.cpp src/logger.cpp -o test_cases
./test_cases
```

## 功能一览
- **节点抽象**：Node 基类 + Publisher / Subscriber 继承多态
- **单例调度核心**：ROSCore 维护话题—节点映射，注册、转发、统一关闭
- **消息封装**：基础文本消息 + 温湿度复合消息，自动时间戳
- **异常体系**：NodeException / TopicException / MessageException 分层捕获
- **分级日志**：INFO / WARN / ERROR，控制台 + 文件双输出
- **容错处理**：空名、重复名、空话题、空消息、未知话题均不崩溃

## 目录结构
```
ros_sim/
├── CMakeLists.txt
├── src/            # 源代码
├── docs/           # 需求、架构、技术、手册、测试报告
└── logs/           # 运行日志（自动生成）
```

## 文档
- [需求文档](docs/需求文档.md)
- [架构设计文档](docs/架构设计文档.md)
- [技术文档](docs/技术文档.md)
- [用户手册](docs/用户手册.md)
- [测试报告](docs/测试报告.md)

## 技术要点
- 设计模式：单例（ROSCore / Logger）、观察者（订阅回调）、多态（节点基类）
- 线程安全：互斥锁保护注册与日志写入
- 标准库：memory、mutex、thread、chrono、map、vector、set、fstream、filesystem
