#ifndef CTR_MSGSERVICE_HH
#define CTR_MSGSERVICE_HH

#include "soil/Config.hh"
#include "json/json.hh"

namespace ctr
{

class MsgCallback
{
 public:
  virtual ~MsgCallback() {
  }

  virtual void msgCallback(const json::Document*) = 0;
};

class MsgService
{
 public:
  virtual ~MsgService()
  {
  }

  static soil::Options* createOptions();

  static MsgService* createService(soil::Options* options, MsgCallback* callback);

};

};

#endif
