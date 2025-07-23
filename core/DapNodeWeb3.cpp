/* INCLUDES */
#include "DapNodeWeb3.h"
#include "DapLogger.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QTimer>
#include <QDebug>

#include "DapServiceDataLocal.h"

#include <QFile>
#include <QTextStream>
/* DEFS */
//#define ENABLE_SENSITIVE_PRINTS
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
#define SIGERROR (110)
#elif defined(Q_OS_MACOS)
#define SIGERROR (-668)
#elif defined (Q_OS_WIN)
#define SIGERROR (10061)
#elif defined Q_OS_ANDROID
#define SIGERROR (111)
#endif

#define DEBUGINFO qDebug()<<"--->Web3<---"
//#define ENABLE_BAD_REPLY_WALLETS

typedef DapNodeWeb3::ReplyMethodID ReplyMethodID;

struct ReplyMethod
{
  ReplyMethodID parseMethodID;
  bool replyError;
  QString wrongReplyText;
  int baseErrorCode;
};

/* VARS */
const QString  DapNodeWeb3::WEB3_URL ("localhost");
const quint16  DapNodeWeb3::WEB3_PORT (8045);

static const QHash<QString, ReplyMethod> s_replyMethodMap =
{
  // connect reply
  { "Connect",            {ReplyMethodID::ParseReplyConnect,      true,  "Wrong reply connect", 100000}},
  // node status reply
  { "GetNodeStatus",      {ReplyMethodID::ParseReplyStatus,       true,  "Wrong reply status", 200000}},
  // wallets list reply
  { "GetWallets",         {ReplyMethodID::ParseReplyWallets,      false, "Wrong reply when get wallets list", 300000}},
  // networks list reply
  { "GetNetworks",        {ReplyMethodID::ParseReplyNetworks,     false, "Wrong reply when get networks list", 400000}},
  // data wallets reply
  { "GetDataWallet",      {ReplyMethodID::ParseDataWallet,        false, "Wrong reply when get data wallets", 500000}},
  // get certificates
  { "GetCertificates",    {ReplyMethodID::ParseCertificates,      false, "Wrong reply when get certificates", 600000}},
  // create certificate
  { "CreateCertificate",  {ReplyMethodID::ParseCreateCertificate, false, "Wrong reply when create certificates", 700000}},
  // create cond transaction reply
  { "CondTxCreate",       {ReplyMethodID::ParseCondTxCreateReply, false, "Wrong reply when create transaction", 800000}},
  // check mempool
  { "GetMempoolTxHash",   {ReplyMethodID::ParseMempoolReply,      false, "Wrong reply when get mempool transaction hash", 900000}},
  // check ledger
  { "GetLedgerTxHash",    {ReplyMethodID::ParseLedgerReply,       false, "Wrong reply when get ledger transaction hash", 1000000}},
  // get orders
  { "GetOrdersList",      {ReplyMethodID::ParseOrderList,         false, "Wrong reply when get orders list", 1100000}},
  // get ip
  { "GetNodeIP",          {ReplyMethodID::ParseNodeIp,            false, "Wrong reply when get node ip", 1200000}},
  // get fee
  { "GetFee",             {ReplyMethodID::ParseFee,               false, "Wrong reply when get fee", 1300000}},
  // node dump
  { "NodeList",           {ReplyMethodID::ParseNodeList,          false, "Wrong reply when get node dump", 1400000}},
  // list of delegated keys on the network
  { "GetListKeys",        {ReplyMethodID::ParseListKeys,          false, "Wrong reply when get list of delegated keys", 1500000}},

  { "GetNetId",           {ReplyMethodID::ParseNetId,             false, "Wrong reply when get net id", 1600000}}
};

static const ReplyMethod s_dummyReply
{
  ReplyMethodID::Invalid,
  false,
  QString(),
  0
};

DapNodeWeb3::DapNodeWeb3 (QObject *obj, int requestTimeout) :
  QObject (obj),
  m_requestTimeout (requestTimeout),
  m_networkReply (nullptr),
  m_parseJsonError (false),
  m_reconnectionAttempts (0)
{
  // Initialize HTTP client
  m_httpClient = new DapNetworkAccessManager();
  
  // Load stored connection ID on startup
  QString storedId = loadStoredConnectionId();
  if (!storedId.isEmpty()) {
    m_connectId = storedId;
    qDebug() << "🔗 [WEB3 ID] Loaded stored connection ID:" << m_connectId;
  }
}


DapNodeWeb3::~ DapNodeWeb3()
{
}

void DapNodeWeb3::request_GET(const QString &host, quint16 port, const QString &urlPath,
                              DapNetworkReply &a_netReply, const QString &headers, bool ssl)
{
    Q_UNUSED(ssl)

    qDebug() << "Sending GET request to" << host << ":" << port << urlPath << "with headers:" << headers;

    if (!m_httpClient) {
        qWarning() << "m_httpClient is null, cannot process GET request";
        return;
    }

    m_httpClient->requestHttp_GET(host, port, urlPath, headers, a_netReply);
}

void DapNodeWeb3::request_GET_long_timeout(const QString &host, quint16 port, const QString &urlPath,
                              DapNetworkReply &a_netReply, const QString &headers, bool ssl)
{
    Q_UNUSED(ssl)

    qDebug() << "Sending GET request with long timeout to" << host << ":" << port << urlPath << "with headers:" << headers;

    if (!m_httpClient) {
        qWarning() << "m_httpClient is null, cannot process GET request";
        return;
    }

    m_httpClient->requestHttp_GET_long_timeout(host, port, urlPath, headers, a_netReply);
}

QString extractMethod (const QString &inputString)
{
  QRegularExpression regex (R"(\?method=([^&]+))");
  QRegularExpressionMatch match = regex.match (inputString);

  if (match.hasMatch())
    return match.captured (1);
  else
    return QString(); // If no match is found
}

void DapNodeWeb3::sendRequest(QString request)
{
    m_networkReply = new DapNetworkReply(this);

    connect(m_networkReply, &DapNetworkReply::finished, this, [this]() {
        responseProcessing(m_networkReply->error(), m_networkReply->errorString());
    });

    connect(m_networkReply, &DapNetworkReply::sigError, this, [this]() {
        responseProcessing(m_networkReply->error(), m_networkReply->errorString(), false);
    });

    m_networkRequest = request;
    
    // Use long timeout for transaction creation operations
    QString methodName = extractMethod(request);
    if (methodName == "CondTxCreate") {
        DEBUGINFO << "Using long timeout for CondTxCreate operation";
        request_GET_long_timeout(WEB3_URL, WEB3_PORT, request, *m_networkReply);
    } else {
        request_GET(WEB3_URL, WEB3_PORT, request, *m_networkReply);
    }
}

bool isVersionLessThan(const QString& version1, const QString& version2) {
    QStringList parts1 = version1.split(QRegExp("[-\\.]"));
    QStringList parts2 = version2.split(QRegExp("[-\\.]"));

    int length = qMax(parts1.size(), parts2.size());
    for (int i = 0; i < length; ++i) {
        int num1 = (i < parts1.size()) ? parts1[i].toInt() : 0;
        int num2 = (i < parts2.size()) ? parts2[i].toInt() : 0;

        if (num1 < num2) {
            return true;
        } else if (num1 > num2) {
            return false;
        }
    }
    return false;
}

