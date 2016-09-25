#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool {

public:
    
    ThreadPool(std::size_t);
    ~ThreadPool();
    
    template<class F, class... Args>
    void enqueue(F&& f, Args&&... args);

    template<class F, class... Args>
    auto enqueueWithReturn(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;
    
    void waitForAll();

private:
    // need to keep track of threads so we can join them
    std::vector< std::thread > workers;
    // the task queue
    std::queue< std::function<void()> > tasks;
    
    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic_bool stop;
    
    //Task Counter
    std::atomic_uint taskCount;
    
    //Launch tasks
    void invoke();
};
 
// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(std::size_t threads) : stop(false), taskCount(0) 
{
    for(std::size_t i = 0; i < threads; ++i) {
        this->workers.emplace_back([this]() -> void {
            ThreadPool::invoke();
        });
    }
}

void ThreadPool::invoke() 
{
    for(;;) {
        std::function<void()> task;
        {
            //acquire lock
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            this->condition.wait(lock, [this]() -> bool { 
                return this->stop || !this->tasks.empty(); 
            });
            
            if(this->stop && this->tasks.empty()) {
                return;
            }

            task = std::move(this->tasks.front());
            this->tasks.pop();
            //release lock
        }
        task();
        this->taskCount--;
    }
}

void ThreadPool::waitForAll() 
{
    while(this->taskCount != 0) {
        std::this_thread::yield();
    }
}

template<class F, class... Args>
auto ThreadPool::enqueueWithReturn(F&& f, Args&&... args) ->
std::future<typename std::result_of<F(Args...)>::type> 
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
        
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        if(this->stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
        tasks.emplace([task](){ (*task)(); });
    }
    this->taskCount++;
    this->condition.notify_one();

    return res;
}

template<class F, class... Args>
void ThreadPool::enqueue(F&& f, Args&&... args)
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    {
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        if(this->stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
        tasks.emplace([task](){ (*task)(); });
    }

    this->taskCount++;
    this->condition.notify_one();
}

inline ThreadPool::~ThreadPool() 
{
    {
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        this->stop = true;
    }
    
    this->condition.notify_all();
    
    for(std::thread &worker: this->workers){
        worker.join();
    }

    this->workers.empty();
}

#endif