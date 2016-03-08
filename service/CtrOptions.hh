#ifndef CTR_OPTIONS_HH
#define CTR_OPTIONS_HH

#include "soil/Config.hh"

#include <string>

namespace ctr
{
namespace po = boost::program_options;

class CtrOptions : public soil::Options
{
 public:

  CtrOptions();
  
  virtual ~CtrOptions();

  virtual po::options_description* configOptions();

  std::string account;
  std::string password;
  
  std::string op_station;

  std::string op_entrust_way;
  
  std::string hs_config;
  
 private:

  boost::program_options::options_description options_;
};

}  


#endif 
