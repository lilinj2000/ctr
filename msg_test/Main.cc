#include <memory>
#include "MsgTest.hh"

int main(int argc, char* argv[])
{

  std::unique_ptr<MsgTest> test;
  test.reset( new MsgTest() );

  test->run();
  
  return 0;
}
