#ifndef CTR_MSGOPTIONS_HH
#define CTR_MSGOPTIONS_HH

#include "soil/Config.hh"

#include <string>

namespace ctr
{
namespace po = boost::program_options;

class MsgOptions : public soil::Options
{
 public:

  MsgOptions();
  
  virtual ~MsgOptions();

  virtual po::options_description* configOptions();

  int issue_type;

  std::string acc_info;

  int timeout;
  
  std::string hs_config;
  
 private:

  boost::program_options::options_description options_;
};

}  


#endif 
