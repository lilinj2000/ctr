#ifndef CTR_MSGSERVICE_IMPL_HH
#define CTR_MSGSERVICE_IMPL_HH

#include "ctr/MsgService.hh"
#include "HsUtil.hh"

#include <thread>

namespace ctr
{

class MsgOptions;

class MsgServiceImpl : public MsgService, public MsgCallback
{
 public:
  MsgServiceImpl(soil::Options* options);

  virtual ~MsgServiceImpl();

  virtual void msgCallback(const json::Document*);

 protected:

  std::string subscribe();

  std::string unsubscribe();

 private:

  MsgOptions* options_;

  std::unique_ptr<HsUtil> hs_util_;

};


};

#endif
