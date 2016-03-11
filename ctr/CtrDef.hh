#ifndef CTR_DEF_HH
#define CTR_DEF_HH

namespace ctr
{

enum
{
  LOGIN_FUNC = 331100,
  QRY_STOCKHOLDER_FUNC = 331300,
  QRY_STOCKCODE_FUNC = 330300,
  QRY_FUND_FUNC = 332255,
  QRY_ENTRUST_FUNC = 333101,
  ENTRUST_FUNC = 333002,
  CANCEL_ENTRUST_FUNC = 333017,

  SUBSCRIBE_FUNC = 620001,
  UNSUBSCRIBE_FUNC = 620002
};

static const constexpr char* RSP_LOGIN = "LoginRsp";
static const constexpr char* RSP_QRY_STOCKHOLDER = "QryStockholderRsp";
static const constexpr char* RSP_QRY_STOCKCODE = "QryStockcodeRsp";
static const constexpr char* RSP_QRY_FUND = "QryFundRsp";
static const constexpr char* RSP_QRY_ENTRUST = "QryEntrustRsp";
static const constexpr char* RSP_ENTRUST = "EntrustRsp";
static const constexpr char* RSP_CANCEL_ENTRUST = "CancelEntrustRsp";

static const constexpr char* RSP_SUBSCRIBE = "SubscribeRsp";
static const constexpr char* RSP_UNSUBSCRIBE = "UnsubscribeRsp";

static const constexpr char* RECORD = "Record";
static const constexpr char* MESSAGE = "Msg";

};

#endif
