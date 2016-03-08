#include "CtrServiceImpl.hh"
#include "CtrOptions.hh"
#include "CtrLog.hh"
#include "CtrDef.hh"

#include "soil/NumberToString.hh"

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

  int ret = CITICs_HsHlp_BizCallAndCommit(hs_handle_, login_function, 0);
  if( ret!=0 )
  {
    std::string err_msg = "login failed.\n" + getHsError();

    throw std::runtime_error( err_msg );
  }

  CTR_DEBUG <<"login response: \n" <<showData();
  
  char msg[512];
  memset(msg, 0x0, sizeof(msg));
  
  CITICs_HsHlp_GetValue(hs_handle_, "client_id", msg);
  client_id_ = msg;
  
  CITICs_HsHlp_GetValue(hs_handle_, "user_token", msg);
  user_token_ = msg;
  
  CITICs_HsHlp_GetValue(hs_handle_, "branch_no", msg);
  branch_no_ = msg;
  
  CITICs_HsHlp_GetValue(hs_handle_, "asset_prop", msg);
  asset_prop_ = msg;
  
  CITICs_HsHlp_GetValue(hs_handle_, "sysnode_id", msg);
  sysnode_id_ = msg;

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

std::string CtrServiceImpl::showData()
{
  std::stringstream sstream;
  
  int row, col;
  char key[64], value[512];
  
  row =CITICs_HsHlp_GetRowCount(hs_handle_);
  
  col =CITICs_HsHlp_GetColCount(hs_handle_);

  for(int i=0; i<row; i++)
  {
    if(0 == CITICs_HsHlp_GetNextRow(hs_handle_))
    {
      sstream <<"row " <<i+1 <<" --------- \n";
      
      for(int j=0; j<col; j++)
      {
        CITICs_HsHlp_GetColName(hs_handle_,j, key);

        CITICs_HsHlp_GetValueByIndex(hs_handle_, j, value);

        sstream <<key <<": " <<value <<"\n";
      }
    }
  }

  return sstream.str();
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
