#include "gtest/gtest.h"

#include "service/CtrLog.hh"
#include "service/CtrServiceImpl.hh"

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


}; 
