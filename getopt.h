//------------------------------------------------------------------------------
// @file  getopt.h
//------------------------------------------------------------------------------
// @brief getopt wrapper class
//------------------------------------------------------------------------------
#ifndef CPPUTIL_GETOPT_H_
#define CPPUTIL_GETOPT_H_
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "variadic.h"  // for array_cast
//------------------------------------------------------------------------------
// @code
// GetOpt opt(argc, argv);
// std::string w = opt.Parse("-w", "--worker");
// int i = opt.ParseInt("-t", "--thread", "--thread_num");
// double j = opt.ParseDouble("-j");
//------------------------------------------------------------------------------


namespace cu {


//------------------------------------------------------------------------------
// @class GetOpt
//------------------------------------------------------------------------------
class GetOpt {
 public:
  using Opts = std::unordered_map<std::string, std::string>;

 private:
  Opts opt_;

 public:
  GetOpt(int argc, char* argv[]);
  explicit GetOpt(const std::vector<std::string>& argv);
  GetOpt(const GetOpt&) = default;
  GetOpt(GetOpt&&) = default;  
  GetOpt& operator=(const GetOpt&) = default;
  GetOpt& operator=(GetOpt&&) = default;

 public:
  bool empty() const {
    return opt_.empty();
  }
  
  std::size_t size() const {
    return opt_.size();
  }

 private:
  void Init(int argc, char* argv[]);
  void Init(const std::vector<std::string>& args);

 public:
  bool HasOpt(const std::string& opt) const;

  template <typename T, typename... Args>
  T Parse(Args&&... opts) const;

 public:
  using const_iterator = Opts::const_iterator;
  const_iterator begin() const { return opt_.begin(); }
  const_iterator end() const { return opt_.end(); }
};


//------------------------------------------------------------------------------
// @code auto val = opts.Parse
//------------------------------------------------------------------------------
template <typename T, typename... Args>
inline T GetOpt::Parse(Args&&... opts) const {
  auto cmds = array_cast<std::string>(std::forward<Args>(opts)...);
  for(const auto& cmd : cmds) {
    auto cursor = opt_.find(cmd);
    if (cursor != opt_.end()) {
      return static_cast<T>(cursor->second);
    }
  }
  return T();
}


}  //  namespace cu
#endif  // CPPUTIL_GETOPT_H_