void DapNodeWeb3::responseProcessing (
  const int error,
  const QString errorString,
  const bool httpFinished)
{
  // get network request
  QString networkRequest  = std::move (m_networkRequest);
  QString methodName      = extractMethod (networkRequest);
  DEBUGINFO  << "DapNodeWeb3::responseProcessing" << networkRequest;

  // debug info
  // node connection reply
  if (methodName == "GetVersions")
    {
        if (error == SIGERROR)
        {
          DEBUGINFO  << "DapNodeWeb3::responseProcessing - nodeNotDetected";
          emit nodeNotDetected();
          return;
        }

        QJsonDocument doc = QJsonDocument::fromJson (m_networkReply->getReplyData());

        if (doc["status"].isString()) {
            if (doc["status"].toString() == QString("ok")) {
                QJsonObject dataObject = doc["data"].toObject();
                
                // Check for either cellframe-dashboard or cellframe-wallet
                QString cellframeDashboard;
                QString dashboardType;
                if (dataObject.contains("cellframe-dashboard") && !dataObject["cellframe-dashboard"].toString().isEmpty()) {
                    cellframeDashboard = dataObject["cellframe-dashboard"].toString();
                    dashboardType = "dashboard";
                    m_connectedAppType = "Dashboard"; // For UI display
                } else if (dataObject.contains("cellframe-wallet") && !dataObject["cellframe-wallet"].toString().isEmpty()) {
                    cellframeDashboard = dataObject["cellframe-wallet"].toString();
                    dashboardType = "wallet";
                    m_connectedAppType = "Cellframe-Wallet"; // For UI display
                }
                
                QString cellframeNode = dataObject["cellframe-node"].toString();

                QString minDashboardVersion = DapServiceDataLocal::instance()->getMinDashboardVersion();
                QString minNodeVersion = DapServiceDataLocal::instance()->getMinNodeVersion();

                bool isDashboardLess = !cellframeDashboard.isEmpty() ? isVersionLessThan(cellframeDashboard, minDashboardVersion) : false;
                bool isNodeLess = isVersionLessThan(cellframeNode, minNodeVersion);

                DEBUGINFO << "nodeStatusOk - " + doc["data"].toString();
                DEBUGINFO << "cellframe-" << dashboardType << " version: " << cellframeDashboard;
                DEBUGINFO << "cellframe-node version: " << cellframeNode;

                // If we have either dashboard or wallet, consider it as normal
                if ((!cellframeDashboard.isEmpty() && !isDashboardLess) && !isNodeLess) {
                    DEBUGINFO << "Node detected. No updates required.";
                    emit nodeDetected();
                    return;
                } else if ((!cellframeDashboard.isEmpty() && isDashboardLess) && isNodeLess) {
                    QString errorMessage = QString("Update required for both %1 and node.").arg(dashboardType);
                    DEBUGINFO << errorMessage;
                    emit sigError(232, errorMessage);
                } else {
                    if (!cellframeDashboard.isEmpty() && isDashboardLess) {
                        QString errorMessage = QString("Update required for %1. ").arg(dashboardType) +
                                               "Current version: " + cellframeDashboard + ", required: " + minDashboardVersion + ".";
                        DEBUGINFO << errorMessage;
                        emit sigError(233, errorMessage);
                    }
                    if (isNodeLess) {
                        QString errorMessage = QString("Update required for node. ") +
                                               "Current version: " + cellframeNode + ", required: " + minNodeVersion + ".";
                        DEBUGINFO << errorMessage;
                        emit sigError(234, errorMessage);
                    }
                    // If neither dashboard nor wallet is present, but node is present, still consider it as detected
                    if (cellframeDashboard.isEmpty() && !isNodeLess) {
                        DEBUGINFO << "Node detected. No dashboard/wallet component found, but node is up to date.";
                        emit nodeDetected();
                        return;
                    }
                }
            } else {
                QString errorMessage = "Invalid status value";
                DEBUGINFO << errorMessage;
                emit sigError(230, errorMessage);
            }
        } else {
            QString errorMessage = "Status not found in JSON response";
            DEBUGINFO << errorMessage;
            emit sigError(231, errorMessage);
        }
    }
  else if (methodName == "Connect")
    {
      if (error == -668)
        {
          DEBUGINFO  << __func__ << " - Dashboard refused authorization";
          emit sigError (error, "Dashboard refused authorization");
          //To send a repeat request
          nodeConnectionRequest();
          return;
        }
    }

  /* get reply method item */
  auto replyMethod  = s_replyMethodMap.value (methodName, s_dummyReply);

  /* check */
  if (replyMethod.parseMethodID == ReplyMethodID::Invalid)
    {
      DEBUGINFO  << "DapNodeWeb3::responseProcessing :: Unknown method:" << methodName;
      return;
    }

  DEBUGINFO  << "DapNodeWeb3::responseProcessing :: Method ID:" << int (replyMethod.parseMethodID);

  /* parse */
  responseParsing (
    error, errorString, httpFinished,
    replyMethod.baseErrorCode,
    replyMethod.wrongReplyText,
    replyMethod.parseMethodID,
    replyMethod.replyError);

//  for (int k = 0; k < replyItems.count(); k++)
//  {
//    auto* r = &replyItems[k];
//    if (networkRequest.contains(r->request))
//    {
//      responseParsing(error, errorString, httpFinished, r->baseErrorCode,
//                      r->wrongReplyText, r->parseMethod, r->replyError);
//      return;
//    }
//  }
}

void DapNodeWeb3::nodeDetectedRequest()
{
  DEBUGINFO << "http://127.0.0.1:8045/ nodeDetectedRequest";
  // A non-existent method is used to check the operation of the dashboard,
  // in the case of the operation of the dashboard service, a response is returned:
  //    {
  //        "data": "",
  //        "errorMsg": "Incorrect id",
  //        "status": "bad"
  //    }
  nodeVersionsRequest();
}

void DapNodeWeb3::nodeConnectionRequest()
{
  // do not send connect if already present
  if (!m_connectId.isEmpty()) {
    qDebug() << "🔗 [WEB3 ID] Using existing connection ID:" << m_connectId;
    return emit connectionIdReceived (m_connectId);
  }

  // Try to load stored connection ID
  QString storedId = loadStoredConnectionId();
  if (!storedId.isEmpty()) {
    m_connectId = storedId;
    qDebug() << "🔗 [WEB3 ID] Using stored connection ID:" << m_connectId;
    return emit connectionIdReceived (m_connectId);
  }

  // No stored ID found, request new connection
  qDebug() << "🔗 [WEB3 ID] No stored connection ID, requesting new connection";
  // example connect request
  // http://127.0.0.1:8045/?method=Connect
  DEBUGINFO << "http://127.0.0.1:8045/?method=Connect";
  sendRequest ("?method=Connect");
}

void DapNodeWeb3::replyConnectError (int code)
{
  Q_UNUSED (code)
  emit nodeNotConnected();
}

