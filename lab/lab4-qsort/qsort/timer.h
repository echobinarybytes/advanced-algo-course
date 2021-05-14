#ifndef _H_TIMER_H_
#define _H_TIMER_H_

#include <iostream>
#include <ctime>
#include <sys/time.h>

class Timer {
public:
    Timer() :start_time(0), end_time(0){}
    Timer(const Timer& timer) = delete; 
    ~Timer() {}

#if 0
    // 结论： 使用time()计时，时间精度是秒，计算程序运行时间没法使用
    // 开始计时
    void Start() {
        start_time = time(nullptr);

    }
    // 结束计时间
    void End() {
        end_time = time(nullptr);
    }

#endif

    // 使用gettimeofday()函数，可以获取微妙级别的精度，但是不可移植，只能在Linux平台使用
    // 开始计时
    void Start() {
        struct timeval t;
        gettimeofday(&t, nullptr);
        start_time = t.tv_sec + (double)t.tv_usec/CLOCKS_PER_SEC;

    }
    // 结束计时间
    void End() {
        struct timeval t;
        gettimeofday(&t, nullptr);
        end_time = t.tv_sec + (double)t.tv_usec/CLOCKS_PER_SEC;
    }
    

#if 0
    // TODO
    // 使用c++11中的chrono中的工具来编码计时器

#endif
    // 获取时间长度
    long GetTimer() {
        return  end_time - start_time; // 
    }

    void Show() {
        // std::cout << start_time << std::endl;
        // std::cout << end_time << std::endl;
         std::cout << end_time - start_time << std::endl; 
    }

private:
    double start_time;
    double end_time;
};

#endif
