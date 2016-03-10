#include "TradeServiceImpl.hh"
#include "TradeOptions.hh"
#include "CtrLog.hh"
#include "CtrDef.hh"

#include "soil/NumberToString.hh"
#include "boost/regex.hpp"

namespace ctr
{

TradeServiceImpl::TradeServiceImpl(soil::Options* options):
    hs_config_(nullptr),
    hs_handle_(nullptr)
{
  CTR_TRACE <<"TradeServiceImpl::TradeServiceImpl()";

  options_ = dynamic_cast<TradeOptions*>(options);

  initHsApi();

  login();

}

TradeServiceImpl::~TradeServiceImpl()
{
  CTR_TRACE <<"TradeServiceImpl::~TradeServiceImpl()";

  if( hs_handle_ )
  {
    CITICs_HsHlp_DisConnect( hs_handle_ );
    CITICs_HsHlp_Exit( hs_handle_ );
  }

}

std::string TradeServiceImpl::qryStockholder()
{
  CTR_TRACE <<"TradeServiceImpl::qryStockholder()";

  setDefaultReqMsg();

  return goT2(QRY_STOCKHOLDER_FUNC, "query stockholder", RSP_QRY_STOCKHOLDER);
}

std::string TradeServiceImpl::qryStockcode(const std::string& stock_code)
{
  CTR_TRACE <<"TradeServiceImpl::qryStockcode()";

  setDefaultReqMsg();

  CITICs_HsHlp_SetValue(hs_handle_, "stock_code", stock_code.data());

  return goT2(QRY_STOCKCODE_FUNC, "query stockcode", RSP_QRY_STOCKCODE);

}

std::string TradeServiceImpl::qryFund()
{
  CTR_TRACE <<"TradeServiceImpl::qryFund()";

  setDefaultReqMsg();

  return goT2(QRY_FUND_FUNC, "query fund", RSP_QRY_FUND);
}

std::string TradeServiceImpl::qryEntrust(const std::string& entrust_no)
{
  CTR_TRACE <<"TradeServiceImpl::qryEntrust()";

  setDefaultReqMsg();

  CITICs_HsHlp_SetValue(hs_handle_, "locate_entrust_no", entrust_no.data());

  return goT2(QRY_ENTRUST_FUNC, "query entrust", RSP_QRY_ENTRUST);
}


std::string TradeServiceImpl::entrust(const std::string& stock_code,
                             int amount,
                             double price,
                             bool buy)
{
  CTR_TRACE <<"TradeServiceImpl::entrust()";

  setDefaultReqMsg();

  CITICs_HsHlp_SetValue(hs_handle_, "exchange_type", toExchange(stock_code).data());
  CITICs_HsHlp_SetValue(hs_handle_, "stock_code", stock_code.data());
  CITICs_HsHlp_SetValue(hs_handle_, "entrust_amount", soil::numToString(amount).data());
  CITICs_HsHlp_SetValue(hs_handle_, "entrust_price", soil::numToString(price).data());
  if( buy )
    CITICs_HsHlp_SetValue(hs_handle_, "entrust_bs", "1");
  else
    CITICs_HsHlp_SetValue(hs_handle_, "entrust_bs", "2");
  
  CITICs_HsHlp_SetValue(hs_handle_, "entrust_prop", "0");
  CITICs_HsHlp_SetValue(hs_handle_, "batch_no", "0");
  CITICs_HsHlp_SetValue(hs_handle_, "entrust_type", "0");

  return goT2(ENTRUST_FUNC, "entrust", RSP_ENTRUST);
}

std::string TradeServiceImpl::cancelEntrust(const std::string& entrust_no)
{
  CTR_TRACE <<"TradeServiceImpl::cancelEntrust()";

  setDefaultReqMsg();

  CITICs_HsHlp_SetValue(hs_handle_, "entrust_no", entrust_no.data());
  CITICs_HsHlp_SetValue(hs_handle_, "batch_flag", "0");
  CITICs_HsHlp_SetValue(hs_handle_, "locate_entrust_no", entrust_no.data());

  return goT2(CANCEL_ENTRUST_FUNC, "cancel entrust", RSP_CANCEL_ENTRUST);
}

std::string TradeServiceImpl::subscribe()
{
  CTR_TRACE <<"TradeServiceImpl::subscribe()";

  std::string msg = "issue_type:" + options_->issue_type;
  msg += ",acc_info:" + options_->acc_info;

  return goT2(SUBSCRIBE_FUNC, "subscribe", RSP_SUBSCRIBE, msg);
}

std::string TradeServiceImpl::unsubscribe()
{
  CTR_TRACE <<"TradeServiceImpl::unsubscribe()";

  std::string msg = "issue_type:" + options_->issue_type;
  msg += ",acc_info:" + options_->acc_info;

  return goT2(UNSUBSCRIBE_FUNC, "unsubscribe", RSP_UNSUBSCRIBE, msg);
}

void TradeServiceImpl::initHsApi()
{
  CTR_TRACE <<"TradeServiceImpl::initHsApi()";

  CTR_INFO <<"hs config file: " <<options_->hs_config;
  
  int ret = CITICs_HsHlp_LoadConfig(&hs_config_, options_->hs_config.data());
  if( ret!=0 )
  {
    std::string err_msg = "load hs config failed.\n";
    err_msg = "ErrorCode " + soil::numToString(ret);

    throw std::runtime_error( err_msg );
  }

  ret = CITICs_HsHlp_Init(&hs_handle_, hs_config_); 
  if( ret!=0 )
  {
    std::string err_msg = "init hs handle failed.\n";

    throw std::runtime_error( err_msg+getHsError() );
  }

  ret = CITICs_HsHlp_ConnectServer(hs_handle_);
  if( ret!=0 )
  {
    std::string err_msg = "connect server failed.\n";
    
    throw std::runtime_error( err_msg+getHsError() );
  }

}

void TradeServiceImpl::login()
{
  CTR_TRACE <<"TradeServiceImpl::login()";

  CITICs_HsHlp_BeginParam(hs_handle_);
  CITICs_HsHlp_SetValue(hs_handle_, "identity_type", "2");
  CITICs_HsHlp_SetValue(hs_handle_, "password_type", "2");
  CITICs_HsHlp_SetValue(hs_handle_, "input_content", "1");
  CITICs_HsHlp_SetValue(hs_handle_, "op_entrust_way", options_->op_entrust_way.data());
  CITICs_HsHlp_SetValue(hs_handle_, "password", options_->password.data());
  CITICs_HsHlp_SetValue(hs_handle_, "account_content", options_->account.data());
  CITICs_HsHlp_SetValue(hs_handle_, "op_station", options_->op_station.data());

  int ret = CITICs_HsHlp_BizCallAndCommit(hs_handle_, LOGIN_FUNC, 0);
  if( ret!=0 )
  {
    std::string err_msg = "login failed.\n" + getHsError();

    throw std::runtime_error( err_msg );
  }

  fetchRspData( RSP_LOGIN, rsp_login_ );
  
  CTR_DEBUG <<"login response: \n"
            <<json::toString( rsp_login_ );
}

std::string TradeServiceImpl::getHsError()
{
  int err_no = 0;
  char msg[ERROR_MSG_SIZE];
  
  CITICs_HsHlp_GetErrorMsg(hs_handle_, &err_no, msg);

  std::string err_msg = "ErrorCode: " + soil::numToString(err_no);
  err_msg += " ErrorMsg: ";
  err_msg += msg;
  
  return err_msg;
}

void TradeServiceImpl::fetchRspData(const std::string& rsp, json::Document& doc)
{
  json::Document::AllocatorType& allocator = doc.GetAllocator();

  json::Value rsp_key;
  rsp_key.SetString(rsp.data(), rsp.length(), allocator);
  
  int row, col;
  char key[64], value[512];
  
  row =CITICs_HsHlp_GetRowCount(hs_handle_);
  
  col =CITICs_HsHlp_GetColCount(hs_handle_);

  json::Value array( json::kArrayType );

  for(int i=0; i<row; i++)
  {
    if(0 == CITICs_HsHlp_GetNextRow(hs_handle_))
    {
      json::Value o(json::kObjectType);
      for(int j=0; j<col; j++)
      {
        CITICs_HsHlp_GetColName(hs_handle_,j, key);
        CITICs_HsHlp_GetValueByIndex(hs_handle_, j, value);

        json::Value k;
        k.SetString(key, strlen(key), allocator);
          
        json::Value v;
        v.SetString(value, strlen(value), allocator);

        o.AddMember(k, v, allocator);
      }
      json::Value obj( json::kObjectType );
      obj.AddMember("Record", o, allocator);

      array.PushBack(obj, allocator);
    }
  }

  doc.SetObject();
  doc.AddMember(rsp_key, array, allocator);

}

void TradeServiceImpl::setDefaultReqMsg()
{
  if( rsp_login_[RSP_LOGIN].Empty() )
    throw std::runtime_error("login response is wrong.");
  
  json::Value& rsp_login_record = rsp_login_[RSP_LOGIN][0];
  
  CITICs_HsHlp_BeginParam(hs_handle_);
  CITICs_HsHlp_SetValue(hs_handle_, "client_id", rsp_login_record["Record"]["client_id"].GetString());
  
  CITICs_HsHlp_SetValue(hs_handle_, "fund_account", options_->account.data());
  CITICs_HsHlp_SetValue(hs_handle_, "sysnode_id", rsp_login_record["Record"]["sysnode_id"].GetString());
  
  CITICs_HsHlp_SetValue(hs_handle_, "identity_type", "2");	
  CITICs_HsHlp_SetValue(hs_handle_, "op_branch_no", rsp_login_record["Record"]["branch_no"].GetString());
  CITICs_HsHlp_SetValue(hs_handle_, "branch_no", rsp_login_record["Record"]["branch_no"].GetString());
  
  CITICs_HsHlp_SetValue(hs_handle_, "op_station", options_->op_station.data());
  CITICs_HsHlp_SetValue(hs_handle_, "op_entrust_way", options_->op_entrust_way.data());
  CITICs_HsHlp_SetValue(hs_handle_, "password_type", "2");
  CITICs_HsHlp_SetValue(hs_handle_, "password", options_->password.data());
  CITICs_HsHlp_SetValue(hs_handle_, "asset_prop", rsp_login_record["Record"]["asset_prop"].GetString());
  CITICs_HsHlp_SetValue(hs_handle_, "user_token", rsp_login_record["Record"]["user_token"].GetString());
  CITICs_HsHlp_SetValue(hs_handle_, "request_num", "500");
}

std::string TradeServiceImpl::goT2(int func_no, const std::string& func_name, const std::string& func_rsp, const std::string& msg)
{
  int ret = -1;

  if( msg.empty() )
  {
    ret = CITICs_HsHlp_BizCallAndCommit(hs_handle_, func_no, 0);
  }
  else
  {
    ret = CITICs_HsHlp_BizCallAndCommit(hs_handle_, func_no, msg.data(), BIZCALL_SUBSCRIBE, 0);
  }
  
  if( ret!=0 )
  {
    std::string err_msg = func_name + " failed.\n" + getHsError();

    throw std::runtime_error( err_msg );
  }

  json::Document doc;
  fetchRspData( func_rsp, doc );

  return json::toString(doc);

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
