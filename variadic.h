//------------------------------------------------------------------------------
// @file  variadic.hpp
//------------------------------------------------------------------------------
// @brief utility function for variadic template.
//------------------------------------------------------------------------------
#ifndef CPPUTIL_VARIADIC_HPP__
#define CPPUTIL_VARIADIC_HPP__
#include <type_traits>
#include <vector>
#include <array>


namespace cu {


//------------------------------------------------------------------------------
// @brief variadic param holder
//------------------------------------------------------------------------------
struct do_in_order {
  template <typename T>
  do_in_order(std::initializer_list<T>&&){ }
};


//------------------------------------------------------------------------------
// @brief 임의의 타입을 받아들여 벡터로 반환한다.
//        단, 입력되는 파라미터가 T 타입으로의 전환이 가능해야 한다.
//        반드시 타입 T 를 명기하도록 하자. vector_cast<int> 등
//------------------------------------------------------------------------------
template <typename T, typename R, typename...Args>
inline std::vector<T> vector_cast(R&& arg1, Args&&... args) {
  std::vector<T> result;
  if (0 != sizeof...(args)) {
    result = { static_cast<T>(arg1), args... };
  }
  return result;
}


//------------------------------------------------------------------------------
// @brief 가변 인자를 입력받아 고정 크기의 array 변환 후 반환한다.
//        단, 입력되는 파라미터가 T 타입으로의 전환이 가능해야 한다.
//        입력된 인자 개수 크기의 array 가 생성된다.
//------------------------------------------------------------------------------
template <typename T, typename R, typename... Args>
inline auto array_cast(R&& arg1, Args&&... args)
    -> std::array<T, sizeof...(args) + 1> {
  std::array<T, sizeof...(args) + 1> result;
  result = { static_cast<T>(arg1) , args... };
  return result;
}


//----------------------------------------------------------------------------
// @brief 가변 인자 개수 반환
//----------------------------------------------------------------------------
template <typename... Args>
constexpr std::size_t variadic_count(Args&&... args) {
  return sizeof...(args);
}


} // namespace cu
#endif // UTIL_VARIADIC_HPP__
