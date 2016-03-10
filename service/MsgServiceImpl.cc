#include "MsgServiceImpl.hh"
#include "MsgOptions.hh"
#include "CtrLog.hh"
#include "CtrDef.hh"

namespace ctr
{

MsgServiceImpl::MsgServiceImpl(soil::Options* options)
{
  CTR_TRACE <<"MsgServiceImpl::MsgServiceImpl()";

  try
  {
    options_ = dynamic_cast<MsgOptions*>(options);

    hs_util_.reset( new HsUtil(options_->hs_config) );

    
    CTR_DEBUG <<"subscribe rsp:\n"
              <<subscribe();
  }
  catch(std::exception& e )
  {
    CTR_ERROR <<e.what();

    throw;
  }

}

MsgServiceImpl::~MsgServiceImpl()
{
  CTR_TRACE <<"MsgServiceImpl::~MsgServiceImpl()";

  try
  {
    CTR_DEBUG <<"unsubscribe rsp:\n"
              <<unsubscribe();
  }
  catch( std::exception& e)
  {
    CTR_ERROR <<e.what();
  }
}

std::string MsgServiceImpl::subscribe()
{
  CTR_TRACE <<"MsgServiceImpl::subscribe()";

  std::string msg = "issue_type:" + options_->issue_type;
  msg += ",acc_info:" + options_->acc_info;

  return hs_util_->goT2(SUBSCRIBE_FUNC, "subscribe", RSP_SUBSCRIBE, msg);
}

std::string MsgServiceImpl::unsubscribe()
{
  CTR_TRACE <<"MsgServiceImpl::unsubscribe()";

  std::string msg = "issue_type:" + options_->issue_type;
  msg += ",acc_info:" + options_->acc_info;

  return hs_util_->goT2(UNSUBSCRIBE_FUNC, "unsubscribe", RSP_UNSUBSCRIBE, msg);
}

soil::Options* MsgService::createOptions()
{
  return new MsgOptions();
}

MsgService* MsgService::createService(soil::Options* options)
{
  return new MsgServiceImpl(options);
}



};
