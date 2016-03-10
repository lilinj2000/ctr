#ifndef CTR_TRADEOPTIONS_HH
#define CTR_TRADEOPTIONS_HH

#include "soil/Config.hh"

#include <string>

namespace ctr
{
namespace po = boost::program_options;

class TradeOptions : public soil::Options
{
 public:

  TradeOptions();
  
  virtual ~TradeOptions();

  virtual po::options_description* configOptions();

  std::string account;
  std::string password;
  
  std::string op_station;

  std::string op_entrust_way;

  std::string issue_type;

  std::string acc_info;
  
  std::string hs_config;
  
 private:

  boost::program_options::options_description options_;
};

}  


#endif 
