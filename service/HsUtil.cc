#include "HsUtil.hh"
#include "CtrLog.hh"

#include "soil/NumberToString.hh"

namespace ctr
{

HsUtil::HsUtil(const std::string& hs_config):
    hs_config_(nullptr),
    hs_handle_(nullptr)
{
  CTR_TRACE <<"HsUtil::HsUtil()";

  initHsApi( hs_config );
}

HsUtil::~HsUtil()
{
  CTR_TRACE <<"HsUtil::~HsUtil()";
  
  if( hs_handle_ )
  {
    CITICs_HsHlp_DisConnect( hs_handle_ );
    CITICs_HsHlp_Exit( hs_handle_ );
  }
  
}

std::string HsUtil::goT2(int func_no, const std::string& func_name, const std::string& func_rsp, const std::string& msg)
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

void HsUtil::initHsApi(const std::string& hs_config)
{
  CTR_TRACE <<"HsUtil::initHsApi()";

  CTR_INFO <<"hs config file: " <<hs_config;
  
  int ret = CITICs_HsHlp_LoadConfig(&hs_config_, hs_config.data());
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

std::string HsUtil::getHsError()
{
  int err_no = 0;
  char msg[ERROR_MSG_SIZE];
  
  CITICs_HsHlp_GetErrorMsg(hs_handle_, &err_no, msg);

  std::string err_msg = "ErrorCode: " + soil::numToString(err_no);
  err_msg += " ErrorMsg: ";
  err_msg += msg;
  
  return err_msg;
}

void HsUtil::fetchRspData(const std::string& rsp, json::Document& doc)
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

// void HsUtil::setDefaultReqMsg()
// {
//   if( rsp_login_[RSP_LOGIN].Empty() )
//     throw std::runtime_error("login response is wrong.");
  
//   json::Value& rsp_login_record = rsp_login_[RSP_LOGIN][0];
  
//   CITICs_HsHlp_BeginParam(hs_handle_);
//   CITICs_HsHlp_SetValue(hs_handle_, "client_id", rsp_login_record["Record"]["client_id"].GetString());
  
//   CITICs_HsHlp_SetValue(hs_handle_, "fund_account", options_->account.data());
//   CITICs_HsHlp_SetValue(hs_handle_, "sysnode_id", rsp_login_record["Record"]["sysnode_id"].GetString());
  
//   CITICs_HsHlp_SetValue(hs_handle_, "identity_type", "2");	
//   CITICs_HsHlp_SetValue(hs_handle_, "op_branch_no", rsp_login_record["Record"]["branch_no"].GetString());
//   CITICs_HsHlp_SetValue(hs_handle_, "branch_no", rsp_login_record["Record"]["branch_no"].GetString());
  
//   CITICs_HsHlp_SetValue(hs_handle_, "op_station", options_->op_station.data());
//   CITICs_HsHlp_SetValue(hs_handle_, "op_entrust_way", options_->op_entrust_way.data());
//   CITICs_HsHlp_SetValue(hs_handle_, "password_type", "2");
//   CITICs_HsHlp_SetValue(hs_handle_, "password", options_->password.data());
//   CITICs_HsHlp_SetValue(hs_handle_, "asset_prop", rsp_login_record["Record"]["asset_prop"].GetString());
//   CITICs_HsHlp_SetValue(hs_handle_, "user_token", rsp_login_record["Record"]["user_token"].GetString());
//   CITICs_HsHlp_SetValue(hs_handle_, "request_num", "500");
// }



};
