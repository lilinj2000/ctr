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

 private:

  CtrOptions* options_;

  HSHLPCFGHANDLE hs_config_;
  HSHLPHANDLE hs_handle_;
};


};

#endif
