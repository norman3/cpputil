#ifndef CPPUTIL_STRUTIL_H_
#define CPPUTIL_STRUTIL_H_
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "variadic.h"

namespace cu {
namespace strutil {


//------------------------------------------------------------------------------
// str을 delim 문자열 기준으로 나눈다.
// 이 때 사용되는 delim은 문자(char) 단위로 처리되는 것이 아니라 문자열 자체로
// 사용되어 split 처리를 하게 된다. code를 참고하도록 한다.
// @code
// auto v = strutil::split("abczaab", "za");  // v = { "abc", "ab" };
// @endcode
// @param str 대상 문자열
// @param delim delim 문자열
// @param accept_empty true인 경우 빈 문자열도 결과에 포함.
// @return 분할된 문자열 벡터
//------------------------------------------------------------------------------
inline std::vector<std::string> split(const std::string& str,
                                      const std::string& delim,
                                      bool accept_empty=false) {
  std::vector<std::string> result;
  if (str.empty())
    return result;
  if (delim.empty() || (delim.size() > str.size())) {
    result.push_back(str);
    return result;
  }
  std::size_t p = 0;
  std::size_t q = str.size();
  while (p < q) {
    auto pos = str.find(delim, p);
    if (std::string::npos == pos) {
      result.push_back(str.substr(p));
      break;
    }
    if (accept_empty == true) {
      result.push_back(str.substr(p, pos - p));
    }
    else if (p < pos) {
      result.push_back(str.substr(p, pos - p));
    }
    p = pos + delim.size();
  }
  return result;
}

//------------------------------------------------------------------------------
// 입력받은 문자열을 모두 합쳐 하나의 문자열로 반환한다.
// 이를 처리하기 위해 반드시 1 개 이상의 문자열을 입력받는다.
// @param s1 첫번째 문자열.
// @param strs 나머지 문자열들.
// @return argument 들을 결합하여 새롭게 생성된 문자열.
//------------------------------------------------------------------------------
template <typename... Args>
inline std::string join(std::string s1, Args&&... args) {
  if (sizeof...(args) == 0)
    return s1;
  auto str_list = vector_cast<std::string>(std::forward<Args>(args)...);
  
  std::size_t total_size = s1.size();
  for (const auto& s : str_list)
    total_size += s.size();
  s1.reserve(total_size);
  for (auto& s : str_list) {
    s1.insert(s1.end(),
              std::make_move_iterator(s.begin()),
              std::make_move_iterator(s.end()));
  }
  return s1
}

//------------------------------------------------------------------------------
// 입력받은 문자열 벡터를 합쳐 하나의 문자열로 반환한다.
// 이 때 gap 문자열을 각 문자열 사이에 추가한다.
// @param str_list 합칠 문자열 벡터
// @param gap 문자열 사이에 추가할 문자열
// @return 합친 결과를 하나의 문자열로 반환.
//------------------------------------------------------------------------------
inline std::string join(const std::vector<std::string>& str_list,
                        const std::string& gap = "") {
  std::string result;
  if (str_list.empty())
    return result;
  
  if (str_list.size() == 1)
    return str_list[0];
  
  std::size_t total_size = 0;
  std::size_t gap_size = gap.size();
  for (const auto& s : str_list)
    total_size += s.size();
  total_size += gap_size * (str_list.size() - 1);
  result.reserve(total_size);

  for (decltype(str_list.size()) i=0; i<str_list.size(); i++) {
    const auto& s = str_list[i];
    result.insert(result.end(), s.begin(), s.end());    
    if (gap.empty()) continue;
    if ((i+1) < str_list.size())
      result.insert(result.end(), gap.begin(), gap.end());
  }
  return result;
}


//------------------------------------------------------------------------------
// str 문자열이 prefix 문자열로 시작하는지 확인한다.
// @param str 입력 문자열
// @param prefix 비교해야 할 문자열
//------------------------------------------------------------------------------
inline bool startsWith(const std::string& str, const std::string& prefix) {
  if (prefix.empty()) return true;
  if (str.empty()) return false;
  
  if (str.size() < prefix.size())
    return false;
  
  std::size_t idx = 0;
  for(const auto& p : prefix) {
    if (p != str[idx++])
      return false;
  }
  return true;
}


//------------------------------------------------------------------------------
// str 문자열이 postfix 문자열로 끝나는지 확인한다.
// @param str 입력 문자열
// @param prefix 비교해야 할 문자열
//------------------------------------------------------------------------------
inline bool endsWith(const std::string& str, const std::string& postfix) {
  if (postfix.empty()) return true;
  if (str.empty()) return false;

  if (str.size() < postfix.size())
    return false;

  std::size_t idx(str.size() - postfix.size());
  for(const auto& p : postfix) {
    if (p!= str[idx++])
      return false;
  }
  return true;
}


//------------------------------------------------------------------------------
// from 문자열을 to 문자열로 치환한다.
// @param str 대상 문자열.
// @param from 대상 문자열 내에 변환하려고 하는 부분 문자열
// @param to 치환하고자 하는 문자열.
// @return str 내의 from 을 to 로 변환한 뒤의 문자열
//------------------------------------------------------------------------------
inline std::string
replace(const std::string& str,
        const std::string& from, const std::string& to) {
  auto snip = split(str, from, true);
  return join(snip, to);
}


//------------------------------------------------------------------------------
// 좌측 공백 삭제. delim 은 문자(char) 단위로 처리된다.
//------------------------------------------------------------------------------
inline std::string
ltrim(const std::string& str, const std::string& delims=" \r\t\n") {
  std::string result;
  auto pos = str.find_first_not_of(delims);
  if (fnd == std::string::npos)
    return std::string();
  return str.substr(pos);
}

//------------------------------------------------------------------------------
// 우측 공백 삭제. delim 은 문자(char) 단위로 처리된다.
//------------------------------------------------------------------------------
inline std::string
rtrim(const std::string& str, const std::string& delims=" \r\t\n") {
  std::string result = str;
  std::size_t pos = str.find_last_not_of(delims);
  if (pos != std::string::npos)
    result.erase(pos + 1);
  return result;
}

//------------------------------------------------------------------------------
// 좌우측 공백 삭제. delim 은 문자(char) 단위로 처리된다.
//------------------------------------------------------------------------------
inline std::string
trim(const std::string& str, const std::string& delims=" \r\t\n") {
  std::string result;
  result = rtrim(str, delims);
  result = ltrim(result, delims);
  return result;
}

}  // namespace strutil
#endif  // STRUTIL_H_
