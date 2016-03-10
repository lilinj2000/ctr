#include "gtest/gtest.h"

#include "service/CtrLog.hh"
#include "ctr/TradeService.hh"

#include "soil/STimer.hh"

namespace ctr
{

class TradeServiceTest : public ::testing::Test
{
 public:
  TradeServiceTest()
  {
  }
  
  void SetUp()
  {
    options_.reset( TradeService::createOptions() );

    std::unique_ptr<soil::Config> config( soil::Config::create() );
    config->configFile() = "trade.cfg";
    config->registerOptions( options_.get() );
    config->loadConfig();

    CTR_LOG_INIT("log.cfg");

    cond_.reset( soil::STimer::create() );

    service_.reset( TradeService::createService(options_.get()) );

  }

  void TearDown()
  {
  }

 protected:
  std::unique_ptr<TradeService> service_;
  
  std::unique_ptr<soil::Options> options_;
  
  std::unique_ptr<soil::STimer> cond_;

};

TEST_F(TradeServiceTest, loginTest)
{
  ASSERT_TRUE( true );
}

TEST_F(TradeServiceTest, qryStockholderTest)
{
  try
  {
    CTR_DEBUG <<"query stockholder rsp:\n"
              <<service_->qryStockholder();
  
    ASSERT_TRUE( true );
  }
  catch( ... )
  {
    ASSERT_TRUE( false );
  }
}

TEST_F(TradeServiceTest, qryStockcodeTest)
{
  try
  {
    // std::string stock_code = "601398";
    std::string stock_code = "000088";

    CTR_DEBUG <<"query stockcode rsp:\n"
              <<service_->qryStockcode( stock_code );
  
    ASSERT_TRUE( true );
  }
  catch( ... )
  {
    ASSERT_TRUE( false );
  }

}

TEST_F(TradeServiceTest, qryFundTest)
{
  try
  {
    CTR_DEBUG <<"query fund rsp:\n"
              <<service_->qryFund();
  
    ASSERT_TRUE( true );
  }
  catch( ... )
  {
    ASSERT_TRUE( false );
  }

}

TEST_F(TradeServiceTest, qryEntrustTest)
{
  try
  {
    std::string entrust_no = "7864";
    // std::string entrust_no = "7874";
  
    CTR_DEBUG <<"query entrust:\n"
              <<service_->qryEntrust( entrust_no );
  
    ASSERT_TRUE( true );
  }
  catch( ... )
  {
    ASSERT_TRUE( false );
  }
  
}

TEST_F(TradeServiceTest, entrustTest)
{
  try
  {
    // std::string stock_code = "601398";
    std::string stock_code = "000088";
    int amount = 100;
    double price = 1.0;

    bool buy = true;
  
    CTR_DEBUG <<"entrust rsp:\n"
              <<service_->entrust(stock_code, amount, price, buy);
  
    ASSERT_TRUE( true );
  }
  catch( ... )
  {
    ASSERT_TRUE( false );
  }

}

TEST_F(TradeServiceTest, cancelEntrustTest)
{
  try
  {
    std::string entrust_no = "7864";
    // std::string entrust_no = "7874";
  
    CTR_DEBUG <<"cancel entrust rsp:\n"
              <<service_->cancelEntrust( entrust_no );
  
    ASSERT_TRUE( true );
  }
  catch( ... )
  {
    ASSERT_TRUE( false );
  }
  
}


}; 
