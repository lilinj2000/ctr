#ifndef CTR_SERVICE_IMPL_HH
#define CTR_SERVICE_IMPL_HH

#include "ctr/CtrService.hh"

#include "CITICs_HsT2Hlp.h"
#include "rapidjson/document.h"

namespace ctr
{

class CtrOptions;

class CtrServiceImpl : public CtrService
{
 public:
  CtrServiceImpl(soil::Options* options);

  virtual ~CtrServiceImpl();

  virtual void qryStockholder();

  virtual void qryFund();

  virtual void qryEntrust(const std::string& entrust_no);
  
  virtual void entrust(const std::string& stock_code,
                       int amount,
                       double price,
                       bool buy);

  virtual void cancelEntrust(const std::string& entrust_no);
  
 protected:
  
  void initHsApi();

  void login();

  std::string getHsError();

  void fetchRspData(const std::string& rsp, rapidjson::Document& doc);

  std::string jsonToString(const rapidjson::Document& doc);

  void setDefaultReqMsg();

  std::string toExchange(const std::string& stock_code);
  
 private:

  CtrOptions* options_;

  HSHLPCFGHANDLE hs_config_;
  HSHLPHANDLE hs_handle_;

  rapidjson::Document rsp_login_;
};


};

#endif
