#pragma once
#include <functional>

// 定义函数指针
// C: typedef int(*handleFunc)(void* arg);
// C++: using handleFunc = int(*)(void*);
// 以上定义出来的函数指针只能指向普通函数和静态成员函数
// 普通成员函数是属于成员的，只有在实例化对象后该函数才存在

//C++11新特性：强类型枚举
enum class FDEvent//定义文件描述符的读写事件
{
    TimeOut = 0x01,
    ReadEvent = 0x02,
    WriteEvent = 0x04
};


class Channel
{
private:
    // 文件描述符
    int m_fd;
    // 事件
    int m_events;
    // 回调函数的参数
    void* m_arg;

public:
    // 可调用对象包装器可以打包：1. 函数指针 2. 可调用对象
    // 最终得到了地址, 但是没有调用
    // using类似于C中的typedef，用来创建类型别名
    using handleFunc = std::function<int(void*)>;

    //构造函数
    Channel(int fd, FDEvent events, handleFunc readFunc, handleFunc writeFunc, handleFunc destroyFunc, void* arg);

    // 回调函数
    handleFunc readCallback;
    handleFunc writeCallback;
    handleFunc destroyCallback;

    // 修改fd的写事件(检测 or 不检测)
    void writeEventEnable(bool flag);
    // 判断是否需要检测文件描述符的写事件
    bool isWriteEventEnable();

    /*提供类外部使用私有成员的方法
      使用内联函数，提高执行效率*/
    inline int getSocket()
    {
        return m_fd;
    }
    inline int getEvent()
    {
        return m_events;
    }

    inline const void* getArg()
    {
        return m_arg;
    }

};

