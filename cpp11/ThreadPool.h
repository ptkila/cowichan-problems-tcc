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
    
    ThreadPool(const std::size_t threads);
    
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

    // threads e tasks
    std::size_t n_threads;
    std::vector< std::thread > workers;
    std::queue< std::function<void()> > tasks;
    
    // sincronizacao
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic_bool stop;
    std::atomic_uint task_count;
    
    // metodos tasks
    template<class F> 
    void enqueue_task(F&& f);

    void invoke();
    
    void joinAll();
};

// Inicializa o ambiente

ThreadPool::ThreadPool(std::size_t threads) : 
workers(),
tasks(),
queue_mutex(),
condition(),
stop(false),
task_count(0),
n_threads(threads) 
{
    // Para cada thread recebe uma instância do método invoke
    for(std::size_t i = 0; i < threads; ++i) {
        this->workers.emplace_back([this]() -> void {
            invoke();
        });
    }
}

// Cada thread, recebe
void ThreadPool::invoke() 
{
    // starta com ele unlocked
    std::unique_lock<std::mutex> l(this->queue_mutex, std::defer_lock);
    
    std::function<void()> task;

    for(;;) {    
        
        l.lock();

        // bloqueia ate ter coisa na fila ou parar
        this->condition.wait(l, [this]() -> bool { 
            return this->stop || !this->tasks.empty(); 
        });
        
        // garantia para executar todas tasks na fila
        if(this->stop && this->tasks.empty()) { return; }
        
        // carrega e remove task da fila
        task = std::move(this->tasks.front());
        this->tasks.pop();

        l.unlock();

        // executa task
        task();
        this->task_count--;
    }
}

template<class F, class... Args>
void ThreadPool::enqueue(F&& f, Args&&... args)
{

    auto task = new std::packaged_task<void()> (
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    enqueue_task(task);

}

template<class F, class... Args>
auto ThreadPool::enqueue_return(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type> 
{

    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = new std::packaged_task<return_type()> (
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();
    
    enqueue_task(task);

    return res;
}

template<class F>
void ThreadPool::enqueue_task(F&& f) {

    std::unique_lock<std::mutex> l(this->queue_mutex);

    if(this->stop) { throw std::runtime_error("ThreadPool stopped"); }
    this->tasks.emplace([f](){ (*f)(); });
    l.unlock();

    this->task_count++;
    this->condition.notify_one();

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

void ThreadPool::waitAll() const 
{
    while(this->task_count != 0) {
        std::this_thread::yield();
    }
}

int ThreadPool::getSize() const 
{
    return this->n_threads;
}

void ThreadPool::joinAll() {
    std::vector<std::thread> workersTmp;
    std::unique_lock<std::mutex> l(this->queue_mutex);
    
    workersTmp = std::move(this->workers);
    l.unlock();
    
    for (auto& thread : workersTmp) {
        thread.join();
    }
}

ThreadPool::~ThreadPool() 
{   
    std::unique_lock<std::mutex> l(this->queue_mutex);
    
    this->stop = true;
    l.unlock();
    
    this->condition.notify_all();
    joinAll();
}

#endif