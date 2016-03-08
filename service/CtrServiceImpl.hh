#ifndef CTR_SERVICE_IMPL_HH
#define CTR_SERVICE_IMPL_HH

#include "ctr/CtrService.hh"

#include "CITICs_HsT2Hlp.h"

namespace ctr
{

class CtrOptions;

class CtrServiceImpl : public CtrService
{
 public:
  CtrServiceImpl(soil::Options* options);

  virtual ~CtrServiceImpl();

 protected:
  
  void initHsApi();

  void login();

  std::string getHsError();

  std::string showData();
  
 private:

  CtrOptions* options_;

  HSHLPCFGHANDLE hs_config_;
  HSHLPHANDLE hs_handle_;

  std::string client_id_;
  std::string user_token_;
  std::string branch_no_;
  std::string asset_prop_;
  std::string sysnode_id_;
};


};

#endif
