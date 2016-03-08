#include "CtrOptions.hh"

#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>

namespace ctr
{

CtrOptions::CtrOptions():
    options_("CtrOptions")
{
  namespace po = boost::program_options;

  options_.add_options()
      ("ctr.account", po::value<std::string>(&account), 
       "account")
      ("ctr.password", po::value<std::string>(&password), 
       "password")

      ("ctr.op_station", po::value<std::string>(&op_station), 
       "op station")
      
      ("ctr.op_entrust_way", po::value<std::string>(&op_entrust_way), 
       "op entrust way")
      
      ("ctr.hs_config", po::value<std::string>(&hs_config), 
       "hs config file")
      ;

  return;
  
}

CtrOptions::~CtrOptions()
{
}

po::options_description* CtrOptions::configOptions()
{
  return &options_;
}

};  
