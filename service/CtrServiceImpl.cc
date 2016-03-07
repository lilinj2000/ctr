#include "CtrServiceImpl.hh"
#include "CtrOptions.hh"
#include "CtrLog.hh"

#include "soil/NumberToString.hh"

namespace ctr
{

CtrServiceImpl::CtrServiceImpl(soil::Options* options)
{
  CTR_TRACE <<"CtrServiceImpl::CtrServiceImpl()";

  options_ = dynamic_cast<CtrOptions*>(options);

  int ret = CITICs_HsHlp_LoadConfig(&hs_config_, options_->hs_config.data());
  if( ret!=0 )
  {
    std::string err_msg = "load hs config failed.\n";
    err_msg = "ErrorCode " + soil::numToString(ret);

    throw std::runtime_error( err_msg );
  }
}

CtrServiceImpl::~CtrServiceImpl()
{
  CTR_TRACE <<"CtrServiceImpl::~CtrServiceImpl()";
}

soil::Options* CtrService::createOptions()
{
  return new CtrOptions();
}

CtrService* CtrService::createService(soil::Options* options)
{
  return new CtrServiceImpl(options);
}



};
