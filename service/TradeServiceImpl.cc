#include "TradeServiceImpl.hh"
#include "TradeOptions.hh"
#include "CtrLog.hh"
#include "HsUtil.hh"
#include "ctr/CtrDef.hh"

#include "soil/NumberToString.hh"
#include "boost/regex.hpp"

namespace ctr
{

TradeServiceImpl::TradeServiceImpl(soil::Options* options)
{
  CTR_TRACE <<"TradeServiceImpl::TradeServiceImpl()";

  options_ = dynamic_cast<TradeOptions*>(options);

  hs_util_.reset( new HsUtil(options_->hs_config) );

  login();

}

TradeServiceImpl::~TradeServiceImpl()
{
  CTR_TRACE <<"TradeServiceImpl::~TradeServiceImpl()";

}

std::string TradeServiceImpl::qryStockholder()
{
  CTR_TRACE <<"TradeServiceImpl::qryStockholder()";

  setDefaultReqMsg();

  return hs_util_->goT2(QRY_STOCKHOLDER_FUNC, "query stockholder", RSP_QRY_STOCKHOLDER);
}

std::string TradeServiceImpl::qryStockcode(const std::string& stock_code)
{
  CTR_TRACE <<"TradeServiceImpl::qryStockcode()";

  setDefaultReqMsg();

  hs_util_->setValue("stock_code", stock_code);

  return hs_util_->goT2(QRY_STOCKCODE_FUNC, "query stockcode", RSP_QRY_STOCKCODE);

}

std::string TradeServiceImpl::qryFund()
{
  CTR_TRACE <<"TradeServiceImpl::qryFund()";

  setDefaultReqMsg();

  return hs_util_->goT2(QRY_FUND_FUNC, "query fund", RSP_QRY_FUND);
}

std::string TradeServiceImpl::qryEntrust(const std::string& entrust_no)
{
  CTR_TRACE <<"TradeServiceImpl::qryEntrust()";

  setDefaultReqMsg();

  hs_util_->setValue("locate_entrust_no", entrust_no);

  return hs_util_->goT2(QRY_ENTRUST_FUNC, "query entrust", RSP_QRY_ENTRUST);
}


std::string TradeServiceImpl::entrust(const std::string& stock_code,
                             int amount,
                             double price,
                             bool buy)
{
  CTR_TRACE <<"TradeServiceImpl::entrust()";

  setDefaultReqMsg();

  hs_util_->setValue("exchange_type", toExchange(stock_code));
  hs_util_->setValue("stock_code", stock_code);
  hs_util_->setValue("entrust_amount", soil::numToString(amount));
  hs_util_->setValue("entrust_price", soil::numToString(price));
  if( buy )
    hs_util_->setValue("entrust_bs", "1");
  else
    hs_util_->setValue("entrust_bs", "2");
  
  hs_util_->setValue("entrust_prop", "0");
  hs_util_->setValue("batch_no", "0");
  hs_util_->setValue("entrust_type", "0");

  return hs_util_->goT2(ENTRUST_FUNC, "entrust", RSP_ENTRUST);
}

std::string TradeServiceImpl::cancelEntrust(const std::string& entrust_no)
{
  CTR_TRACE <<"TradeServiceImpl::cancelEntrust()";

  setDefaultReqMsg();

  hs_util_->setValue("entrust_no", entrust_no);
  hs_util_->setValue("batch_flag", "0");
  hs_util_->setValue("locate_entrust_no", entrust_no);

  return hs_util_->goT2(CANCEL_ENTRUST_FUNC, "cancel entrust", RSP_CANCEL_ENTRUST);
}

std::string TradeServiceImpl::subscribe()
{
  CTR_TRACE <<"TradeServiceImpl::subscribe()";

  std::string msg = "issue_type:" + options_->issue_type;
  msg += ",acc_info:" + options_->acc_info;

  return hs_util_->goT2(SUBSCRIBE_FUNC, "subscribe", RSP_SUBSCRIBE, msg);
}

std::string TradeServiceImpl::unsubscribe()
{
  CTR_TRACE <<"TradeServiceImpl::unsubscribe()";

  std::string msg = "issue_type:" + options_->issue_type;
  msg += ",acc_info:" + options_->acc_info;

  return hs_util_->goT2(UNSUBSCRIBE_FUNC, "unsubscribe", RSP_UNSUBSCRIBE, msg);
}

void TradeServiceImpl::login()
{
  CTR_TRACE <<"TradeServiceImpl::login()";

  hs_util_->beginParam();
  hs_util_->setValue("identity_type", "2");
  hs_util_->setValue("password_type", "2");
  hs_util_->setValue("input_content", "1");
  hs_util_->setValue("op_entrust_way", options_->op_entrust_way);
  hs_util_->setValue("password", options_->password);
  hs_util_->setValue("account_content", options_->account);
  hs_util_->setValue("op_station", options_->op_station);

  std::string rsp_login = hs_util_->goT2(LOGIN_FUNC, "login", RSP_LOGIN);
  CTR_DEBUG <<"rsp login:\n"
            <<rsp_login;

  json::fromString(rsp_login, rsp_login_);
}

void TradeServiceImpl::setDefaultReqMsg()
{
  if( rsp_login_[RSP_LOGIN].Empty() )
    throw std::runtime_error("login response is wrong.");
  
  json::Value& rsp_login_record = rsp_login_[RSP_LOGIN][0];

  hs_util_->beginParam();
  hs_util_->setValue("client_id", rsp_login_record["client_id"].GetString());
  
  hs_util_->setValue("fund_account", options_->account);
  hs_util_->setValue("sysnode_id", rsp_login_record["sysnode_id"].GetString());

  hs_util_->setValue("identity_type", "2");	
  hs_util_->setValue("op_branch_no", rsp_login_record["branch_no"].GetString());
  hs_util_->setValue("branch_no", rsp_login_record["branch_no"].GetString());

  hs_util_->setValue("op_station", options_->op_station);
  hs_util_->setValue("op_entrust_way", options_->op_entrust_way);
  hs_util_->setValue("password_type", "2");
  hs_util_->setValue("password", options_->password);
  hs_util_->setValue("asset_prop", rsp_login_record["asset_prop"].GetString());
  hs_util_->setValue("user_token", rsp_login_record["user_token"].GetString());
  hs_util_->setValue("request_num", "500");

}

std::string TradeServiceImpl::toExchange(const std::string& stock_code)
{
  boost::regex sh_regex("^(60)(.*)");
  boost::regex sz_regex("^(00|30)(.*)");

  if( boost::regex_match(stock_code, sh_regex) )
    return "1";

  if( boost::regex_match(stock_code, sz_regex) )
    return "2";

  std::string err_msg = "the stock code - " + stock_code + " is wrong.";
      
  throw std::runtime_error(err_msg);
}

soil::Options* TradeService::createOptions()
{
  return new TradeOptions();
}

TradeService* TradeService::createService(soil::Options* options)
{
  return new TradeServiceImpl(options);
}



};
