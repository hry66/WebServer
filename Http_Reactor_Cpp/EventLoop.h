#pragma once
#include "Dispatcher.h"
#include "Channel.h"
#include <thread>
#include <queue>
#include <map>
#include <mutex>
using namespace std;

// 默认枚举值以int类型存储，指定为char类型后节省了内存
enum class ElemType:char{
    ADD,
    DELETE,
    MODIFY
};

// 定义任务队列的节点
struct ChannelElement{
    ElemType type;   // 如何处理该节点中的channel
    Channel* channel;
};

class Dispatcher;

class EventLoop{
public:
    //带参和不带参分别对应之前的eventLoopInit和eventLoopInitEx
    EventLoop();
    EventLoop(const string threadName);
    ~EventLoop();
    // 启动反应堆模型
    int run();
    // 处理别激活的文件fd
    int eventActive(int fd, int event);
    // 添加任务到任务队列
    int addTask(struct Channel* channel, ElemType type);
    // 处理任务队列中的任务
    int processTaskQ();
    // 处理dispatcher中的节点
    int add(Channel* channel);
    int remove(Channel* channel);
    int modify(Channel* channel);
    // 释放channel
    int freeChannel(Channel* channel);

    static int readLocalMessage(void* arg);
    int readMessage();

    // 返回线程ID
    inline thread::id getThreadID(){
        return m_threadID;
    }
    inline string getThreadName(){
        return m_threadName;
    }

private:
    void taskWakeup();

private:
    bool m_isQuit;

    // 该指针指向子类的实例 epoll, poll, select
    Dispatcher* m_dispatcher;

    // 任务队列
    queue<ChannelElement*> m_taskQ;

    // ChannelMap
    map<int, Channel*> m_channelMap;

    // 线程id, name, mutex
    thread::id m_threadID;
    string m_threadName;
    mutex m_mutex;

    // 存储本地通信的fd，通过socketpair 初始化
    int m_socketPair[2];  // 存储本地通信的fd 通过socketpair 初始化
};


