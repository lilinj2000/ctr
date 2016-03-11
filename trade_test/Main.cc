#include <memory>
#include "TradeTest.hh"

int main(int argc, char* argv[])
{

  std::unique_ptr<TradeTest> test;
  test.reset( new TradeTest() );

  test->run();
  
  return 0;
}
