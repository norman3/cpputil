//------------------------------------------------------------------------------
// @file  getopt.cc
//------------------------------------------------------------------------------
// @brief getopt wrapper class
//------------------------------------------------------------------------------
#include "getopt.h"


namespace cu {


namespace {
// check '-' or '--' character from prefix.
bool is_opt(const std::string& opt) {
  auto sz = opt.size();
  if (sz < 2)
    return false;
  if (opt[0] == '-')
    return true;
  return false;
}

}  // namespace


//-----------------------------------------------------------------------------
// @brief GetOpt constructor
//-----------------------------------------------------------------------------
GetOpt::GetOpt(int argc, char* argv[]) : opt_{} {
  Init(argc, argv);
}

//-----------------------------------------------------------------------------
// @brief GetOpt constructor
//-----------------------------------------------------------------------------
GetOpt::GetOpt(const std::vector<std::string>& args) : opt_{} {
  Init(args);
}

//-----------------------------------------------------------------------------
// @brief initialize opt object
//-----------------------------------------------------------------------------
void GetOpt::Init(int argc, char* argv[]) {
  if ((argc <= 1) || (argv == nullptr))
    return;
  std::vector<std::string> args;
  args.reserve(argc - 1);
  for(auto i = 1; i < argc; i++)
    args.push_back(argv[i]);
  Init(args);
}

//-----------------------------------------------------------------------------
// @brief initialize opt object
//-----------------------------------------------------------------------------
void GetOpt::Init(const std::vector<std::string>& args) {
  std::string cmd = "";
  for(const auto& arg : args) {
    if (is_opt(arg)) {
      if (!cmd.empty())
        opt_[cmd] = "";
      cmd = arg;
    }
    else {
      if (!cmd.empty())
        opt_[cmd] = arg;
      cmd = "";
    }
  }
}

//------------------------------------------------------------------------------
// @brief return true if GetOpt has [arg] cmd.
//------------------------------------------------------------------------------
bool GetOpt::HasOpt(const std::string& opt) const {
  assert(!opt.empty());
  if (opt_.find(opt) != opt_.end())
    return true;
  return false;
}

}  // namespace cu