void DapNodeWeb3::responseParsing (
  const int error,
  const QString wrongReplyerrorString,
  const bool httpFinished,
  int baseErrorCode,
  QString messageReplyDataError,
//    void(DapNodeWeb3::*parseMethod)(const QString&, int baseErrorCode, const QString&),
//    void(DapNodeWeb3::*replyError)(int error),
  ReplyMethodID parseMethod,
  bool responceError)
{
  /* on any error */
  if (error != QNetworkReply::NetworkError::NoError)
    {
      if (responceError)
        replyConnectError (error);

      if (httpFinished)
        replyError (baseErrorCode + 10000 + error, wrongReplyerrorString, wrongReplyerrorString);
      else
        replyError (baseErrorCode + error, messageReplyDataError, messageReplyDataError);

      return;
    }

  // no error reply
  auto reply = m_networkReply->getReplyData();
//  qInfo() << "reply message: " << reply;

  // check reply message error
  if (reply.size() == 0)
    return replyError (baseErrorCode + 20000, messageReplyDataError, messageReplyDataError);

  // parse reply
  switch (parseMethod)
    {
    case ReplyMethodID::Invalid: break;
    case ReplyMethodID::ParseReplyConnect:      parseReplyConnect (reply,       baseErrorCode); break;
    case ReplyMethodID::ParseReplyStatus:       parseReplyStatus (reply,        baseErrorCode); break;
    case ReplyMethodID::ParseReplyWallets:      parseReplyWallets (reply,       baseErrorCode); break;
    case ReplyMethodID::ParseReplyNetworks:     parseReplyNetworks (reply,      baseErrorCode); break;
    case ReplyMethodID::ParseDataWallet:        parseDataWallet (reply,         baseErrorCode); break;
    case ReplyMethodID::ParseCertificates:      parseCertificates (reply,       baseErrorCode); break;
    case ReplyMethodID::ParseCreateCertificate: parseCreateCertificate (reply,  baseErrorCode); break;
    case ReplyMethodID::ParseCondTxCreateReply: parseCondTxCreateReply (reply,  baseErrorCode); break;
    case ReplyMethodID::ParseMempoolReply:      parseMempoolReply (reply,       baseErrorCode); break;
    case ReplyMethodID::ParseLedgerReply:       parseLedgerReply (reply,        baseErrorCode); break;
    case ReplyMethodID::ParseOrderList:         parseOrderList (reply,          baseErrorCode); break;
    case ReplyMethodID::ParseNodeIp:            parseNodeIp (reply,             baseErrorCode); break;
    case ReplyMethodID::ParseFee:               parseFee (reply,                baseErrorCode); break;
    case ReplyMethodID::ParseNodeList:          parseNodeList (reply,           baseErrorCode); break;
    case ReplyMethodID::ParseListKeys:          parseListKeys (reply,           baseErrorCode); break;
    case ReplyMethodID::ParseNetId:             parseNetId (reply,              baseErrorCode); break;
    }
}

void DapNodeWeb3::nodeStatusRequest()
{
  QString requesString = QString ("?method=GetNodeStatus&id=%1").arg (m_connectId);
  sendRequest (requesString);
}

void DapNodeWeb3::nodeDetectRequest()
{
//  QString requesString = QString ("?");
  sendRequest ("?");
}

void DapNodeWeb3::nodeVersionsRequest()
{
    QString requesString = QString ("?method=GetVersions").arg (m_connectId);
    sendRequest (requesString);
}

void DapNodeWeb3::walletsRequest()
{
  //http://127.0.0.1:8045/?method=GetWallets&id=0x7D6878E27691240294BE6DE326F56FD1344087024F0FBBEA98B4A04321D21F73
  QString requesString = QString ("?method=GetWallets&id=%1").arg (m_connectId);
  sendRequest (requesString);
}

void DapNodeWeb3::networksRequest()
{
  QString requesString = QString ("?method=GetNetworks&id=%1").arg (m_connectId);
  sendRequest (requesString);
}

void DapNodeWeb3::condTxCreateRequest (QString walletName, QString networkName, QString sertificateName, QString tokenName, QString value, QString unit)
{
  m_networkName = networkName;
  QString requesString = QString ("?method=CondTxCreate&"
                                  "id=%1&"
                                  "net=%2&"
                                  "tokenName=%3&"
                                  "walletName=%4&"
                                  "certName=%5&"
                                  "value=%6&"
                                  "fee=&"
                                  "unit=%7&"
                                  "srv_uid=1")
                         .arg (m_connectId)
                         .arg (networkName)
                         .arg (tokenName)
                         .arg (walletName)
                         .arg (sertificateName)
                         .arg (value)
                         .arg (unit);
  sendRequest (requesString);
}

void DapNodeWeb3::createCertificate (const QString &certType, const QString &certName)
{
  QString requesString = QString ("?method=CreateCertificate&"
                                  "id=%1&"
                                  "certType=%2&"
                                  "certName=%3&"
                                  "categoryCert=%4")
                         .arg (m_connectId)
                         .arg (certType)
                         .arg (certName)
                         .arg ("public");
  sendRequest (requesString);
}

void DapNodeWeb3::getCertificates()
{
  QString requesString = QString ("?method=GetCertificates&"
                                  "id=%1&"
                                  "categoryCert=%2")
                         .arg (m_connectId)
                         .arg ("public");
  sendRequest (requesString);
}

void DapNodeWeb3::getMempoolTxHashRequest (QString transactionHash, QString networkName)
{
  QString requesString = QString ("?method=GetMempoolTxHash&"
                                  "id=%1&net=%2&hashTx=%3")
                         .arg (m_connectId)
                         .arg (networkName)
                         .arg (transactionHash);
  sendRequest (requesString);
}

void DapNodeWeb3::getLedgerTxHashRequest (QString transactionHash, QString networkName)
{
  QString requesString = QString ("?method=GetLedgerTxHash&"
                                  "id=%1&hashTx=%2&net=%3")
                         .arg (m_connectId)
                         .arg (transactionHash)
                         .arg (networkName);
  sendRequest (requesString);
}

void DapNodeWeb3::getOrdersListRequest (QString networkName, QString tokenName, QString minPrice, QString maxPrice, QString unit)
{
  m_networkName = networkName;
  QString requesString = QString ("?method=GetOrdersList&"
                                  "id=%1&direction=%2&srv_uid=1")
                         .arg (m_connectId)
                         .arg ("sell");
  if (!networkName.isEmpty())
    requesString += QString ("&net=%1").arg (networkName);
  if (!tokenName.isEmpty())
    requesString += QString ("&tokenName=%1").arg (tokenName);
  if (!minPrice.isEmpty())
    requesString += QString ("&price_min=%1").arg (minPrice);
  if (!maxPrice.isEmpty())
    requesString += QString ("&price_max=%1").arg (maxPrice);
  if (!unit.isEmpty())
    requesString += QString ("&unit=%1").arg (unit);
  qDebug() << "OrdersListRequest" << requesString;
  sendRequest (requesString);
}

void DapNodeWeb3::getNodeIPRequest (const QString &networkName, const QJsonArray &orderList)
{
  auto addressArgument = [] (const QJsonArray & list) -> QString
  {
    return list.size() > 1
    ? "jsonArray=" + QJsonDocument (list).toJson (QJsonDocument::Compact)
    : "addr=" + list.begin()->toString();
  };

  qDebug() << "🔍 [IP DEBUG] getNodeIPRequest for network:" << networkName << "with" << orderList.size() << "nodes";
  
  // Log all node addresses being requested
  for (int i = 0; i < orderList.size(); ++i) {
    qDebug() << "🔍 [IP DEBUG] Requesting IP for node" << (i+1) << "of" << orderList.size() << ":" << orderList[i].toString();
  }

  QString requesString =
    QString ("?method=GetNodeIP"
             "&id=%1"
             "&net=%2&%3")
    .arg (m_connectId, networkName, addressArgument (orderList));

  qDebug() << "🔍 [IP DEBUG] Sending GetNodeIP request:" << requesString;
  sendRequest (requesString);
}

void DapNodeWeb3::getFeeRequest (QString networkName)
{
  m_networkName = networkName;
  QString requesString = QString ("?method=GetFee&"
                                  "id=%1&net=%2")
                         .arg (m_connectId)
                         .arg (networkName);
  sendRequest (requesString);
}

