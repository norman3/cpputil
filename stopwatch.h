//------------------------------------------------------------------------------
// @file  stopwatch.hpp
//------------------------------------------------------------------------------
// @brief 정밀도 시간을 측정한다.
//------------------------------------------------------------------------------
#ifndef CPPUTIL_STOPWATCH_H_
#define CPPUTIL_STOPWATCH_H_
#include <chrono>


namespace cu {


//------------------------------------------------------------------------------
// @class Stopwatch
//------------------------------------------------------------------------------
class Stopwatch {
 private:
  using Resolution = std::chrono::high_resolution_clock;
  using TimePoint  = std::chrono::time_point<Resolution>;
  using ns = std::chrono::nanoseconds;
  using ms = std::chrono::microseconds;  

 private:
  TimePoint tp_;

 public:
  Stopwatch() : tp_{Now()} { }
  Stopwatch(const Stopwatch&) = delete;
  Stopwatch(Stopwatch&&) = delete;

 private:
  TimePoint Now() const {
    return Resolution::now();
  }

 public  :
  template<typename U>
  typename U::rep Elapsed() const {
    return std::chrono::duration_cast<U>(Now() - tp_).count();
  }

  void Reset() {
    tp_ = Now();
  }

  double sec() const {
    return nsec() / std::nano::den;
  }

  double msec() const {
    return nsec() / std::milli::den;
  }

  double nsec() const {
    return Elapsed<typename Stopwatch::ns>();
  }
};


}  // namespace cu
#endif  // CPPUTIL_STOPWATCH_H_
