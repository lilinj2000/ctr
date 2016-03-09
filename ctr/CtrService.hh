#ifndef CTR_SERVICE_HH
#define CTR_SERVICE_HH

#include "soil/Config.hh"

namespace ctr
{

class CtrService
{
 public:
  virtual ~CtrService()
  {
  }

  virtual void qryStockholder() = 0;

  virtual void qryFund() = 0;

  virtual void qryEntrust(const std::string& entrust_no) = 0;

  virtual void entrust(const std::string& stock_code,
                       int amount,
                       double price,
                       bool buy) = 0;
                       
  virtual void cancelEntrust(const std::string& entrust_no) = 0;
  
  static soil::Options* createOptions();

  static CtrService* createService(soil::Options* options);
};

};

#endif
