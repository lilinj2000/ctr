#include "MsgServiceImpl.hh"
#include "MsgOptions.hh"
#include "CtrLog.hh"
#include "ctr/CtrDef.hh"

#include "soil/NumberToString.hh"

namespace ctr
{

MsgServiceImpl::MsgServiceImpl(soil::Options* options, MsgCallback* callback):
    callback_(callback)
{
  CTR_TRACE <<"MsgServiceImpl::MsgServiceImpl()";

  try
  {
    options_ = dynamic_cast<MsgOptions*>(options);

    hs_util_.reset( new HsUtil(options_->hs_config) );

    std::string rsp_sub = subscribe();

    CTR_DEBUG <<"subscribe rsp:\n"
              <<rsp_sub;

    hs_util_->startMsgProcess(callback_, options_->issue_type, options_->timeout);
    
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

  hs_util_->stopMsgProcess();
}

std::string MsgServiceImpl::subscribe()
{
  CTR_TRACE <<"MsgServiceImpl::subscribe()";

  std::string msg = "issue_type:" + soil::numToString(options_->issue_type);
  msg += ",acc_info:" + options_->acc_info;

  CTR_DEBUG <<"msg info: \n"
            <<msg;

  return hs_util_->goT2(SUBSCRIBE_FUNC, "subscribe", RSP_SUBSCRIBE, msg);

}

std::string MsgServiceImpl::unsubscribe()
{
  CTR_TRACE <<"MsgServiceImpl::unsubscribe()";

  std::string msg = "issue_type:" + soil::numToString(options_->issue_type);
  msg += ",acc_info:" + options_->acc_info;
  
  CTR_DEBUG <<"msg info: \n"
            <<msg;

  return hs_util_->goT2(UNSUBSCRIBE_FUNC, "unsubscribe", RSP_UNSUBSCRIBE, msg);
}

soil::Options* MsgService::createOptions()
{
  return new MsgOptions();
}

MsgService* MsgService::createService(soil::Options* options, MsgCallback* callback)
{
  return new MsgServiceImpl(options, callback);
}



};
