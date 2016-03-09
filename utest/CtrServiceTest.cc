#include "gtest/gtest.h"

#include "service/CtrLog.hh"
#include "ctr/CtrService.hh"

#include "soil/STimer.hh"

namespace ctr
{

class CtrServiceImplTest : public ::testing::Test
{
 public:
  CtrServiceImplTest()
  {
  }
  
  void SetUp()
  {
    options_.reset( CtrService::createOptions() );

    std::auto_ptr<soil::Config> config( soil::Config::create() );
    config->configFile() = "ctr.cfg";
    config->registerOptions( options_.get() );
    config->loadConfig();

    CTR_LOG_INIT("log.cfg");

    cond_.reset( soil::STimer::create() );

    service_.reset( CtrService::createService(options_.get()) );

  }

  void TearDown()
  {
  }

 protected:
  std::auto_ptr<CtrService> service_;
  
  std::auto_ptr<soil::Options> options_;
  
  std::auto_ptr<soil::STimer> cond_;

};

TEST_F(CtrServiceImplTest, loginTest)
{
  ASSERT_TRUE( true );
}

TEST_F(CtrServiceImplTest, qryStockholderTest)
{
  service_->qryStockholder();
  
  ASSERT_TRUE( true );
}

TEST_F(CtrServiceImplTest, qryFundTest)
{
  service_->qryFund();
  
  ASSERT_TRUE( true );
}

TEST_F(CtrServiceImplTest, qryEntrustTest)
{
  std::string entrust_no = "7864";
  // std::string entrust_no = "7874";
  
  service_->qryEntrust( entrust_no );
  
  ASSERT_TRUE( true );
}

TEST_F(CtrServiceImplTest, entrustTest)
{
  // std::string stock_code = "601398";
  std::string stock_code = "000088";
  int amount = 100;
  double price = 1.0;

  bool buy = true;
  
  service_->entrust(stock_code, amount, price, buy);
  
  ASSERT_TRUE( true );
}

TEST_F(CtrServiceImplTest, cancelEntrustTest)
{
  std::string entrust_no = "7864";
  // std::string entrust_no = "7874";
  
  service_->cancelEntrust( entrust_no );
  
  ASSERT_TRUE( true );
}

}; 
