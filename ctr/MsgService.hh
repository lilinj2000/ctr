#ifndef CTR_MSGSERVICE_HH
#define CTR_MSGSERVICE_HH

#include "soil/Config.hh"

namespace ctr
{

class MsgService
{
 public:
  virtual ~MsgService()
  {
  }

  static soil::Options* createOptions();

  static MsgService* createService(soil::Options* options);

};

};

#endif
