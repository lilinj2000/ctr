#ifndef CTR_MSGSERVICE_IMPL_HH
#define CTR_MSGSERVICE_IMPL_HH

#include "ctr/MsgService.hh"
#include "HsUtil.hh"

namespace ctr
{

class MsgOptions;

class MsgServiceImpl : public MsgService
{
 public:
  MsgServiceImpl(soil::Options* options, MsgCallback* callback);

  virtual ~MsgServiceImpl();

 protected:

  std::string subscribe();

  std::string unsubscribe();

 private:

  MsgOptions* options_;

  std::unique_ptr<HsUtil> hs_util_;

  MsgCallback* callback_;

};


};

#endif
