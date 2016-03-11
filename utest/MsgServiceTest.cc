#include "gtest/gtest.h"

#include "service/CtrLog.hh"
#include "ctr/MsgService.hh"

#include "soil/STimer.hh"

namespace ctr
{

class MsgServiceTest : public ::testing::Test, public MsgCallback
{
 public:
  MsgServiceTest()
  {
  }
  
  void SetUp()
  {
    options_.reset( MsgService::createOptions() );

    std::unique_ptr<soil::Config> config( soil::Config::create() );
    config->configFile() = "msg.cfg";
    config->registerOptions( options_.get() );
    config->loadConfig();

    CTR_LOG_INIT("log.cfg");

    cond_.reset( soil::STimer::create() );

    service_.reset( MsgService::createService(options_.get(), this) );

  }

  void TearDown()
  {
  }

  virtual void msgCallback(const json::Document* doc)
  {
    CTR_INFO <<"msg received:\n"
             <<json::toString(*doc);
  }

 protected:
  std::unique_ptr<MsgService> service_;
  
  std::unique_ptr<soil::Options> options_;
  
  std::unique_ptr<soil::STimer> cond_;

};

TEST_F(MsgServiceTest, msgTest)
{
  ASSERT_TRUE( true );
}




}; 
