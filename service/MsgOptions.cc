#include "MsgOptions.hh"

#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>

namespace ctr
{

MsgOptions::MsgOptions():
    options_("MsgOptions")
{
  namespace po = boost::program_options;

  options_.add_options()
      ("msg.issue_type", po::value<int>(&issue_type), 
       "issue type")
      ("msg.acc_info", po::value<std::string>(&acc_info), 
       "acc info")
      
      ("msg.timeout", po::value<int>(&timeout), 
       "timeout")

      ("msg.hs_config", po::value<std::string>(&hs_config), 
       "hs config file")
      ;

  return;
  
}

MsgOptions::~MsgOptions()
{
}

po::options_description* MsgOptions::configOptions()
{
  return &options_;
}

};  
