#include "TradeOptions.hh"

#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>

namespace ctr
{

TradeOptions::TradeOptions():
    options_("TradeOptions")
{
  namespace po = boost::program_options;

  options_.add_options()
      ("trade.account", po::value<std::string>(&account), 
       "account")
      ("trade.password", po::value<std::string>(&password), 
       "password")

      ("trade.op_station", po::value<std::string>(&op_station), 
       "op station")
      
      ("trade.op_entrust_way", po::value<std::string>(&op_entrust_way), 
       "op entrust way")

      ("trade.issue_type", po::value<std::string>(&issue_type), 
       "issue type")
      ("trade.acc_info", po::value<std::string>(&issue_type), 
       "issue type")

      ("trade.hs_config", po::value<std::string>(&hs_config), 
       "hs config file")
      ;

  return;
  
}

TradeOptions::~TradeOptions()
{
}

po::options_description* TradeOptions::configOptions()
{
  return &options_;
}

};  