void DapNodeWeb3::getNetIdRequest (QString networkName)
{
  QString requesString = QString ("?method=GetNetId&"
                                  "id=%1&net=%2")
                         .arg (m_connectId)
                         .arg (networkName);
  sendRequest (requesString);
}

void DapNodeWeb3::nodeListRequest (QString networkName)
{
  QString requesString = QString ("?method=NodeList&"
                                  "id=%1&net=%2")
                         .arg (m_connectId)
                         .arg (networkName);
  sendRequest (requesString);
}

void DapNodeWeb3::getListKeysRequest (QString networkName)
{
  QString requesString = QString ("?method=GetListKeys&"
                                  "id=%1&net=%2")
                         .arg (m_connectId)
                         .arg (networkName);
  sendRequest (requesString);
}


void DapNodeWeb3::parseReplyStatus (const QString &replyData, int baseErrorCode)
{
  Q_UNUSED(baseErrorCode)

  DEBUGINFO << __func__ << replyData;

  QJsonDocument doc = QJsonDocument::fromJson (replyData.toUtf8());

  if (doc["status"].isString())
    {
      if (doc["status"].toString() == QString ("ok"))
        {
          DEBUGINFO << "nodeStatusOk";
          emit statusOk();
          return;
        }
    }
}

void DapNodeWeb3::parseReplyConnect (const QString &replyData, int baseErrorCode)
{
  // connect reply example
  //    "{"
  //    "    \"data\": {"
  //    "        \"id\": \"0xCD33094D27F8ACEBBCC90B31467E119DD2CD8B0581CEC007B126CB6BAC34CB1E\""
  //    "    },"
  //    "    \"errorMsg\": \"\","
  //    "    \"status\": \"ok\""
  //    "}";

  DEBUGINFO << __func__ << replyData;

  QJsonDocument doc;
  parseJson (replyData.toUtf8(), baseErrorCode, __func__, &doc);

  if (jsonError())
    return;

  if (doc["data"].isObject())
    {
      QJsonObject data = doc["data"].toObject();
      // connection id
      if (data["id"].isString())
        {
          QString newConnectionId = data["id"].toString();
          
          // Only update if the ID has changed
          if (m_connectId != newConnectionId) {
            m_connectId = newConnectionId;
            saveConnectionId(m_connectId);
            qDebug() << "🔗 [WEB3 ID] New connection ID received and saved:" << m_connectId;
          } else {
            qDebug() << "🔗 [WEB3 ID] Received same connection ID, no update needed:" << m_connectId;
          }
          
          m_reconnectionAttempts = 0; // Reset counter on successful connection
          qDebug() << "🔄 [RECONNECTION DEBUG] Successfully connected to Dashboard, reconnection counter reset";
          emit connectionIdReceived (m_connectId);
          //DEBUGINFO << "[data][id]" << m_connectId;
        }
    }
}

void DapNodeWeb3::parseReplyWallets (const QString &replyData, int baseErrorCode)
{
  // wallets reply example
  //    "{"
  //    "    \"data\": ["
  //    "        \"CELLTestWallet\","
  //    "        \"walletCELL\""
  //    "    ],"
  //    "    \"errorMsg\": \"\","
  //    "    \"status\": \"ok\""
  //    "}";
#ifdef ENABLE_SENSITIVE_PRINTS
  DEBUGINFO << __func__ << replyData;
#endif // ENABLE_SENSITIVE_PRINTS

  QJsonDocument doc;
  parseJson (replyData.toUtf8(), baseErrorCode, __func__, &doc);

  if (jsonError())
    return;

#ifdef ENABLE_BAD_REPLY_WALLETS
  replyError (baseErrorCode + 40000,
              "Can't parse result. Socket connection err: 111\nCan't connect to cellframe-node\n",
              QString ("Request \"%1\" status is \"%2\"").arg ("parseReplyWallets", "bad"));
  m_parseJsonError = true;
  return;
#endif // ENABLE_BAD_REPLY_WALLETS

  if (doc["data"].isArray())
    {
      // wallets
      QStringList walletsList;
      const QJsonArray dataArray = doc["data"].toArray();

      for (const auto &item : dataArray)
      {
        /* if item is object */
        if (item.isObject())
        {
          auto itemObject = item.toObject();

          /* skip non active */
          if (itemObject.value ("status") == "non-Active")
            continue;

          /* store */
          walletsList << itemObject.value ("name").toString();
        }

        /* if item is string */
        else
        {
          /* store */
          walletsList << item.toString();
        }
      }

      emit sigReceivedWalletsList (walletsList);
    }
}


void DapNodeWeb3::parseReplyNetworks (const QString &replyData, int baseErrorCode)
{
  // networks reply example
  //    "{"
  //    "    \"data\": ["
  //    "        \"CELLTestWallet\","
  //    "        \"walletCELL\""
  //    "    ],"
  //    "    \"errorMsg\": \"\","
  //    "    \"status\": \"ok\""
  //    "}";

  DEBUGINFO << __func__ << replyData;

  QJsonDocument doc;
  parseJson (replyData.toUtf8(), baseErrorCode, __func__, &doc);

  if (jsonError())
    return;

  if (doc["data"].isArray())
    {
      // wallets
      QStringList networksList;
      QJsonArray dataArray = doc["data"].toArray();
      for (auto i = dataArray.cbegin(), e = dataArray.cend(); i != e; i++)
        if (i->isString())
          networksList << i->toString();
//        DEBUGINFO << "Networks list: " << networksList;
      emit sigReceivedNetworksList (networksList);
    }
}

void DapNodeWeb3::walletDataRequest (const QString &walletName)
{
  QString requesString = QString ("?method=GetDataWallet&id=%1&walletName=%2")
                         .arg (m_connectId)
                         .arg (walletName);
  sendRequest (requesString);
}

void DapNodeWeb3::parseDataWallet (const QString &replyData, int baseErrorCode)
{
  //{
  //"data": [
  //    {
  //        "address": "mWNv7A43YnqRHCWVJG2zobGRUWepgMs5b2K2Hq4w7QePxDXoy1VArS2vhdyAxp5cSR1Q2qUYQDkYQs4uFxr7TP3WnJzzTWa2iGFdDDv7",
  //        "network": "mileena",
  //        "tokens": [
  //            {
  //                "balance": "0",
  //                "datoshi": "0",
  //                "tokenName": "0"
  //            }
  //        ]
  //    },
  //    ...
  //    ...
  //],
  //"errorMsg": "",
  //"status": "ok"
  //}

  DEBUGINFO << __func__ << replyData;

  QJsonDocument doc;
  parseJson (replyData.toUtf8(), baseErrorCode, __func__, &doc);

  if (jsonError())
    return;

  if (doc["data"].isArray())
    {
      // wallet data
//        DEBUGINFO << "walletDataReply" << doc["data"].toArray();
      emit sigWalletDataReady (doc["data"].toArray());
    }
}

void DapNodeWeb3::parseCertificates (const QString &replyData, int baseErrorCode)
{
  //    {
  //        "data": [
  //            "node-addr-Backbone",
  //            "node-addr-kelvpn-minkowski",
  //            "node-addr-mileena",
  //            "node-addr-subzero"
  //            "cert01_public"
  //        ],
  //        "errorMsg": "",
  //        "status": "ok"
  //    }
#ifdef ENABLE_SENSITIVE_PRINTS
  DEBUGINFO << __func__ << replyData;
#endif // ENABLE_SENSITIVE_PRINTS

  QJsonDocument doc;
  parseJson (replyData.toUtf8(), baseErrorCode, __func__, &doc);

  if (jsonError())
    return;

  if (doc["data"].isArray())
    {
      // certificates
      QStringList certificates;
      QJsonArray dataArray = doc["data"].toArray();
      for (auto i = dataArray.cbegin(), e = dataArray.cend(); i != e; i++)
        if (i->isString())
          certificates << i->toString();
//        DEBUGINFO << "Certificates: " << certificates;
      emit sigReceivedCertificatestList (certificates);
    }
}

