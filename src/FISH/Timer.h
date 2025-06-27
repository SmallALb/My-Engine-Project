#pragma once

#include "Core.h"

namespace FISH {
    using TimerFUN = std::function<void()>;
    //计时器类型
    enum class TimerMode {
        SINGLE,
        REPEATING
    };
    //计时器属性
    struct TimerProps {
        //id
        uint32_t id;
        //间隔（ms）
        int interval;
        //下次执行时间
        std::chrono::steady_clock::time_point startTime;
        //回调函数
        TimerFUN usingFun;
        //模式
        TimerMode mode;
        //激活状态
        mutable bool active;
        //小于号重载
        bool operator < (const TimerProps& b) const {
            return startTime < b.startTime;
        }

        TimerProps(
            int _id, 
            int _interval, 
            std::chrono::steady_clock::time_point _startT,
            TimerFUN FUN,
            TimerMode _mode,
            bool state
        ): id(_id), interval(_interval), startTime(_startT), 
            usingFun(FUN), mode(_mode), active(state) {}
    };

    //计时器
    class API_ Timer {
    public:
        Timer(): timersLock(), conditionV(), runTag(false) {}
        ~Timer();
        //创建事件计时器
        int createTimer(int interval, TimerFUN fun, TimerMode mode = TimerMode::SINGLE);
        //停止计时器
        void stopTimer(int id);
        //停止所有
        void stopAll();
    private:
        //运行函数
        void timerWorker();

    private:
        //定时器列表
        std::multiset<TimerProps>       timers;
        //互斥锁
        std::recursive_mutex            timersLock;
        //线程条件变量
        std::condition_variable_any          conditionV;
        //运行标志
        std::atomic<bool>               runTag{false};
        //主线程，用于控制所有计时器的线程
        std::thread                     mainThread;
    };

}