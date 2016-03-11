#ifndef TRADE_TEST_HH
#define TRADE_TEST_HH

#include <memory>
#include "ctr/TradeService.hh"
#include "soil/STimer.hh"

class TradeTest
{
 public:
  TradeTest();

  virtual ~TradeTest();

  void run();

 protected:
  
  void menu(int&);

 private:
  std::unique_ptr<ctr::TradeService> service_;

  std::unique_ptr<soil::Options> options_;

  std::unique_ptr<soil::STimer> cond_;
};


#endif
