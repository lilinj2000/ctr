#ifndef CTR_TRADESERVICE_IMPL_HH
#define CTR_TRADESERVICE_IMPL_HH

#include "ctr/TradeService.hh"
#include "CITICs_HsT2Hlp.h"
#include "json/json.hh"

namespace ctr
{

class TradeOptions;

class TradeServiceImpl : public TradeService
{
 public:
  TradeServiceImpl(soil::Options* options);

  virtual ~TradeServiceImpl();

  virtual std::string qryStockholder();

  virtual std::string qryStockcode(const std::string& stock_code);

  virtual std::string qryFund();

  virtual std::string qryEntrust(const std::string& entrust_no);
  
  virtual std::string entrust(const std::string& stock_code,
                       int amount,
                       double price,
                       bool buy);

  virtual std::string cancelEntrust(const std::string& entrust_no);

  virtual std::string subscribe();
  
  virtual std::string unsubscribe();
  
 protected:
  
  void initHsApi();

  void login();

  std::string getHsError();

  void fetchRspData(const std::string& rsp, json::Document& doc);

  void setDefaultReqMsg();

  std::string goT2(int func_no, const std::string& func_name,
                   const std::string& func_rsp,
                   const std::string& msg="");

  std::string toExchange(const std::string& stock_code);
  
 private:

  TradeOptions* options_;

  HSHLPCFGHANDLE hs_config_;
  HSHLPHANDLE hs_handle_;

  json::Document rsp_login_;
};


};

#endif
