#ifndef CTR_HSUTIL_HH
#define CTR_HSUTIL_HH

#include <string>
#include "CITICs_HsT2Hlp.h"
#include "json/json.hh"

namespace ctr
{

class HsUtil
{
 public:
  HsUtil(const std::string& hs_config);

  virtual ~HsUtil();

  
  std::string goT2(int func_no, const std::string& func_name,
                   const std::string& func_rsp,
                   const std::string& msg="");

  void beginParam();

  void setValue(const std::string& name, const std::string& value);

 protected:
  
  void initHsApi(const std::string& hs_config);

  std::string getHsError();

  void fetchRspData(const std::string& rsp, json::Document& doc);


 private:

  HSHLPCFGHANDLE hs_config_;
  
  HSHLPHANDLE hs_handle_;

};

};

#endif
