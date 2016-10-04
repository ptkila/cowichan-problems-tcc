#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <algorithm>
#include <queue>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <typeinfo>  //for 'typeid' to work  

/*
    Utilidades:
    
    //número args no variadic
    int size_extra_params = sizeof...(extra_args); 
    
    using return_type = typename std::remove_reference<decltype(f)>::type;

*/

class ThreadPool {

public:
    
    ThreadPool(std::size_t);
    
    ~ThreadPool();
    template<class F, class... Args>
    void enqueue(F&& f, Args&&... args);

    template<class F, class... Args>
    auto enqueue_return(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>;
    
    template<class F, class... Args>
    void parallel_for(F&& f, const int size, Args&&... extra_args);

    template <class F>
    auto parallel_reducer(F&& f, const int size, const std::string operation)
        -> decltype(f(0, 0, 0));

    int getSize() const;

    void waitAll() const;

private:

    int n_threads;
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

    // Join Tasks
    void joinAll();
};
 
ThreadPool::ThreadPool(std::size_t threads) : stop(false), taskCount(0) 
{
    this->n_threads = threads;
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

template<class F, class... Args>
void ThreadPool::parallel_for(F&& f, const int size, Args&&... extra_args) {

    const int numOpThreadM = size / this->n_threads;
    const int numOpThreadR = size % this->n_threads;
    bool end = false;

    for (int i = 0; i < this->n_threads; ++i) {

        int firstIndex = numOpThreadM * i;
        int lastIndex = numOpThreadM * (i + 1);

        if (i + 1 == this->n_threads && numOpThreadR > 0 || numOpThreadM == 0) {
            lastIndex += numOpThreadR;
            end = true;
        }

        enqueue(f, firstIndex, lastIndex, size, std::forward<Args>(extra_args)...);

        if (end) break;
    }
}

template <class F>
auto ThreadPool::parallel_reducer(F&& f, const int size, const std::string operation)
    -> decltype(f(0, 0, 0)) {

    using return_type = decltype(f(0, 0, 0));

    if (operation.compare("max") || operation.compare("min") 
      || operation.compare("+") || operation.compare("*")) {

        std::vector<std::future<return_type>> values;
        const int numOpThreadM = size / this->n_threads;
        const int numOpThreadR = size % this->n_threads;
        bool end = false;

        for (int i = 0; i < this->n_threads; ++i) {
            
            int firstIndex = numOpThreadM * i;
            int lastIndex = numOpThreadM * (i + 1);

            if ((i + 1 == this->n_threads && numOpThreadR > 0) 
                || numOpThreadM == 0) {
                lastIndex += numOpThreadR;
                end = true;
            }

            values.emplace_back(enqueue_return(f, firstIndex,
                lastIndex, size));

            if (end) break;
        }

        return_type val;
        std::vector<return_type> sortedValues;

        for(auto& value: values) {
            sortedValues.emplace_back(value.get());
        }

        if (operation.compare("max") == 0) {
            
            std::sort(sortedValues.begin(), sortedValues.end()); 
            val = sortedValues.back();
        
        } else if (operation.compare("min") == 0) {
            
            std::sort(sortedValues.begin(), sortedValues.end()); 
            val = sortedValues.front();
        
        } else if (operation.compare("+") == 0){
            
            val = 0;
            for (return_type v: sortedValues) {
                val += v;
            }

        } else if (operation.compare("*") == 0) {

            val = 1;
            for (return_type v: sortedValues) {
                val*= v;
            }
        
        }
        
        return val;

    } else {

        std::cout << "Invalid operation" << std::endl;
        return return_type();
    
    }
}

template<class F, class... Args>
auto ThreadPool::enqueue_return(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type> 
{

    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        if(this->stop) { throw std::runtime_error("enqueue on stopped ThreadPool"); }
        tasks.emplace([task](){ (*task)(); });
    }

    this->taskCount++;
    this->condition.notify_one();

    return res;
}

template<class F, class... Args>
void ThreadPool::enqueue(F&& f, Args&&... args)
{

    auto task = std::make_shared< std::packaged_task<void()> >(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    {
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        if(this->stop) { throw std::runtime_error("enqueue on stopped ThreadPool"); }
        tasks.emplace([task](){ (*task)(); });
    }

    this->taskCount++;
    this->condition.notify_one();
}

void ThreadPool::waitAll() const 
{
    while(this->taskCount != 0) {
        std::this_thread::yield();
    }
}

int ThreadPool::getSize() const 
{
    return this->n_threads;
}

void ThreadPool::joinAll() {
    std::vector<std::thread> workersTmp;
    {
        std::lock_guard<std::mutex> lock(this->queue_mutex);
        workersTmp = std::move(this->workers);
    }
    for (auto& thread : workersTmp) {
        thread.join();
    }
}

ThreadPool::~ThreadPool() 
{
    {
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        this->stop = true;
    }
    this->condition.notify_all();
    joinAll();
}

#endif