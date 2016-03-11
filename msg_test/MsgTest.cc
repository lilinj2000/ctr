#include "MsgTest.hh"
#include "soil/Log.hh"
#include "soil/String.hh"

#include <iostream>

MsgTest::MsgTest()
{
  options_.reset( ctr::MsgService::createOptions() );

  std::unique_ptr<soil::Config> config( soil::Config::create() );
  config->configFile() = "msg.cfg";
  config->registerOptions( options_.get() );
  config->loadConfig();

  SOIL_LOG_INIT("log.cfg");

  cond_.reset( soil::STimer::create() );

  service_.reset( ctr::MsgService::createService(options_.get()) );
}

MsgTest::~MsgTest()
{
}

void MsgTest::run()
{
  while( true )
  {
    cond_->wait( 2000 );
  }
}

