#include "fspcs.h"
#include "Log.h"
#include "Timer.h"

namespace FISH {

    Timer::~Timer() {
        stopAll();
        FS_INFO("Timer destruct!");
    }

    int Timer::createTimer(int interval, TimerFUN fun, TimerMode mode) {
        //上锁
        std::lock_guard<std::recursive_mutex> lock(timersLock);
        static int idCounter = 0;
        int id = ++idCounter;
        FS_INFO("add TimerEvent: {0}", id);

        timers.emplace(id, interval, 
            std::chrono::steady_clock::now() + std::chrono::milliseconds(interval), 
            fun, mode, true);

        if (!runTag) {
            runTag = 1;
            mainThread = std::thread(&Timer::timerWorker, this);
        }

        conditionV.notify_one();
        return id;
    }

    void Timer::stopTimer(int id) {
        std::lock_guard<std::recursive_mutex> lock(timersLock);
        auto it = std::find_if(timers.begin(), timers.end(), [id](const auto& timer) {
            return timer.id == id;
        });
        if (it != timers.end()) {
            TimerProps modifiedTimer = *it;  // 复制元素
            modifiedTimer.active = false;    // 修改副本
            timers.erase(it);               // 移除原元素
            timers.emplace(std::move(modifiedTimer));   // 插入修改后的元素
        }
        FS_INFO("Done!, stopped the thread:{0}", id);
        conditionV.notify_one();        // 通知工作线程
    }

    void Timer::stopAll() {
        {
            std::lock_guard<std::recursive_mutex> lock(timersLock);
            for (auto& timer : timers) timer.active = 0;
            runTag = 0;
        }
        conditionV.notify_one();

        if (mainThread.joinable()) mainThread.join();
    }

    void Timer::timerWorker() {
        FS_INFO("Timer thread had been started");
        while(runTag) {
            std::unique_lock<std::recursive_mutex> lock(timersLock);

            //定时器队列为空时，等待传入定时器
            if (timers.empty()) {
                conditionV.wait(lock, [this] {return !timers.empty() || !runTag; });
                if (!runTag) break;
            }
            //取得最小的开始时间的线程
            auto now = std::chrono::steady_clock::now();
            auto waitTime = timers.begin()->startTime - now;
            //如果未到达指定触发时间，则等待
            if (waitTime > std::chrono::milliseconds(0)) conditionV.wait_for(lock, waitTime);
            //需要运行时间的队列
            std::vector<TimerProps> needRun;
            //需要重复执行的队列
            std::vector<TimerProps> repTimer;
            for (auto it = timers.begin(); it != timers.end(); ) {
                if (!it->active) {
                    it = timers.erase(it);
                    continue;
                }

                if (now >= it->startTime) {
                    needRun.push_back(*it);
                    if (it->mode == TimerMode::REPEATING && it->active){
                        TimerProps newTimer = *it;
                        newTimer.startTime = now + std::chrono::milliseconds(it->interval);
                        repTimer.emplace_back(std::move(newTimer));
                    }

                    it = timers.erase(it);
                }
                else it++;
            }

            //需要运行的调用函数
            for (auto& timer : needRun) {
                timer.usingFun();
            }

            //需要重复执行的重新添加回计时器中
            for (auto& x : repTimer) {
                timers.emplace(std::move(x));
            }

            lock.unlock();
        }
        FS_INFO("Timer thread had been closed");
        std::lock_guard<std::recursive_mutex> lock(timersLock);
        timers.clear();
    
    }
}
