#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

using namespace std;

class ThreadPool
{
public:

    ThreadPool(int threads);

    ~ThreadPool();

    template<class F, class... Args>
    auto ThreadPool::enqueue(F&& f, Args&&... args) -> 
    future<typename std::result_of<F(Args...)>::type>;

    void waitForAll();

    void shutDown();

private:

    vector<thread> threadPool;

    queue<function<void()>> tasks;

    mutex tasksMutex;

    condition_variable condition;

    bool terminate;

    bool stopped;

    void invoke();
};

// Constructor.
ThreadPool::ThreadPool(int threads) :
    terminate(false),
    stopped(false)
{
    for(int i = 0; i < threads; i++) {

        threadPool.emplace_back(std::thread(&ThreadPool::invoke, this));
    
    }
}

template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}

void ThreadPool::invoke() {

    function<void()> task;
    for( ; ; )
    {
        // Scope based locking.
        {
            // Put unique lock on task mutex.
            unique_lock<mutex> lock(tasksMutex);

            // Wait until queue is not empty or termination signal is sent.
            condition.wait(lock, [this]{ return !tasks.empty() || terminate; });

            // If termination signal received and queue is empty then exit else continue clearing the queue.
            if (terminate && tasks.empty()) {
                return;
            }

            // Get next task in the queue.
            task = tasks.front();

            // Remove it from the queue.
            tasks.pop();
        }

        // Execute the task.
        task();
    }
}

void ThreadPool::ShutDown()
{
    // Scope based locking.
    {
        // Put unique lock on task mutex.
        unique_lock<mutex> lock(tasksMutex);

        // Set termination flag to true.
        terminate = true;
    }

    // Wake up all threads.
    condition.notify_all();

    // Join all threads.
    for(thread &thread : threadPool)
    {
        thread.join();
    }

    // Empty workers vector.
    threadPool.empty();

    // Indicate that the pool has been shut down.
    stopped = true;
}

// Destructor.
ThreadPool::~ThreadPool()
{
    if (!stopped) {
        ShutDown();
    }
}