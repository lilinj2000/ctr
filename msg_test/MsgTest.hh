#ifndef MSG_TEST_HH
#define MSG_TEST_HH

#include <memory>
#include "ctr/MsgService.hh"
#include "soil/STimer.hh"

class MsgTest
{
 public:
  MsgTest();

  virtual ~MsgTest();

  void run();

 protected:
  
 private:
  std::unique_ptr<ctr::MsgService> service_;

  std::unique_ptr<soil::Options> options_;

  std::unique_ptr<soil::STimer> cond_;
};


#endif
