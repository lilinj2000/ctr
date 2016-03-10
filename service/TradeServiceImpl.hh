#ifndef CTR_TRADESERVICE_IMPL_HH
#define CTR_TRADESERVICE_IMPL_HH

#include "ctr/TradeService.hh"
#include "json/json.hh"

namespace ctr
{

class TradeOptions;
class HsUtil;

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
  
  void login();

  std::string toExchange(const std::string& stock_code);

  void setDefaultReqMsg();
  
 private:

  TradeOptions* options_;

  std::unique_ptr<HsUtil> hs_util_;
  
  json::Document rsp_login_;
};


};

#endif
