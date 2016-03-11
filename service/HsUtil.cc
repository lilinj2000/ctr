#include "HsUtil.hh"
#include "CtrLog.hh"

#include "soil/NumberToString.hh"

namespace ctr
{

HsUtil::HsUtil(const std::string& hs_config):
    hs_config_(nullptr),
    hs_handle_(nullptr),
    msg_run_(false),
    timeout_(1000)
{
  CTR_TRACE <<"HsUtil::HsUtil()";

  initHsApi( hs_config );
}

HsUtil::~HsUtil()
{
  CTR_TRACE <<"HsUtil::~HsUtil()";

  stopMsgProcess();
  
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

void HsUtil::beginParam()
{
  CITICs_HsHlp_BeginParam(hs_handle_);
}

void HsUtil::setValue(const std::string& name, const std::string& value)
{
  CITICs_HsHlp_SetValue(hs_handle_, name.data(), value.data());
}

void HsUtil::startMsgProcess(MsgCallback* callback, int issue_type, int timeout)
{
  if( !msg_run_ )
  {
    msg_ctrl_.reset( new MSG_CTRL() );

    msg_ctrl_->nIssueType = issue_type;

    timeout_ = timeout;

    cond_.reset( soil::STimer::create() );

    msg_queue_.reset( new soil::MsgQueue<json::Document, MsgCallback>(callback) );

    msg_run_ = true;

    msg_thread_.reset( new std::thread(&HsUtil::msgProcess, this) );
  }
}

void HsUtil::stopMsgProcess()
{
  if( msg_run_ )
  {
    msg_run_ = false;

    cond_->notifyAll();

    msg_thread_->join();
  }

}

void HsUtil::msgProcess()
{
  CTR_TRACE <<"HsUtil::msgProcess()";
  
  while( msg_run_ )
  {
    if( CITICs_HsHlp_QueueMsgCount(hs_handle_)>0 )
    {
      int ret = CITICs_HsHlp_QueueGetMsg(hs_handle_, msg_ctrl_.get());

      CTR_DEBUG <<"msg_ctrl.nFlags: " <<msg_ctrl_->nFlags;

      if( msg_ctrl_->nFlags!=0 )
      {
        CTR_INFO <<"the subscribe is not success.";

        cond_->wait( timeout_ );
        
        continue;
      }

      if( msg_ctrl_->HAsyncMsg!=0 )
      {
        std::unique_ptr<json::Document> doc(new json::Document());
        
        fetchMsgData( *doc );

        msg_queue_->pushMsg( doc.get() );

        CITICs_HsHlp_QueueEraseMsg(hs_handle_, msg_ctrl_.get());
      }
    }
    else
    {
      cond_->wait( timeout_ );
    }
  }

  CTR_INFO <<"HsUtil::msgProcess() done.";
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

void HsUtil::fetchMsgData(json::Document& doc)
{
  json::Document::AllocatorType& allocator = doc.GetAllocator();

  int row, col;
  char key[64], value[512];
  
  row =CITICs_HsHlp_GetRowCount(hs_handle_, msg_ctrl_.get());
  
  col =CITICs_HsHlp_GetColCount(hs_handle_, msg_ctrl_.get());

  json::Value array( json::kArrayType );

  for(int i=0; i<row; i++)
  {
    if(0 == CITICs_HsHlp_GetNextRow(hs_handle_, msg_ctrl_.get()))
    {
      json::Value o(json::kObjectType);
      for(int j=0; j<col; j++)
      {
        CITICs_HsHlp_GetColName(hs_handle_,j, key, msg_ctrl_.get());
        CITICs_HsHlp_GetValueByIndex(hs_handle_, j, value, msg_ctrl_.get());

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
  doc.AddMember("Msg", array, allocator);

}

};
