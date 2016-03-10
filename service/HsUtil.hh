#ifndef CTR_HSUTIL_HH
#define CTR_HSUTIL_HH

#include <string>
#include <memory>
#include <thread>
#include "CITICs_HsT2Hlp.h"
#include "json/json.hh"

#include "soil/STimer.hh"
#include "soil/MsgQueue.hh"

namespace ctr
{

class MsgCallback
{
 public:
  virtual ~MsgCallback() {
  }

  virtual void msgCallback(const json::Document*) = 0;
};

class HsUtil
{
 public:
  HsUtil(const std::string& hs_config);

  virtual ~HsUtil();

  
  std::string goT2(int func_no, const std::string& func_name,
                   const std::string& func_rsp,
                   const std::string& msg="");

  void beginParam();

  void setValue(const std::string& name, const std::string& value);

  void startMsgProcess(MsgCallback* callback, int issue_type, int timeout=1000);

  void stopMsgProcess();
  
 protected:

  void msgProcess();

  void initHsApi(const std::string& hs_config);

  std::string getHsError();

  void fetchRspData(const std::string& rsp, json::Document& doc);

  void fetchMsgData(json::Document& doc);

 private:

  HSHLPCFGHANDLE hs_config_;
  
  HSHLPHANDLE hs_handle_;

  volatile bool msg_run_;
  std::unique_ptr<std::thread> msg_thread_;

  std::unique_ptr<soil::MsgQueue<json::Document, MsgCallback> > msg_queue_;

  std::unique_ptr<MSG_CTRL> msg_ctrl_;

  int timeout_;
  std::unique_ptr<soil::STimer> cond_;
};

};

#endif
