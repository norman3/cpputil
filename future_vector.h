//------------------------------------------------------------------------------
// @file  future_vector.h
//------------------------------------------------------------------------------
// @brief manage std::future objects (repetitive tasks with thread pool.)
//------------------------------------------------------------------------------
#ifndef CPPUTIL_FUTURE_VECTOR_H_
#define CPPUTIL_FUTURE_VECTOR_H_
#include <future>  // for std::future
#include <vector>  // for std::vector
#include "thread_pool.h"


namespace cu {


//------------------------------------------------------------------------------
// @class FutureVector<ReturnType>
//------------------------------------------------------------------------------
template <typename ReturnType>
class FutureVector {
  static_assert(!std::is_reference<ReturnType>::value,
                "can't use reference type in future-vector");

 private:
  ThreadPool* pool_;
  std::vector<std::future<ReturnType>> futures_;
  
 public:
  FutureVector() : pool_{nullptr}, futures_{} { }
  explicit FutureVector(ThreadPool& pool) : pool_{&pool}, futures_{} { }

 public:
  FutureVector(const FutureVector&) = delete;
  FutureVector(FutureVector&&) = default;
  FutureVector& operator=(const FutureVector&) = delete;
  FutureVector& operator=(FutureVector&&) = default;

 public:
  std::vector<ReturnType> get() {
    std::vector<ReturnType> result;
    result.reserve(futures_.size());
    for(auto& fut : futures_)
      result.emplace_back(fut.get());
    clear();
    return result;
  }
  
  void clear() {
    futures_.clear();
  }

  bool empty() const {
    return futures_.empty();
  }

  void Enqueue(std::future<ReturnType>&& f) {
    futures_.emplace_back(std::move(f));
  }

  template<typename F, typename... Args>
  void Enqueue(F&& func, Args&&... args) {
    using FuncRetType = typename std::result_of<F(Args...)>::type;
    static_assert(std::is_same<FuncRetType, ReturnType>::value,
                  "Invalid 'func()' return type.");
    if (pool_) {
      auto f = pool_->Enqueue(std::forward<F>(func),
                              std::forward<Args>(args)...);
      Enqueue(std::move(f));
    }
    else {
      auto bf = std::bind(std::forward<F>(func),
                          std::forward<Args>(args)...);
      std::promise<ReturnType> p;
      p.set_value(bf());
      Enqueue(p.get_future());
    }
  }
};


//------------------------------------------------------------------------------
// @class FutureVector<void>
// @brief specialized template class for non-return type function
//------------------------------------------------------------------------------
template <>
class FutureVector<void> {
 private:
  ThreadPool* pool_;
  std::vector<std::future<void>> futures_;
  
 public:
  FutureVector() : pool_{nullptr}, futures_{} { }
  
  explicit FutureVector(ThreadPool& pool) : pool_{&pool}, futures_{} { }

 public:
  FutureVector(const FutureVector&) = delete;
  FutureVector(FutureVector&&) = default;
  FutureVector& operator=(const FutureVector&) = delete;
  FutureVector& operator=(FutureVector&&) = default;
  
 public:
  void get() {
    for(auto& fut : futures_)
      fut.get();
    clear();
  }
  
  void clear() {
    futures_.clear();
  }

  bool empty() const {
    return futures_.empty();
  }

  void Enqueue(std::future<void>&& f) {
    futures_.emplace_back(std::move(f));
  }

  template<class F, class... Args>    
  void Enqueue(F&& func, Args&&... args) {
    using RetType = typename std::result_of<F(Args...)>::type;
    static_assert(std::is_void<RetType>::value,
                  "Invalid func() return type. (use 'void')");
    if (pool_) {
      auto f = pool_->Enqueue(std::forward<F>(func),
                              std::forward<Args>(args)...);
      Enqueue(std::move(f));
    }
    else {
      using TaskType = std::packaged_task<void()>;
      auto df = std::bind(std::forward<F>(func),
                          std::forward<Args>(args)...);
      auto task = std::make_shared<TaskType>(std::move(df));
      futures_.emplace_back(task->get_future());
    }
  }
};


}  // namespace cu
#endif  // CPPUTIL_FUTURE_VECTOR_H_
