#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <any>
#include <future>

enum class TaskLevel {
    Height,
    Normal,
    Low
}; 
//---
//## Task Pool
//  - Primarily used to handle various asynchronous operations
//  - Support passing all functions as tasks
//  - If the function has a result, it will return the std::future<Res>
//  - It's recommended to write callback mode when using it
//---
class TaskPool {
    //Task Interface
    class TASKI {
    public:
        virtual ~TASKI() = default;

        virtual void execute()  = 0;
        virtual TaskLevel getLevel() const {return TaskLevel::Normal;}
    };

    //Task template class，Generate polymorphism after compilation
    template<class Func, class ResultType = std::invoke_result_t<Func>>
    class TASK final : public TASKI {
        static_assert(std::is_invocable_v<Func>, "Func must be callable without arguments");
    public:
        TASK(const Func& func_, TaskLevel lev): func(func_), level(lev) {}
        //execute
        void execute() override {
            if constexpr (std::is_void_v<ResultType>) {
                func();
                result.set_value();
            }
            else {
                auto res = func();
                result.set_value(std::move(res));
            }
        }
        //return Task Level
        TaskLevel getLevel() const override {return level;}
        //return Task Result
        std::future<ResultType> getResult() {return result.get_future();} 
    private:
        Func func;
        std::promise<ResultType> result;
        TaskLevel level;
    };

    using TaskPtr = std::unique_ptr<TASKI>;
public:
    TaskPool(size_t siz) {
        ThreadPool.reserve(siz);
        for (int i=0; i<siz; i++)
            ThreadPool.emplace_back([this] {work();});    
        }

    ~TaskPool() {
        waitAll();
        stop();
        
    }
    //submit a task(function and level) return results(if any)
    template<class Func>
    auto submit(Func&& func, TaskLevel level = TaskLevel::Normal)->std::future<std::invoke_result_t<Func>> {
        using ResTy = std::invoke_result_t<Func>;

        auto task = std::make_unique<TASK<Func, ResTy>>(std::forward<Func>(func), level);

        std::future<ResTy> res = task->getResult();
        {
            std::lock_guard<std::mutex> lock(Que_mutex);
            if (stopTag) {
                return res;
            }
            else TaskQue.push(std::move(task));
        }
        condition.notify_one();
        return res;
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(Que_mutex);
            stopTag = 1;
        }
        condition.notify_all();

        for (auto& worker : ThreadPool) if (worker.joinable()){
            worker.join();
        }
    }

    void waitAll() {
        std::unique_lock<std::mutex> lock(Que_mutex); stopTag = 1;
        all_done_condition.wait(lock,[this]() {return TaskQue.empty() && activeThreads == 0;});
    }

    int getActive() {
        return activeThreads;
    }
private:
    //function used by thread
    void work() {
        while(1) {
            TaskPtr task{nullptr};
            //lock
            {
                std::unique_lock<std::mutex> lock(Que_mutex);
                condition.wait(lock, [this](){return stopTag || !TaskQue.empty();});
                if (stopTag && TaskQue.empty()) return;
                task = std::move(const_cast<TaskPtr&>(TaskQue.top()));
                TaskQue.pop();
                ++activeThreads;
            }
            //执行任务
            task->execute();
            
            //上锁检查
            {
                std::lock_guard<std::mutex> lock(Que_mutex);
                --activeThreads;
                if (TaskQue.empty() && activeThreads == 0) all_done_condition.notify_all();
            }
        }
    }
private:
    std::vector<std::thread>    ThreadPool{};
    //停止标志
    std::atomic_bool            stopTag {0};
    //任务队列
    std::priority_queue<TaskPtr, std::vector<TaskPtr>, 
        decltype([](const TaskPtr& a, const TaskPtr& b) {return a->getLevel() > b->getLevel();})> 
    TaskQue; 
    //激活线程数
    std::atomic_size_t          activeThreads{0};
    //线程锁
    mutable std::mutex          Que_mutex;

    std::condition_variable condition;
    std::condition_variable all_done_condition;
};