void DapNodeWeb3::parseCreateCertificate (const QString &replyData, int baseErrorCode)
{
  //    {
  //        "data": {
  //            "name": "rslCert02",
  //            "type": "sig_dil"
  //        },
  //        "errorMsg": "",
  //        "status": "ok"
  //    }
#ifdef ENABLE_SENSITIVE_PRINTS
  DEBUGINFO << __func__ << replyData;
#endif // ENABLE_SENSITIVE_PRINTS

  QJsonDocument doc;
  parseJson (replyData.toUtf8(), baseErrorCode, __func__, &doc);

  if (jsonError())
    return;

  if (doc["data"].isObject() && doc["data"].toObject()["name"].isString())
    {
      // get hash
      QString certificate = doc["data"].toObject()["name"].toString();
      emit sigCreatedCertificate (certificate);
    }
}

void DapNodeWeb3::parseCondTxCreateReply (const QString &replyData, int baseErrorCode)
{
  //    {
  //        "data": {
  //            "hash": "385A0652191C419AB7C09907D84EB1779DC0EF6EC55C952D02EA871325B2A80C"
  //        },
  //        "errorMsg": "",
  //        "status": "ok"
  //    }
  //
  //    OR when transaction is queued:
  //    {
  //        "data": {
  //            "idQueue": "0x688F93C77B18801981480A70D0ACDDB72689EC3A078DB2C2AB59AF7FFF1BFFBC",
  //            "success": true,
  //            "toQueue": true
  //        },
  //        "errorMsg": "",
  //        "status": "ok"
  //    }

  DEBUGINFO << __func__ << replyData;

  QJsonDocument doc;
  parseJson (replyData.toUtf8(), baseErrorCode, __func__, &doc);

  if (jsonError())
    return;

  if (doc["data"].isObject())
    {
      QJsonObject dataObj = doc["data"].toObject();
      
      // Check if transaction is queued
      if (dataObj["toQueue"].toBool() && dataObj.contains("idQueue")) {
        QString queueId = dataObj["idQueue"].toString();
        DEBUGINFO << "Transaction queued in Dashboard with ID:" << queueId;
        emit sigTransactionInQueue(queueId, m_connectedAppType);
        return;
      }
      
      // Check for transaction hash (immediate transaction)
      QString transactionHash;
      if (dataObj["tx_hash"].isString()) {
        transactionHash = dataObj["tx_hash"].toString();
        DEBUGINFO << "Found transaction hash in 'tx_hash' field:" << transactionHash;
      }
      else if (dataObj["hash"].isString()) {
        transactionHash = dataObj["hash"].toString();
        DEBUGINFO << "Found transaction hash in 'hash' field:" << transactionHash;
      }
      
      if (!transactionHash.isEmpty()) {
        emit sigCondTxCreateSuccess (transactionHash);
      } else {
        DEBUGINFO << "No valid transaction hash found in response data";
        replyError(baseErrorCode + 60000, "Transaction hash not found", "Response doesn't contain valid transaction hash");
      }
    }
}

void DapNodeWeb3::parseMempoolReply (const QString &replyData, int baseErrorCode)
{
  //    {
  //        "data": {
  //            "string": "transaction: hash: 0x385A0652191C419AB7C09907D84EB1779DC0EF6EC55C952D02EA871325B2A80C\n TS Created: Wed Nov 16 22:42:18 2022\n Token ticker: tKEL\n Items:\n IN:\nTx_prev_hash: 0x62C0CDEB24CFDE8ED40367CB180AE0C660DC4E47318698C91B26CD4942D9F30F\n Tx_out_prev_idx: 1\n OUT COND:\n Header:\n ts_expires: never\n value: 0.000000000000000001 (1)\n subtype: DAP_CHAIN_TX_OUT_COND_SUBTYPE_SRV_PAY\n unit: 0x00000003\n uid: 0x0000000000000001\n pkey: 0xD304A59B5C0A0A95149A2B34F77270E40ACBEBCCBC9536099928DBDB8DF3882C\n max price: 0.0 (0) \n OUT:\n Value: 989.899999999999999988 (989899999999999999988)\n Address: iDAeSXFFiwH2LyhquYZn7ce7wydGX8QUDRmCW1CKsMDDRRMvAA9CvRooswfp91fEz8hSqH6XxCysMWSxGUZGQSB9FXdUuMfgvWxgwKK7\nSignature: \nType: sig_dil\nPublic key hash: 0xAC75D1E675BB182FE1A5171BD50B6833C50BD29AC06A75491CCF7F411D53CB80\nPublic key size: 1196\nSignature size: 2096\n\n\r\n\n"
  //        },
  //        "errorMsg": "",
  //        "status": "ok"
  //    }

  DEBUGINFO << __func__ << replyData;

  QJsonDocument doc;
  parseJson (replyData.toUtf8(), baseErrorCode, __func__, &doc);

  if (jsonError())
    return;

  emit sigMempoolContainHash();
}

void DapNodeWeb3::parseLedgerReply (const QString &replyData, int baseErrorCode)
{
  //    {
  //        "data": {
  //            "string": "transaction: hash: 0x385A0652191C419AB7C09907D84EB1779DC0EF6EC55C952D02EA871325B2A80C\n TS Created: Wed Nov 16 22:42:18 2022\n Token ticker: tKEL\n Items:\n IN:\nTx_prev_hash: 0x62C0CDEB24CFDE8ED40367CB180AE0C660DC4E47318698C91B26CD4942D9F30F\n Tx_out_prev_idx: 1\n OUT COND:\n Header:\n ts_expires: never\n value: 0.000000000000000001 (1)\n subtype: DAP_CHAIN_TX_OUT_COND_SUBTYPE_SRV_PAY\n unit: 0x00000003\n uid: 0x0000000000000001\n pkey: 0xD304A59B5C0A0A95149A2B34F77270E40ACBEBCCBC9536099928DBDB8DF3882C\n max price: 0.0 (0) \n OUT:\n Value: 989.899999999999999988 (989899999999999999988)\n Address: iDAeSXFFiwH2LyhquYZn7ce7wydGX8QUDRmCW1CKsMDDRRMvAA9CvRooswfp91fEz8hSqH6XxCysMWSxGUZGQSB9FXdUuMfgvWxgwKK7\nSignature: \nType: sig_dil\nPublic key hash: 0xAC75D1E675BB182FE1A5171BD50B6833C50BD29AC06A75491CCF7F411D53CB80\nPublic key size: 1196\nSignature size: 2096\n\n\r\n\n"
  //        },
  //        "errorMsg": "",
  //        "status": "ok"
  //    }

  DEBUGINFO << __func__ << replyData;

  QJsonDocument doc;
  parseJson (replyData.toUtf8(), baseErrorCode, __func__, &doc);

  if (jsonError())
    return;

  emit sigLedgerContainHash();
}

