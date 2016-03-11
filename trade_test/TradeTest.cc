#include "TradeTest.hh"
#include "soil/Log.hh"
#include "soil/String.hh"

#include <iostream>

TradeTest::TradeTest()
{
  options_.reset( ctr::TradeService::createOptions() );

  std::unique_ptr<soil::Config> config( soil::Config::create() );
  config->configFile() = "trade.cfg";
  config->registerOptions( options_.get() );
  config->loadConfig();

  SOIL_LOG_INIT("log.cfg");

  cond_.reset( soil::STimer::create() );

  service_.reset( ctr::TradeService::createService(options_.get()) );
}

TradeTest::~TradeTest()
{
}

void TradeTest::run()
{
  int item = -1;

  do
  {
    menu( item );

    try
    {
      std::string rsp;

      switch( item )
      {
        case 0:
          return;
        
        case 1:
          rsp = service_->qryStockholder();
          break;

        case 2:
          {
            std::string stock_code;
            std::cout <<"please input the stock code: ";
            std::cin >>stock_code;
            rsp = service_->qryStockcode( stock_code );
            break;
          }

        case 3:
          rsp = service_->qryFund();
          break;

        case 4:
          {
            std::string entrust_no;
            std::cout <<"please input the entrust no: ";
            std::cin >>entrust_no;
            rsp = service_->qryEntrust( entrust_no );
            break;
          }

        case 5:
          {
            std::string stock_code;
            int amount = 0;
            double price = 0;
            std::string buy;
          
            std::cout <<"please input the stock code: ";
            std::cin >>stock_code;

            std::cout <<"please input the amount: ";
            std::cin >>amount;

            std::cout <<"please input the price: ";
            std::cin >>price;

            std::cout <<"does buy this " <<stock_code <<" ? ";
            std::cin >>buy;

            soil::toUpper(buy);
            bool is_buy = (buy=="YES")? true : false;
            rsp = service_->entrust( stock_code, amount, price, is_buy );
          
            break;
          }

        case 6:
          {
            std::string entrust_no;
            std::cout <<"please input the entrust no: ";
            std::cin >>entrust_no;
            rsp = service_->cancelEntrust( entrust_no );
            break;
          }
      }

      SOIL_INFO <<"\n"
                <<rsp ;
    }
    catch( std::exception& e)
    {
      SOIL_ERROR <<e.what();
    }
    
  }while( item!=0 );
}
void TradeTest::menu(int &item)
{
  do
  {
    std::cout <<"======== trade test ========\n";
    std::cout <<"1. query stock holder \n"
              <<"2. query stock code \n"
              <<"3. query fund \n"
              <<"4. query entrust \n"
              <<"5. entrust \n"
              <<"6. cancel entrust \n"
              <<"0. quit \n"
              <<"Please input the item: ";

    std::cin >>item;
    
  }while( item<0 || item>6);

  return ;
}
