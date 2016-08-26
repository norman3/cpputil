//------------------------------------------------------------------------------
// @file  exception.h
//------------------------------------------------------------------------------
// @brief common exception class.
//------------------------------------------------------------------------------
#ifndef CPPUTIL_EXCEPTION_H_
#define CPPUTIL_EXCEPTION_H_
#include <stdexcept>
#include <sstream>
#include <string>


namespace cu {


//------------------------------------------------------------------------------
// @class Exception (default)
//------------------------------------------------------------------------------
class Exception : public std::exception {
 private:
  std::string msg_;

 public:
  Exception() : msg_{"[exception] no message"} {
  }

  explicit Exception(const std::string& msg) : msg_{""} {
    std::stringstream ss;
    ss << "[exception] " << msg << "\n";
    msg_ = ss.str();
  }

  Exception(const std::string& grp, const std::string& msg) : msg_{""} {
    std::stringstream ss;
    ss << "[exception] (" << grp << ") " << msg << "\n";
    msg_ = ss.str();
  }

 public:
  Exception(const Exception&) = default;
  Exception(Exception&&) = default;
  Exception& operator=(const Exception&) = default;
  Exception& operator=(Exception&&) = default;
  virtual ~Exception() noexcept = default;

 public:
  const char* what() const noexcept override {
    return msg_.c_str();
  }
};


//------------------------------------------------------------------------------
// @class LogicError
//------------------------------------------------------------------------------
class LogicError : public Exception {
 public:
  explicit LogicError(const std::string& msg)
      : Exception("logic error", msg) { }
  LogicError(const LogicError&) = default;
  LogicError(LogicError&&) = default;
  LogicError& operator=(const LogicError&) = default;
  LogicError& operator=(LogicError&&) = default;
  ~LogicError() noexcept override = default;
};


//------------------------------------------------------------------------------
// @class InvalidParameterError
//------------------------------------------------------------------------------
class InvalidParameterError : public Exception {
 public:
  explicit InvalidParameterError(const std::string& msg)
      : Exception("argument error", msg) { }
  InvalidParameterError(const InvalidParameterError&) = default;
  InvalidParameterError(InvalidParameterError&&) = default;
  InvalidParameterError& operator=(const InvalidParameterError&) = default;
  InvalidParameterError& operator=(InvalidParameterError&&) = default;
  ~InvalidParameterError() noexcept override = default;
};


//------------------------------------------------------------------------------
// @class SystemError
//------------------------------------------------------------------------------
class SystemError : public Exception {
 public:
  explicit SystemError(const std::string& msg)
      : Exception("system error", msg) { }
  SystemError(const SystemError&) = default;
  SystemError(SystemError&&) = default;
  SystemError& operator=(const SystemError&) = default;
  SystemError& operator=(SystemError&&) = default;
  ~SystemError() noexcept override = default;
};


//------------------------------------------------------------------------------
// @class RuntimeError
//------------------------------------------------------------------------------
class RuntimeError : public Exception {
 public:
  explicit RuntimeError(const std::string& msg)
      : Exception("runtime error", msg) { }
  RuntimeError(const RuntimeError&) = default;
  RuntimeError(RuntimeError&&) = default;
  RuntimeError& operator=(const RuntimeError&) = default;
  RuntimeError& operator=(RuntimeError&&) = default;
  ~RuntimeError() noexcept override = default;
};


}  // namespace cu
#endif  // CPPUTIL_EXCEPTION_H_