void DapNodeWeb3::parseOrderList (const QString &replyData, int baseErrorCode)
{
  //{
  //"data": [
  //  {
  //      "direction": "SERV_DIR_SELL",
  //      "ext": "0x000000000000000000000000000000000000000000000000240000000000000000000000000000006D744B454C0000000000000000000000",
  //      "hash": " 0x2E525744B43AC4A0CB1A4E0BFBAE2A34C180F04A6CCEC7997C5BFCF28A574E8C",
  //      "node_addr": "7FFE::CBAC::8085::BBBA",
  //      "node_location": "None-None",
  //      "price": "0.00000000000000001(10)",
  //      "srv_uid": "0x0000000000000002",
  //      "tx_cond_hash": "0x1D05D1EB49B89AF54C8BD13281E823C161806AD2098ECC02A3B1CA4878990988",
  //      "version": "3"
  //  },
  //    ...
  //    ...
  //],
  //"errorMsg": "",
  //"status": "ok"
  //}


  qDebug().noquote().nospace() << __func__ << ": replyData = " << replyData;

  QJsonDocument doc;
  parseJson (replyData.toUtf8(), baseErrorCode, __func__, &doc);

  if (jsonError()) {
      QString fileName = DapLogger::getPathToLog() + "/error_replyData.json";
      qDebug() << fileName;
      QFile file(fileName);
      if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
          QTextStream out(&file);
          out << replyData;
          file.close();
          qDebug().noquote().nospace() << __func__ << ": JSON error occurred, replyData saved to " << fileName;
      } else {
          qDebug().noquote().nospace() << __func__ << ": Failed to save replyData to file.";
      }
      
      // Emit error to notify upper layers about the JSON parsing failure
      replyError(baseErrorCode + 70000, "JSON parsing error in order list", "Failed to parse order list response - invalid JSON format");
      return;
  }

  auto docArray  = doc["data"];
  QJsonArray orderArray = docArray.isArray() ? docArray.toArray() : QJsonArray();
  qDebug() << "🔍 [FILTER DEBUG] parseOrderList: Initial orders count =" << orderArray.size();
  emit sigOrderList (orderArray);
}

void DapNodeWeb3::parseNodeIp (const QString &replyData, int baseErrorCode)
{
  // reply example
  //    {
  //        "data": {
  //            "node IP": "138.68.111.134"
  //        },
  //        "errorMsg": "",
  //        "status": "ok"
  //    }

  qDebug() << "🔍 [IP DEBUG] parseNodeIp received:" << replyData;

  QJsonDocument doc;
  parseJson (replyData.toUtf8(), baseErrorCode, __func__, &doc);

  if (jsonError())
    return;

  QJsonObject data = doc.object();

  if (data.contains ("data"))
  {
    /* get data value */
    QJsonValue dataValue  = data.value ("data");

    /* if object, return as is with validation */
    if (dataValue.isObject())
    {
      data = data.value ("data").toObject();
      
      // Validate IP addresses in the response
      QJsonObject validatedData;
      int unknownIpCount = 0;
      int validIpCount = 0;
      
      for (auto it = data.begin(); it != data.end(); ++it) {
        QString key = it.key();
        QString ipValue = it.value().toString();
        
        // Special handling for Dashboard's "unknown ip" response
        if (ipValue == "unknown ip") {
          unknownIpCount++;
          validatedData.insert(key, ""); // Convert to empty string for UI
          qDebug() << "🚨 [IP DEBUG] Dashboard returned 'unknown ip' for node:" << key;
        } else if (isValidIPAddress(ipValue)) {
          validIpCount++;
          validatedData.insert(key, ipValue);
          qDebug() << "🔍 [IP DEBUG] Valid IP found:" << key << "->" << ipValue;
        } else if (ipValue.isEmpty()) {
          validatedData.insert(key, ""); // Keep empty for UI handling
          qDebug() << "🚨 [IP DEBUG] Empty IP for node:" << key;
        } else {
          qDebug() << "🚨 [IP DEBUG] Invalid IP format:" << ipValue << "for node:" << key;
          validatedData.insert(key, ""); // Replace invalid with empty
        }
      }
      
      qDebug() << "🔍 [IP DEBUG] IP Resolution Summary:" << validIpCount << "valid IPs," << unknownIpCount << "unknown from Dashboard," << (data.size() - validIpCount - unknownIpCount) << "other issues";
      
      // Trigger diagnostics if many IPs are unknown
      if (unknownIpCount > 0 && (unknownIpCount >= data.size() / 2 || unknownIpCount >= 3)) {
        qDebug() << "🚨 [IP DEBUG] High number of unknown IPs detected, triggering diagnostics";
        
        // Collect failed nodes for diagnostics
        QJsonArray failedNodes;
        for (auto it = data.begin(); it != data.end(); ++it) {
          if (it.value().toString().isEmpty()) { // These were "unknown ip" originally
            failedNodes.append(it.key());
          }
        }
        
        // Trigger diagnostics (delayed to avoid interfering with current response)
        QTimer::singleShot(1000, this, [this, failedNodes]() {
          diagnoseDashboardNodeAvailability(m_networkName, failedNodes);
        });
      }
      
      data = validatedData;
    }

    /* if array -> convert to object with validation */
    else if (dataValue.isArray())
    {
      /* clear */
      data = QJsonObject();

      /* get array */
      auto jarr = dataValue.toArray();
      
      int unknownIpCount = 0;
      int validIpCount = 0;

      /* convert in cycle with validation */
      for (const auto &jarrItem : qAsConst (jarr))
      {
        QJsonObject jitem = jarrItem.toObject();

        if (jitem.isEmpty())
          continue;

        auto it = jitem.begin();
        QString key = it.key();
        QString ipValue = it.value().toString();
        
        // Special handling for Dashboard's "unknown ip" response
        if (ipValue == "unknown ip") {
          unknownIpCount++;
          data.insert(key, ""); // Convert to empty string for UI
          qDebug() << "🚨 [IP DEBUG] Dashboard returned 'unknown ip' in array for node:" << key;
        } else if (isValidIPAddress(ipValue)) {
          validIpCount++;
          data.insert(key, ipValue);
          qDebug() << "🔍 [IP DEBUG] Valid IP from array:" << key << "->" << ipValue;
        } else if (ipValue.isEmpty()) {
          data.insert(key, ""); // Keep empty for UI handling
          qDebug() << "🚨 [IP DEBUG] Empty IP in array for node:" << key;
        } else {
          qDebug() << "🚨 [IP DEBUG] Invalid IP in array:" << ipValue << "for node:" << key;
          data.insert(key, ""); // Replace invalid with empty
        }
      }
      
      qDebug() << "🔍 [IP DEBUG] Array IP Resolution Summary:" << validIpCount << "valid IPs," << unknownIpCount << "unknown from Dashboard," << (jarr.size() - validIpCount - unknownIpCount) << "other issues";
    }

    qDebug() << "🔍 [IP DEBUG] Final parsed IP data:" << QJsonDocument(data).toJson(QJsonDocument::Compact);
    emit sigNodeIp (data);
  } else {
    qDebug() << "🚨 [IP DEBUG] No 'data' field found in response";
    // Emit empty object to prevent hanging
    emit sigNodeIp(QJsonObject());
  }
}

bool DapNodeWeb3::isValidIPAddress(const QString& ip) const
{
    if (ip.isEmpty()) {
        return false; // Empty is not valid, but handled separately
    }
    
    // IPv4 validation
    QRegularExpression ipv4Regex(R"(^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$)");
    if (!ipv4Regex.match(ip).hasMatch()) {
        return false;
    }
    
    // Check each octet is valid (0-255)
    QStringList octets = ip.split('.');
    for (const QString& octet : octets) {
        bool ok;
        int value = octet.toInt(&ok);
        if (!ok || value < 0 || value > 255) {
            return false;
        }
    }
    
    return true;
}

