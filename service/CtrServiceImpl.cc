#include "CtrServiceImpl.hh"
#include "CtrOptions.hh"
#include "CtrLog.hh"
#include "CtrDef.hh"

#include "soil/NumberToString.hh"
#include "rapidjson/prettywriter.h"

#include "boost/regex.hpp"

namespace ctr
{

CtrServiceImpl::CtrServiceImpl(soil::Options* options):
    hs_config_(nullptr),
    hs_handle_(nullptr)
{
  CTR_TRACE <<"CtrServiceImpl::CtrServiceImpl()";

  options_ = dynamic_cast<CtrOptions*>(options);

  initHsApi();

  login();

}

CtrServiceImpl::~CtrServiceImpl()
{
  CTR_TRACE <<"CtrServiceImpl::~CtrServiceImpl()";

  if( hs_handle_ )
  {
    CITICs_HsHlp_DisConnect( hs_handle_ );
    CITICs_HsHlp_Exit( hs_handle_ );
  }

}

void CtrServiceImpl::qryStockholder()
{
  CTR_TRACE <<"CtrServiceImpl::qryStockholder()";

  setDefaultReqMsg();

  int ret = CITICs_HsHlp_BizCallAndCommit(hs_handle_, QRY_STOCKHOLDER_FUNC, 0);
  if( ret!=0 )
  {
    std::string err_msg = "query stockholder failed.\n" + getHsError();

    throw std::runtime_error( err_msg );
  }

  rapidjson::Document doc;
  fetchRspData( RSP_QRY_STOCKHOLDER, doc );
  
  CTR_DEBUG <<"query stockholder response: \n"
            <<jsonToString( doc );

}

void CtrServiceImpl::qryStockcode(const std::string& stock_code)
{
  CTR_TRACE <<"CtrServiceImpl::qryStockcode()";

  setDefaultReqMsg();

  CITICs_HsHlp_SetValue(hs_handle_, "stock_code", stock_code.data());
  
  int ret = CITICs_HsHlp_BizCallAndCommit(hs_handle_, QRY_STOCKCODE_FUNC, 0);
  if( ret!=0 )
  {
    std::string err_msg = "query stockcode failed.\n" + getHsError();

    throw std::runtime_error( err_msg );
  }

  rapidjson::Document doc;
  fetchRspData( RSP_QRY_STOCKCODE, doc );
  
  CTR_DEBUG <<"query stockcode response: \n"
            <<jsonToString( doc );

}

void CtrServiceImpl::qryFund()
{
  CTR_TRACE <<"CtrServiceImpl::qryFund()";

  setDefaultReqMsg();

  int ret = CITICs_HsHlp_BizCallAndCommit(hs_handle_, QRY_FUND_FUNC, 0);
  if( ret!=0 )
  {
    std::string err_msg = "query fund failed.\n" + getHsError();

    throw std::runtime_error( err_msg );
  }

  rapidjson::Document doc;
  fetchRspData( RSP_QRY_FUND, doc );
  
  CTR_DEBUG <<"query fund response: \n"
            <<jsonToString( doc );

}

void CtrServiceImpl::qryEntrust(const std::string& entrust_no)
{
  CTR_TRACE <<"CtrServiceImpl::qryEntrust()";

  setDefaultReqMsg();

  CITICs_HsHlp_SetValue(hs_handle_, "locate_entrust_no", entrust_no.data());

  int ret = CITICs_HsHlp_BizCallAndCommit(hs_handle_, QRY_ENTRUST_FUNC, 0);
  if( ret!=0 )
  {
    std::string err_msg = "query entrust failed.\n" + getHsError();

    throw std::runtime_error( err_msg );
  }

  rapidjson::Document doc;
  fetchRspData( RSP_QRY_ENTRUST, doc );
  
  CTR_DEBUG <<"query entrust response: \n"
            <<jsonToString( doc );

}


void CtrServiceImpl::entrust(const std::string& stock_code,
                             int amount,
                             double price,
                             bool buy)
{
  CTR_TRACE <<"CtrServiceImpl::entrust()";

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

  int ret = CITICs_HsHlp_BizCallAndCommit(hs_handle_, ENTRUST_FUNC, 0);
  if( ret!=0 )
  {
    std::string err_msg = "entrust failed.\n" + getHsError();

    throw std::runtime_error( err_msg );
  }

  rapidjson::Document doc;
  fetchRspData( RSP_ENTRUST, doc );
  
  CTR_DEBUG <<"entrust response: \n"
            <<jsonToString( doc );

}

void CtrServiceImpl::cancelEntrust(const std::string& entrust_no)
{
  CTR_TRACE <<"CtrServiceImpl::cancelEntrust()";

  setDefaultReqMsg();

  CITICs_HsHlp_SetValue(hs_handle_, "entrust_no", entrust_no.data());
  CITICs_HsHlp_SetValue(hs_handle_, "batch_flag", "0");
  CITICs_HsHlp_SetValue(hs_handle_, "locate_entrust_no", entrust_no.data());
        
  int ret = CITICs_HsHlp_BizCallAndCommit(hs_handle_, CANCEL_ENTRUST_FUNC, 0);
  if( ret!=0 )
  {
    std::string err_msg = "cancel entrust failed.\n" + getHsError();

    throw std::runtime_error( err_msg );
  }

  rapidjson::Document doc;
  fetchRspData( RSP_CANCEL_ENTRUST, doc );
  
  CTR_DEBUG <<"cancel entrust response: \n"
            <<jsonToString( doc );

}


void CtrServiceImpl::initHsApi()
{
  CTR_TRACE <<"CtrServiceImpl::initHsApi()";

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

void CtrServiceImpl::login()
{
  CTR_TRACE <<"CtrServiceImpl::login()";

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
            <<jsonToString( rsp_login_ );
}

std::string CtrServiceImpl::getHsError()
{
  int err_no = 0;
  char msg[512];
  
  CITICs_HsHlp_GetErrorMsg(hs_handle_, &err_no, msg);

  std::string err_msg = "ErrorCode: " + soil::numToString(err_no);
  err_msg += " ErrorMsg: ";
  err_msg += msg;
  
  return err_msg;
}

void CtrServiceImpl::fetchRspData(const std::string& rsp, rapidjson::Document& doc)
{
  rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

  rapidjson::Value rsp_key;
  rsp_key.SetString(rsp.data(), rsp.length(), allocator);
  
  int row, col;
  char key[64], value[512];
  
  row =CITICs_HsHlp_GetRowCount(hs_handle_);
  
  col =CITICs_HsHlp_GetColCount(hs_handle_);

  rapidjson::Value array( rapidjson::kArrayType );

  for(int i=0; i<row; i++)
  {
    if(0 == CITICs_HsHlp_GetNextRow(hs_handle_))
    {
      rapidjson::Value o(rapidjson::kObjectType);
      for(int j=0; j<col; j++)
      {
        CITICs_HsHlp_GetColName(hs_handle_,j, key);
        CITICs_HsHlp_GetValueByIndex(hs_handle_, j, value);

        rapidjson::Value k;
        k.SetString(key, strlen(key), allocator);
          
        rapidjson::Value v;
        v.SetString(value, strlen(value), allocator);

        o.AddMember(k, v, allocator);
      }
      rapidjson::Value obj( rapidjson::kObjectType );
      obj.AddMember("Record", o, allocator);

      array.PushBack(obj, allocator);
    }
  }

  doc.SetObject();
  doc.AddMember(rsp_key, array, allocator);

}

std::string CtrServiceImpl::jsonToString(const rapidjson::Document& doc)
{
  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

  doc.Accept(writer);

  return sb.GetString();
}

void CtrServiceImpl::setDefaultReqMsg()
{
  if( rsp_login_[RSP_LOGIN].Empty() )
    throw std::runtime_error("login response is wrong.");
  
  rapidjson::Value& rsp_login_record = rsp_login_[RSP_LOGIN][0];
  
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

std::string CtrServiceImpl::toExchange(const std::string& stock_code)
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

soil::Options* CtrService::createOptions()
{
  return new CtrOptions();
}

CtrService* CtrService::createService(soil::Options* options)
{
  return new CtrServiceImpl(options);
}



};
