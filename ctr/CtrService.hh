#ifndef CTR_SERVICE_HH
#define CTR_SERVICE_HH

#include "soil/Config.hh"

namespace ctr
{

class CtrService
{
 public:
  virtual ~CtrService()
  {
  }

  static soil::Options* createOptions();

  static CtrService* createService(soil::Options* options);
};

};

#endif