void DapNodeWeb3::requestNodeListForDiagnostics(const QString &networkName)
{
    qDebug() << "🔍 [IP DEBUG] Requesting NodeList for diagnostics on network:" << networkName;
    
    QString requestString = QString("?method=NodeList&id=%1&net=%2")
                           .arg(m_connectId)
                           .arg(networkName);
                           
    qDebug() << "🔍 [IP DEBUG] Diagnostic NodeList request:" << requestString;
    // Note: This will use the existing parseNodeList handler
    sendRequest(requestString);
}

void DapNodeWeb3::diagnoseDashboardNodeAvailability(const QString &networkName, const QJsonArray &failedNodes)
{
    qDebug() << "🚨 [IP DEBUG] Dashboard Diagnostics for network:" << networkName;
    qDebug() << "🚨 [IP DEBUG] Failed to resolve" << failedNodes.size() << "node IPs";
    
    for (int i = 0; i < failedNodes.size(); ++i) {
        QString nodeAddr = failedNodes[i].toString();
        qDebug() << "🚨 [IP DEBUG] Failed node" << (i+1) << ":" << nodeAddr;
    }
    
    qDebug() << "🔍 [IP DEBUG] Possible causes for 'unknown ip':";
    qDebug() << "🔍 [IP DEBUG] 1. Nodes are offline or not connected to network";
    qDebug() << "🔍 [IP DEBUG] 2. Dashboard is not fully synchronized with network";
    qDebug() << "🔍 [IP DEBUG] 3. Nodes haven't announced their IP addresses";
    qDebug() << "🔍 [IP DEBUG] 4. Network connectivity issues";
    qDebug() << "🔍 [IP DEBUG] 5. Nodes are behind NAT/firewall";
    
    // Request NodeList for comparison
    requestNodeListForDiagnostics(networkName);
}

void DapNodeWeb3::parseFee (const QString &replyData, int baseErrorCode)
{

  // reply example
  //    {
  //        "data": {
  //            "network": "riemann",
  //            "network fee": {
  //                "fee addr": "o9z3wUTSTicckJuoxkLc5q1CwaYs23474GbBm8ebgSZd1WmB7EhkPDpsoZPGX3hmhGa1wCqTDKgPjirbp3H45bg3tc6U5k8wCEJX575X",
  //                "fee coins": "0.0025",
  //                "fee datoshi": "2500000000000000",
  //                "fee ticker": "tKEL"
  //            },
  //            "validator fee": {
  //                "average fee coins": "0.056666666666666666",
  //                "average fee datoshi": "56666666666666666",
  //                "fee ticker": "tKEL ",
  //                "max fee coins": "0.07",
  //                "max fee datoshi": "70000000000000000",
  //                "min fee coins": "0.04",
  //                "min fee datoshi": "40000000000000000"
  //            }
  //        },
  //        "errorMsg": "",
  //        "status": "ok"
  //    }
#ifdef ENABLE_SENSITIVE_PRINTS
  DEBUGINFO << __func__ << replyData;
#endif // ENABLE_SENSITIVE_PRINTS

  /* parse */
  QJsonDocument doc;
  parseJson (replyData.toUtf8(), baseErrorCode, __func__, &doc);

  if (jsonError())
    return;

  /* get base */
  QJsonObject jdata = doc["data"].toObject();

  /* notify data */
  emit sigFeeData (jdata);

  /* read field or throw if not exists */
  auto field = [] (
     /*   IN    */ const QJsonObject & jobj,
     /*   IN    */ const QString & a_name,
     /* OUT+OPT */ QJsonObject * a_destJobj,
     /* OUT+OPT */ QString * a_destString)
  {
    /* if not exists */
    if (!jobj.contains (a_name))
      throw std::exception();

    /* get value */
    auto value  = jobj.value (a_name);

    /* store result */
    if (a_destJobj)
      *a_destJobj = value.toObject();
    else if (a_destString)
      *a_destString = value.toString();
  };

  try
    {
      QJsonObject jnet, jfee;
      QString feeCoins;

      /* get field sequence */
      field (/* from */ jdata, networkName(),  /* to */ &jnet, nullptr);
      field (/* from */ jnet, "network_fee",   /* to */ &jfee, nullptr);
      field (/* from */ jfee, "fee_coins",     /* to */ nullptr, &feeCoins);

      /* notify result value */
      emit sigFee (feeCoins);
    }
  catch (...)
    {
      DEBUGINFO << "parseFee : Error. Unable to get proper fee data";
    }

//  if (doc["data"].isObject() && doc["data"].toObject()[networkName()].toObject()["network_fee"].isObject())
//    {
//      emit sigFee (doc["data"].toObject()[networkName()].toObject()["network_fee"].toObject()["fee_coins"].toString());
//    }
}

void DapNodeWeb3::parseNodeList (const QString &replyData, int baseErrorCode)
{
//    {
//        "data": [
//            {
//                "cell": "0x0000000000000000",
//                "ipv4": "70.34.245.106",
//                "node address": "DDDD::0000::0000::0001",
//                "number of links": "0",
//                "port": "8079"
//            },
//            {
//                "cell": "0x0000000000000000",
//                "ipv4": "146.185.162.8",
//                "node address": "DDDD::0000::0000::0002",
//                "number of links": "0",
//                "port": "8079"
//            },
//            ...
//        ],
//    "errorMsg": "",
//    "status": "ok"
//    }

  qDebug() << "🔍 [IP DEBUG] parseNodeList received for diagnostics";
  DEBUGINFO << __func__ << replyData;

  QJsonDocument doc;
  parseJson (replyData.toUtf8(), baseErrorCode, __func__, &doc);

  if (jsonError())
    return;

  if (doc["data"].isArray())
    {
      QList<QMap<QString, QString>> nodeList;
      QStringList availableNodeAddresses;
      
      foreach (const auto &nodeJo, doc["data"].toArray())
        {
          QMap<QString, QString> itemDump;
          foreach (const QString &key, nodeJo.toObject().keys())
            itemDump[key] = nodeJo[key].toString();
          nodeList.append (itemDump);
          
          // Collect node addresses for diagnostic comparison
          QString nodeAddr = nodeJo.toObject().value("node address").toString();
          if (!nodeAddr.isEmpty()) {
            availableNodeAddresses.append(nodeAddr);
          }
        }
        
      qDebug() << "🔍 [IP DEBUG] Dashboard reports" << nodeList.size() << "nodes available in network";
      qDebug() << "🔍 [IP DEBUG] Available node addresses:" << availableNodeAddresses;
      
      emit sigNodeList (nodeList);
    }
}

