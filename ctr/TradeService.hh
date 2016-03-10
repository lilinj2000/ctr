#ifndef CTR_TRADESERVICE_HH
#define CTR_TRADESERVICE_HH

#include <string>
#include "soil/Config.hh"

namespace ctr
{

class TradeService
{
 public:
  virtual ~TradeService()
  {
  }

  virtual std::string qryStockholder() = 0;
  
  virtual std::string qryStockcode(const std::string& stock_code) = 0;
  
  virtual std::string qryFund() = 0;

  virtual std::string qryEntrust(const std::string& entrust_no) = 0;

  virtual std::string entrust(const std::string& stock_code,
                       int amount,
                       double price,
                       bool buy) = 0;
                       
  virtual std::string cancelEntrust(const std::string& entrust_no) = 0;

  static soil::Options* createOptions();

  static TradeService* createService(soil::Options* options);
};

};

#endif
