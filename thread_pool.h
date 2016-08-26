//------------------------------------------------------------------------------
// @file thread_pool.h
//------------------------------------------------------------------------------
// @brief thread pooling implementation.
//------------------------------------------------------------------------------
#ifndef CPPUTIL_THREAD_POOL_H_
#define CPPUTIL_THREAD_POOL_H_
#include <vector>             // for std::vector
#include <queue>              // for std::queue
#include <memory>             // for std::make_shared
#include <thread>             // for std::thread
#include <mutex>              // for std::mutex
#include <condition_variable> // for std::condition_variable
#include <future>             // for std::future
#include <functional>         // for std::function
#include <stdexcept>          // for std::runtime_error
#include <cassert>            // for assert


namespace cu {


//------------------------------------------------------------------------------
// @class ThreadPool
//------------------------------------------------------------------------------
class ThreadPool {
 public:
  using Task = std::function<void()>;
  
 public:
  explicit ThreadPool(std::size_t num_threads);
  ~ThreadPool();

 public:
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

 public:
  template<typename F, typename... Args>
  auto Enqueue(F&& f, Args&&... args)
      -> std::future<typename std::result_of<F(Args...)>::type>;

 public:
  std::size_t size() const;

 private:
  std::vector<std::thread> workers_;
  std::queue<Task> tasks_;

 private:
  std::mutex mtx_;
  std::condition_variable cond_;
  volatile bool stop_;
};


//------------------------------------------------------------------------------
// @brief ThreadPool constructor
//------------------------------------------------------------------------------
inline ThreadPool::ThreadPool(std::size_t num_threads)
    : workers_{}, tasks_{}, mtx_{}, cond_{}, stop_{false} {
  assert(num_threads > 0);
  workers_.reserve(num_threads);
  for (decltype(num_threads) i = 0; i < num_threads; i++) {
    workers_.emplace_back([this]() {
        while (true) {
          Task task;
          {
            std::unique_lock<std::mutex> lock(this->mtx_);
            this->cond_.wait(lock, [this] {
                return this->stop_ || (!this->tasks_.empty());
              });
            if (this->stop_ && this->tasks_.empty())
              return;
            task = std::move(this->tasks_.front());
            this->tasks_.pop();
          }
          task();  // call task
        }
      });
  }  // for loop
  assert(workers_.size() == num_threads);
}


//------------------------------------------------------------------------------
// @brief distructor
//------------------------------------------------------------------------------
inline ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(this->mtx_);
    stop_ = true;
  }
  cond_.notify_all();  
  for(auto& worker : workers_) {
    worker.join();
  }
}


//------------------------------------------------------------------------------
// @brief insert task into thread pool
//------------------------------------------------------------------------------
template<typename F, typename... Args>
inline auto ThreadPool::Enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type> {
  using RetType = typename std::result_of<F(Args...)>::type;
  using TaskType = std::packaged_task<RetType()>;

  auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
  auto task = std::make_shared<TaskType>(std::move(func));

  std::future<RetType> result = task->get_future();
  {
    std::unique_lock<std::mutex> lock(mtx_);
    if (true == stop_)
      throw std::runtime_error("enqueue on stopped ThreadPool");
    // add task into queue.
    tasks_.emplace([task](){ (*task)(); });
  }
  cond_.notify_one();
  return result;
}


//------------------------------------------------------------------------------
// @brief return thread size.
//------------------------------------------------------------------------------
std::size_t ThreadPool::size() const {
  return tasks_.size();
}


}  //  namespace cu
#endif  // CPPUTIL_THREAD_POOL_H_