void DapNodeWeb3::parseListKeys (const QString &replyData, int baseErrorCode)
{
//    {
//        "data": {
//            "inactive keys count": "0",
//            "keys": [
//                {
//                    "active": "true",
//                    "node addr": "DDDD::1234::0000::0000",
//                    "pKey hash": "0xD719E3DE39D92998B7CFE7092AA16B53B619B6B093DA9D36B676402CE7AF24DE",
//                    "stake value": "5.0",
//                    "tx hash": "0x0000000000000000000000000000000000000000000000000000000000000000"
//                },
//                {
//                    "active": "true",
//                    "node addr": "DDDD::1234::0000::0001",
//                    "pKey hash": "0x2376A46753161112DD67C81F3EC2DB93A9ED38C2342104FC77301AC65E31EE7C",
//                    "stake value": "4.0",
//                    "tx hash": "0x6F0612F0761950A3AD822365DFBAF955A2681B2BFEE3DEA5FC001401AAC906AF"
//                },
//                {
//                    "active": "true",
//                    "node addr": "DDDD::1234::0000::0002",
//                    "pKey hash": "0x7AF94D9A0B3A97D92AD6A66EE6C20C450931946C5DFF695750CB7FF25CAEB1E5",
//                    "stake value": "3.0",
//                    "tx hash": "0x6AD3B465D338640D2E5E70D039F287D4EEF3A45AA8D1718D72011D4E08D6213F"
//                }
//            ],
//            "minimum value for key delegating": "5.0 mtKEL",
//            "total keys count": "3"
//        },
//        "errorMsg": "",
//        "status": "ok"
//    }
#ifdef ENABLE_SENSITIVE_PRINTS
  DEBUGINFO << __func__ << replyData;
#endif // ENABLE_SENSITIVE_PRINTS

  QJsonDocument doc;
  parseJson (replyData.toUtf8(), baseErrorCode, __func__, &doc);

  if (jsonError())
    return;

  if (doc["data"].isObject() && doc["data"].toObject()["keys"].isArray())
    {
      QList<QString> listKeys;
      foreach (const auto &keyData, doc["data"].toObject()["keys"].toArray())
        {
//          if (keyData.toObject()["active"].toString() == QString ("true"))
            listKeys.append (keyData.toObject()["pKey hash"].toString());
        }
      DEBUGINFO << "sigListKeys" << listKeys;
      emit sigListKeys (listKeys);
    }
}

void DapNodeWeb3::parseNetId (const QString &replyData, int baseErrorCode)
{

  DEBUGINFO << __func__ << replyData;

  QJsonDocument doc;
  parseJson (replyData.toUtf8(), baseErrorCode, __func__, &doc);

  if (jsonError())
    return;

//  if (doc["data"].isObject())
//    {
//      emit sigNetId (doc["data"].toObject());
//    }
  QString netId;
  QJsonObject jdata = doc["data"].toObject();

  if (!jdata.contains ("id")){
    qDebug() << "Bad data";
    return;
  }

  /* get value */
//  QString value  = jdata.value ("net_id").toString();
  emit sigNetId (jdata.value ("id").toString());


//  field (/* from */ jdata, "net_id",     /* to */ nullptr, &netId);

}



void DapNodeWeb3::parseJson (const QString &replyData, int baseErrorCode, const QString &a_replyName, QJsonDocument *a_destDoc)
{
  // reply example
  //    "{ ... "
  //    "    \"errorMsg\": \"\","
  //    "    \"status\": \"ok\""
  //    "}";

  /* reset error flag */
  m_parseJsonError = false;

  /* parse JSON */
  QJsonDocument doc = QJsonDocument::fromJson (replyData.toUtf8());
  if (a_destDoc)
    *a_destDoc  = doc;

  /* return if unable to parse JSON */
  if (doc.isNull())
    {
      replyError (baseErrorCode + 30000,  "Unable to parse json data", "Invalid WEB3 response data");
#ifdef ENABLE_SENSITIVE_PRINTS
      DEBUGINFO << "Invalid json data:" << replyData;
#endif // ENABLE_SENSITIVE_PRINTS
      m_parseJsonError = true;
      return;
    }

//   DEBUGINFO << "parseJsonError status" << doc["status"] << doc["status"].toString();
  // TODO "errorMsg": "Incorrect id"

  /* get status */
  auto status = doc["status"].toString();

  /* return on ok */
  if (status == "ok")
    {
      DEBUGINFO << "status ok";
      return;
    }

  /* get error message */
  auto errorMsg = doc["errorMsg"];

  /* if details present */
  if (errorMsg.isString())
    {
      /* get details */
      auto errorMsgString = errorMsg.toString();

      /* notify on incorrect id */
      if (errorMsgString == "Incorrect id")
      {
        qDebug() << "🔗 [WEB3 ID] Received 'Incorrect id' error, clearing stored connection ID";
        clearStoredConnectionId();
        m_connectId.clear();
        emit sigIncorrectId();
      }

      /* print error */
      if (!errorMsgString.isEmpty())
        {
          replyError (baseErrorCode + 40000,
                      errorMsgString,
                      QString ("Request \"%1\" status is \"%2\"").arg (a_replyName, status));
          m_parseJsonError = true;
          return;
        }
    }

  /* if no error details present */
  replyError (baseErrorCode + 50000, "Error, details missing", "Error, details missing");
  m_parseJsonError = true;
  return;
}

void DapNodeWeb3::replyError (int errorCode, const QString &errorString, const QString &errorGuiMessage)
{
  DEBUGINFO << "Reply error"
            << errorCode
            << ((errorGuiMessage != errorString) ? errorGuiMessage : QString())
            << errorString;

  // For reconnect to Dashboard
  if(errorString == "Incorrect id")
  {
    m_reconnectionAttempts++;
    qDebug() << "🔄 [RECONNECTION DEBUG] Incorrect ID received, attempt" << m_reconnectionAttempts << "of" << MAX_RECONNECTION_ATTEMPTS;
    
    if (m_reconnectionAttempts <= MAX_RECONNECTION_ATTEMPTS) {
      qInfo() << "🔄 [RECONNECTION DEBUG] Attempting to reconnect to Dashboard...";
      // Clear stored ID when it becomes invalid
      clearStoredConnectionId();
      m_connectId.clear();
      nodeConnectionRequest();
      //TODO: Need reset no cdb GUI status
    } else {
      qCritical() << "🚨 [RECONNECTION DEBUG] CRITICAL: Maximum reconnection attempts exceeded, stopping reconnection attempts";
      m_reconnectionAttempts = 0; // Reset counter for next session
      emit sigError (errorCode + 80000, "Maximum reconnection attempts exceeded - unable to establish connection to Dashboard");
    }
  }
  else
    emit sigError (errorCode, errorGuiMessage);
}

/****************************************//**
 * @name WEB3 CONNECTION ID PERSISTENCE
 *******************************************/
/// @{

void DapNodeWeb3::saveConnectionId(const QString& connectionId)
{
  if (connectionId.isEmpty()) {
    qWarning() << "🔗 [WEB3 ID] Attempted to save empty connection ID";
    return;
  }
  
  DapServiceDataLocal::saveSetting("web3_connection_id", connectionId);
  qDebug() << "🔗 [WEB3 ID] Saved connection ID to storage:" << connectionId;
}

QString DapNodeWeb3::loadStoredConnectionId()
{
  QString storedId = DapServiceDataLocal::getSetting("web3_connection_id").toString();
  if (!storedId.isEmpty()) {
    qDebug() << "🔗 [WEB3 ID] Retrieved stored connection ID:" << storedId;
  }
  return storedId;
}

void DapNodeWeb3::clearStoredConnectionId()
{
  DapServiceDataLocal::removeSetting("web3_connection_id");
  qDebug() << "🔗 [WEB3 ID] Cleared stored connection ID";
}

bool DapNodeWeb3::hasStoredConnectionId()
{
  return !DapServiceDataLocal::getSetting("web3_connection_id").toString().isEmpty();
}

void DapNodeWeb3::forceReconnect()
{
  qDebug() << "🔗 [WEB3 ID] Force reconnect requested - clearing stored ID and reconnecting";
  clearStoredConnectionId();
  m_connectId.clear();
  m_reconnectionAttempts = 0;
  nodeConnectionRequest();
}

/// @}

