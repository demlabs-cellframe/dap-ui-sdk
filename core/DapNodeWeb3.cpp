/* INCLUDES */
#include "DapNodeWeb3.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include "DapDataLocal.h"
#include <QTimer>
#include <QDebug>

/* DEFS */
#define SIGERROR (110)
#define DEBUGINFO qDebug()<<"--->Web3<---"

typedef DapNodeWeb3::ReplyMethodID ReplyMethodID;

struct ReplyMethod
{
  //QString request;
  //void(DapNodeWeb3::*parseMethod)(const QString&, int baseErrorCode, const QString&);
  //void(DapNodeWeb3::*replyError)(int error);
  ReplyMethodID parseMethodID;
  bool replyError;
  QString wrongReplyText;
  int baseErrorCode;
};

/* VARS */
const QString  DapNodeWeb3::WEB3_URL      ("localhost");
const quint16  DapNodeWeb3::WEB3_PORT     (8045);

//const QList<ReplyMethod> replyItems = {
//    // connect reply
//    {"method=Connect",              &DapNodeWeb3::parseReplyConnect,        &DapNodeWeb3::replyConnectError, "Wrong reply connect", 100000},
//    // node status reply
//    {"method=GetNodeStatus",        &DapNodeWeb3::parseReplyStatus,         &DapNodeWeb3::replyConnectError, "Wrong reply status", 200000},
//    // wallets list reply
//    {"method=GetWallets",           &DapNodeWeb3::parseReplyWallets,        nullptr, "Wrong reply when get wallets list", 300000},
//    // networks list reply
//    {"method=GetNetworks",          &DapNodeWeb3::parseReplyNetworks,       nullptr, "Wrong reply when get networks list", 400000},
//    // data wallets reply
//    {"method=GetDataWallet",        &DapNodeWeb3::parseDataWallet,          nullptr, "Wrong reply when get data wallets", 500000},
//    // get certificates
//    {"method=GetCertificates",      &DapNodeWeb3::parseCertificates,        nullptr, "Wrong reply when get certificates", 600000},
//    // create certificate
//    {"method=CreateCertificate",    &DapNodeWeb3::parseCreateCertificate,   nullptr, "Wrong reply when create certificates", 700000},
//    // create cond transaction reply
//    {"method=CondTxCreate",         &DapNodeWeb3::parseCondTxCreateReply,   nullptr, "Wrong reply when create transaction", 800000},
//    // check mempool
//    {"method=GetMempoolTxHash",     &DapNodeWeb3::parseMempoolReply,        nullptr, "Wrong reply when get mempool transaction hash", 900000},
//    // check ledger
//    {"method=GetLedgerTxHash",      &DapNodeWeb3::parseLedgerReply,         nullptr, "Wrong reply when get ledger transaction hash", 1000000},
//    // get orders
//    {"method=GetOrdersList",        &DapNodeWeb3::parseOrderList,           nullptr, "Wrong reply when get orders list", 1100000},
//    // get ip
//    {"method=GetNodeIP",            &DapNodeWeb3::parseNodeIp,              nullptr, "Wrong reply when get node ip", 1200000},
//    // get fee
//    {"method=GetFee",               &DapNodeWeb3::parseFee,                 nullptr, "Wrong reply when get fee", 1300000},
//    // node dump
//    {"method=NodeDump",             &DapNodeWeb3::parseNodeDump,            nullptr, "Wrong reply when get node dump", 1400000},
//    // list of delegated keys on the network
//    {"method=GetListKeys",          &DapNodeWeb3::parseListKeys,            nullptr, "Wrong reply when get list of delegated keys", 1500000},
//};

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
  { "NodeDump",           {ReplyMethodID::ParseNodeDump,          false, "Wrong reply when get node dump", 1400000}},
  // list of delegated keys on the network
  { "GetListKeys",        {ReplyMethodID::ParseListKeys,          false, "Wrong reply when get list of delegated keys", 1500000}},
};

static const ReplyMethod s_dummyReply
{
  ReplyMethodID::Invalid,
  false,
  QString(),
  0
};

DapNodeWeb3::DapNodeWeb3(QObject * obj, int requestTimeout) :
    QObject(obj),
    m_requestTimeout(requestTimeout),
    m_connectId(QString()),
    m_networkReply(nullptr),
    m_networkRequest(QString())
{
    m_httpClient = new DapNetworkAccessManager();
}


DapNodeWeb3::~ DapNodeWeb3()
{
}

void DapNodeWeb3::request_GET(const QString& host,  quint16 port, const QString & urlPath,
                           DapNetworkReply &a_netReply, const QString& headers, bool ssl)
{
    Q_UNUSED(ssl)
//    qDebug() << urlPath;
    m_httpClient->requestHttp_GET(host, port, urlPath, headers, a_netReply);
}

void DapNodeWeb3::sendRequest(QString request)
{
    m_networkReply =  new DapNetworkReply;
    connect( m_networkReply, &DapNetworkReply::finished, this, [&] {
        responseProcessing(m_networkReply->error(), m_networkReply->errorString());
    });
    connect( m_networkReply, &DapNetworkReply::sigError, this, [&] {
        responseProcessing(m_networkReply->error(), m_networkReply->errorString(), false);
    });
    // send request
    m_networkRequest = request;
    request_GET(WEB3_URL, WEB3_PORT, request, *m_networkReply);
}

QString extractMethod (const QString &inputString)
{
  QRegularExpression regex (R"(\?method=([^&]+))");
  QRegularExpressionMatch match = regex.match(inputString);

  if (match.hasMatch())
    return match.captured(1);
  else
    return QString(); // Если совпадение не найдено
}

void DapNodeWeb3::responseProcessing(
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
  if (methodName == "GetNodeStatus") // if (networkRequest.contains ("method=GetNodeStatus"))
  {
    if (error == SIGERROR)
    {
      // TODO check (error == SIGERROR) on other platforms (windows, android, ios)
      emit nodeNotDetected();
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

  /* parse */
  responseParsing(
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
    DEBUGINFO << "http://127.0.0.1:8045/?method=GetNodeStatus";
    // A non-existent method is used to check the operation of the dashboard,
    // in the case of the operation of the dashboard service, a response is returned:
    //    {
    //        "data": "",
    //        "errorMsg": "Incorrect id",
    //        "status": "bad"
    //    }
    nodeStatusRequest();
}

void DapNodeWeb3::nodeConnectionRequest()
{
    // example connect request
    // http://127.0.0.1:8045/?method=Connect
    DEBUGINFO << "http://127.0.0.1:8045/?method=Connect";
    sendRequest("?method=Connect");
}

void DapNodeWeb3::replyConnectError(int code)
{
    Q_UNUSED(code)
    emit nodeNotConnected();
}

void DapNodeWeb3::responseParsing(
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
  if (error == QNetworkReply::NetworkError::NoError)
  {
    // no error reply
    auto reply = m_networkReply->getReplyData();
//        qInfo() << "reply message: " << reply;

    // check reply message error
    if (reply.size() == 0)
      return replyError (baseErrorCode + 20000, messageReplyDataError);

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
        case ReplyMethodID::ParseNodeDump:          parseNodeDump (reply,           baseErrorCode); break;
        case ReplyMethodID::ParseListKeys:          parseListKeys (reply,           baseErrorCode); break;
      }

//    if (parseMethod)
//      (this->*parseMethod)(reply, baseErrorCode, save_data);
  }
  else
  {
    if (responceError)
      replyConnectError (error);

    if (httpFinished)
      replyError (baseErrorCode + 10000 + error , wrongReplyerrorString);
    else
      replyError (baseErrorCode + error, messageReplyDataError);
  }
}

void DapNodeWeb3::nodeStatusRequest()
{
    QString requesString = QString("?method=GetNodeStatus&id=%1").arg(m_connectId);
    sendRequest(requesString);
}

void DapNodeWeb3::walletsRequest()
{
    //http://127.0.0.1:8045/?method=GetWallets&id=0x7D6878E27691240294BE6DE326F56FD1344087024F0FBBEA98B4A04321D21F73
    QString requesString = QString("?method=GetWallets&id=%1").arg(m_connectId);
    sendRequest(requesString);
}

void DapNodeWeb3::networksRequest()
{
    QString requesString = QString("?method=GetNetworks&id=%1").arg(m_connectId);
    sendRequest(requesString);
}

void DapNodeWeb3::condTxCreateRequest(QString walletName, QString networkName, QString sertificateName, QString tokenName, QString value, QString unit, QString fee)
{
    QString requesString = QString("?method=CondTxCreate&"
                "id=%1&"
                "net=%2&"
                "tokenName=%3&"
                "walletName=%4&"
                "certName=%5&"
                "value=%6&"
                "unit=%7&"
                "fee=%8&"
                "srv_uid=1")
            .arg(m_connectId)
            .arg(networkName)
            .arg(tokenName)
            .arg(walletName)
            .arg(sertificateName)
            .arg(value)
            .arg(unit)
            .arg(fee);
    sendRequest(requesString);
}

void DapNodeWeb3::createCertificate(const QString& certType, const QString& certName)
{
    QString requesString = QString("?method=CreateCertificate&"
                "id=%1&"
                "certType=%2&"
                "certName=%3&"
                "categoryCert=%4")
            .arg(m_connectId)
            .arg(certType)
            .arg(certName)
            .arg("public");
    sendRequest(requesString);
}

void DapNodeWeb3::getCertificates()
{
    QString requesString = QString("?method=GetCertificates&"
                "id=%1&"
                "categoryCert=%2")
            .arg(m_connectId)
            .arg("public");
    sendRequest(requesString);
}

void DapNodeWeb3::getMempoolTxHashRequest(QString transactionHash, QString networkName)
{
    QString requesString = QString("?method=GetMempoolTxHash&"
                "id=%1&net=%2&hashTx=%3")
            .arg(m_connectId)
            .arg(networkName)
            .arg(transactionHash);
    sendRequest(requesString);
}

void DapNodeWeb3::getLedgerTxHashRequest(QString transactionHash, QString networkName)
{
    QString requesString = QString("?method=GetLedgerTxHash&"
                "id=%1&hashTx=%2&net=%3")
            .arg(m_connectId)
            .arg(transactionHash)
            .arg(networkName);
    sendRequest(requesString);
}

void DapNodeWeb3::getOrdersListRequest(QString networkName, QString tokenName, QString minPrice, QString maxPrice, QString unit)
{
    QString requesString = QString("?method=GetOrdersList&"
            "id=%1&direction=%2&srv_uid=1")
            .arg(m_connectId)
            .arg("sell");
    if (!networkName.isEmpty())
        requesString += QString("&net=%1").arg(networkName);
    if (!tokenName.isEmpty())
        requesString += QString("&tokenName=%1").arg(tokenName);
    if (!minPrice.isEmpty())
        requesString += QString("&price_min=%1").arg(minPrice);
    if (!maxPrice.isEmpty())
        requesString += QString("&price_max=%1").arg(maxPrice);
    if (!unit.isEmpty())
        requesString += QString("&unit=%1").arg(unit);
    qDebug() << "OrdersListRequest" << requesString;
    sendRequest(requesString);
}

void DapNodeWeb3::getNodeIPRequest (const QString &networkName, const QJsonArray &orderList)
{
  auto addressArgument = [](const QJsonArray &list) -> QString
  {
    return list.size() > 1
        ? "jsonArray=" + QJsonDocument(list).toJson (QJsonDocument::Compact)
        : "addr=" + list.begin()->toString();
  };

  QString requesString =
    QString ("?method=GetNodeIP"
      "&id=%1"
      "&net=%2&%3")
    .arg(m_connectId, networkName, addressArgument (orderList));

  sendRequest (requesString);
}

void DapNodeWeb3::getFeeRequest(QString networkName)
{
    QString requesString = QString("?method=GetFee&"
                "id=%1&net=%2")
            .arg(m_connectId)
            .arg(networkName);
    sendRequest(requesString);
}

void DapNodeWeb3::nodeDumpRequest(QString networkName)
{
    QString requesString = QString("?method=NodeDump&"
                "id=%1&net=%2")
            .arg(m_connectId)
            .arg(networkName);
    sendRequest(requesString);
}

void DapNodeWeb3::getListKeysRequest(QString networkName)
{
    QString requesString = QString("?method=GetListKeys&"
                "id=%1&net=%2")
            .arg(m_connectId)
            .arg(networkName);
    sendRequest(requesString);
}


void DapNodeWeb3::parseReplyStatus(const QString& replyData, int baseErrorCode)
{
    // status reply example
    //    {
    //        "data": "",
    //        "errorMsg": "Incorrect id",
    //        "status": "bad"
    //    }
    if (jsonError())
        return;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    DEBUGINFO << "parseReplyNodeStatus" << replyData << doc;
    if (doc["status"].isString())
    {
        if (doc["status"].toString() == QString("bad")) {
            // node detected and not have correct connect_id
            DEBUGINFO << "nodeDetected, but connet_id is incorrect";
            emit nodeDetected();
            return;
        }
        if (doc["status"].toString() == QString("ok")) {
            DEBUGINFO << "nodeStatusOk";
            emit statusOk();
            emit nodeDetected();
            return;
        }
    }
}

void DapNodeWeb3::parseReplyConnect(const QString& replyData, int baseErrorCode)
{
    // connect reply example
    //    "{"
    //    "    \"data\": {"
    //    "        \"id\": \"0xCD33094D27F8ACEBBCC90B31467E119DD2CD8B0581CEC007B126CB6BAC34CB1E\""
    //    "    },"
    //    "    \"errorMsg\": \"\","
    //    "    \"status\": \"ok\""
    //    "}";
    parseJsonError(replyData.toUtf8(), baseErrorCode);
    if (jsonError())
        return;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    DEBUGINFO << "parseReplyConnect" << replyData << doc;
    if (doc["data"].isObject())
    {
        QJsonObject data = doc["data"].toObject();
        // connection id
        if (data["id"].isString())
        {
            m_connectId = data["id"].toString();
            emit connectionIdReceived(m_connectId);
            //DEBUGINFO << "[data][id]" << m_connectId;
        }
    }
}

void DapNodeWeb3::parseReplyWallets(const QString& replyData, int baseErrorCode)
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
    parseJsonError(replyData.toUtf8(), baseErrorCode);
    if (jsonError())
        return;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    if (doc["data"].isArray())
    {
        // wallets
        QStringList walletsList;
        QJsonArray dataArray = doc["data"].toArray();
        for (auto i = dataArray.cbegin(), e = dataArray.cend(); i != e; i++)
            if (i->isString())
                walletsList << i->toString();
        //DEBUGINFO << "Wallets list: " << walletsList;
        emit sigReceivedWalletsList(walletsList);
    }
}


void DapNodeWeb3::parseReplyNetworks(const QString& replyData, int baseErrorCode)
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
    parseJsonError(replyData.toUtf8(), baseErrorCode);
    if (jsonError())
        return;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    if (doc["data"].isArray())
    {
        // wallets
        QStringList networksList;
        QJsonArray dataArray = doc["data"].toArray();
        for (auto i = dataArray.cbegin(), e = dataArray.cend(); i != e; i++)
            if (i->isString())
                networksList << i->toString();
//        DEBUGINFO << "Networks list: " << networksList;
        emit sigReceivedNetworksList(networksList);
    }
}

void DapNodeWeb3::walletDataRequest(const QString& walletName)
{
    QString requesString = QString("?method=GetDataWallet&id=%1&walletName=%2")
            .arg(m_connectId)
            .arg(walletName);
    sendRequest(requesString);
}

void DapNodeWeb3::parseDataWallet(const QString& replyData, int baseErrorCode)
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
//    DEBUGINFO << "walletDataReply" << replyData;
    parseJsonError(replyData.toUtf8(), baseErrorCode);
    if (jsonError())
        return;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    if (doc["data"].isArray())
    {
        // wallet data
//        DEBUGINFO << "walletDataReply" << doc["data"].toArray();
        emit sigWalletDataReady(doc["data"].toArray());
    }
}

void DapNodeWeb3::parseCertificates(const QString& replyData, int baseErrorCode)
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
    parseJsonError(replyData.toUtf8(), baseErrorCode);
    if (jsonError())
        return;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    if (doc["data"].isArray())
    {
        // certificates
       QStringList certificates;
        QJsonArray dataArray = doc["data"].toArray();
        for (auto i = dataArray.cbegin(), e = dataArray.cend(); i != e; i++)
            if (i->isString())
                certificates << i->toString();
//        DEBUGINFO << "Certificates: " << certificates;
        emit sigReceivedCertificatestList(certificates);
    }
}

void DapNodeWeb3::parseCreateCertificate(const QString& replyData, int baseErrorCode)
{
    //    {
    //        "data": {
    //            "name": "rslCert02",
    //            "type": "sig_dil"
    //        },
    //        "errorMsg": "",
    //        "status": "ok"
    //    }
    parseJsonError(replyData.toUtf8(), baseErrorCode);
    if (jsonError())
        return;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    if (doc["data"].isObject() && doc["data"].toObject()["name"].isString())
    {
        // get hash
        QString certificate = doc["data"].toObject()["name"].toString();
        emit sigCreatedCertificate(certificate);
    }
}

void DapNodeWeb3::parseCondTxCreateReply(const QString& replyData, int baseErrorCode)
{
    //    {
    //        "data": {
    //            "hash": "385A0652191C419AB7C09907D84EB1779DC0EF6EC55C952D02EA871325B2A80C"
    //        },
    //        "errorMsg": "",
    //        "status": "ok"
    //    }
    DEBUGINFO << "parseCondTxCreateReply" << replyData;
    parseJsonError(replyData.toUtf8(), baseErrorCode);
    if (jsonError())
        return;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    if (doc["data"].isObject() && doc["data"].toObject()["hash"].isString())
    {
        // get hash
        QString transactionHash = "0x" + doc["data"].toObject()["hash"].toString();
        emit sigCondTxCreateSuccess(transactionHash);
    }
}

void DapNodeWeb3::parseMempoolReply(const QString& replyData, int baseErrorCode)
{
    //    {
    //        "data": {
    //            "string": "transaction: hash: 0x385A0652191C419AB7C09907D84EB1779DC0EF6EC55C952D02EA871325B2A80C\n TS Created: Wed Nov 16 22:42:18 2022\n Token ticker: tKEL\n Items:\n IN:\nTx_prev_hash: 0x62C0CDEB24CFDE8ED40367CB180AE0C660DC4E47318698C91B26CD4942D9F30F\n Tx_out_prev_idx: 1\n OUT COND:\n Header:\n ts_expires: never\n value: 0.000000000000000001 (1)\n subtype: DAP_CHAIN_TX_OUT_COND_SUBTYPE_SRV_PAY\n unit: 0x00000003\n uid: 0x0000000000000001\n pkey: 0xD304A59B5C0A0A95149A2B34F77270E40ACBEBCCBC9536099928DBDB8DF3882C\n max price: 0.0 (0) \n OUT:\n Value: 989.899999999999999988 (989899999999999999988)\n Address: iDAeSXFFiwH2LyhquYZn7ce7wydGX8QUDRmCW1CKsMDDRRMvAA9CvRooswfp91fEz8hSqH6XxCysMWSxGUZGQSB9FXdUuMfgvWxgwKK7\nSignature: \nType: sig_dil\nPublic key hash: 0xAC75D1E675BB182FE1A5171BD50B6833C50BD29AC06A75491CCF7F411D53CB80\nPublic key size: 1196\nSignature size: 2096\n\n\r\n\n"
    //        },
    //        "errorMsg": "",
    //        "status": "ok"
    //    }
//    DEBUGINFO << "parseMempoolReply" << replyData;
    parseJsonError(replyData.toUtf8(), baseErrorCode);
    if (jsonError())
        return;
    emit sigMempoolContainHash();
}

void DapNodeWeb3::parseLedgerReply(const QString& replyData, int baseErrorCode)
{
    //    {
    //        "data": {
    //            "string": "transaction: hash: 0x385A0652191C419AB7C09907D84EB1779DC0EF6EC55C952D02EA871325B2A80C\n TS Created: Wed Nov 16 22:42:18 2022\n Token ticker: tKEL\n Items:\n IN:\nTx_prev_hash: 0x62C0CDEB24CFDE8ED40367CB180AE0C660DC4E47318698C91B26CD4942D9F30F\n Tx_out_prev_idx: 1\n OUT COND:\n Header:\n ts_expires: never\n value: 0.000000000000000001 (1)\n subtype: DAP_CHAIN_TX_OUT_COND_SUBTYPE_SRV_PAY\n unit: 0x00000003\n uid: 0x0000000000000001\n pkey: 0xD304A59B5C0A0A95149A2B34F77270E40ACBEBCCBC9536099928DBDB8DF3882C\n max price: 0.0 (0) \n OUT:\n Value: 989.899999999999999988 (989899999999999999988)\n Address: iDAeSXFFiwH2LyhquYZn7ce7wydGX8QUDRmCW1CKsMDDRRMvAA9CvRooswfp91fEz8hSqH6XxCysMWSxGUZGQSB9FXdUuMfgvWxgwKK7\nSignature: \nType: sig_dil\nPublic key hash: 0xAC75D1E675BB182FE1A5171BD50B6833C50BD29AC06A75491CCF7F411D53CB80\nPublic key size: 1196\nSignature size: 2096\n\n\r\n\n"
    //        },
    //        "errorMsg": "",
    //        "status": "ok"
    //    }
//    DEBUGINFO << "parseLedgerReply" << replyData;
    parseJsonError(replyData.toUtf8(), baseErrorCode);
    if (jsonError())
        return;
    emit sigLedgerContainHash();
}

void DapNodeWeb3::parseOrderList(const QString& replyData, int baseErrorCode)
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
    DEBUGINFO << "orderListReply" << replyData;
    parseJsonError(replyData.toUtf8(), baseErrorCode);
    if (jsonError())
        return;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    if (doc["data"].isArray())
    {
        // wallet data
//        DEBUGINFO << "orderListReply" << doc["data"].toArray();
        emit sigOrderList(doc["data"].toArray());
    }
}

void DapNodeWeb3::parseNodeIp(const QString& replyData, int baseErrorCode)
{
    // reply example
    //    {
    //        "data": {
    //            "node IP": "138.68.111.134"
    //        },
    //        "errorMsg": "",
    //        "status": "ok"
    //    }
    parseJsonError(replyData.toUtf8(), baseErrorCode);
    if (jsonError())
        return;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    if (doc["data"].isArray())
    {
      emit sigNodeIp (doc["data"].toArray());
    }
}

void DapNodeWeb3::parseFee(const QString& replyData, int baseErrorCode)
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
    parseJsonError(replyData.toUtf8(), baseErrorCode);
    if (jsonError())
        return;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    if (doc["data"].isObject() && doc["data"].toObject()["network fee"].isObject())
    {
        emit sigFee(doc["data"].toObject()["network fee"].toObject()["fee coins"].toString());
    }
}

void DapNodeWeb3::parseNodeDump(const QString& replyData, int baseErrorCode)
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
    parseJsonError(replyData.toUtf8(), baseErrorCode);
    if (jsonError())
        return;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    if (doc["data"].isArray())
    {
        QList<QMap<QString, QString>> nodeDump;
        foreach (const auto& nodeJo, doc["data"].toArray())
        {
            QMap<QString, QString> itemDump;
            foreach (const QString& key, nodeJo.toObject().keys())
                itemDump[key] = nodeJo[key].toString();
            nodeDump.append(itemDump);
        }
        emit sigNodeDump(nodeDump);
    }
}

void DapNodeWeb3::parseListKeys(const QString& replyData, int baseErrorCode)
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
    DEBUGINFO << "parseListKeys" << replyData;
    parseJsonError(replyData.toUtf8(), baseErrorCode);
    if (jsonError())
        return;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    if (doc["data"].isObject() && doc["data"].toObject()["keys"].isArray())
    {
        QList<QString> listKeys;
        foreach (const auto& keyData, doc["data"].toObject()["keys"].toArray())
        {
            if (keyData.toObject()["active"].toString() == QString("true"))
                listKeys.append(keyData.toObject()["pKey hash"].toString());
        }
        DEBUGINFO << "sigListKeys" << listKeys;
        emit sigListKeys(listKeys);
    }
}


void DapNodeWeb3::parseJsonError(const QString& replyData, int baseErrorCode)
{
    // reply example
    //    "{ ... "
    //    "    \"errorMsg\": \"\","
    //    "    \"status\": \"ok\""
    //    "}";
    m_parseJsonError = false;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    if(doc.isNull())
    {
        replyError(baseErrorCode + 30000,  "Unable to parse json data");
        m_parseJsonError = true;
        return;
    }
    else
    {
//        DEBUGINFO << "parseJsonError status" << doc["status"] << doc["status"].toString();
        // TODO "errorMsg": "Incorrect id"
        if (doc["status"].isString() && doc["status"].toString() != "ok")
        {
            if (doc["errorMsg"].isString() == QString("Incorrect id"))
            {
                emit sigIncorrectId();
            }

            if (doc["errorMsg"].isString())
            {
                replyError(baseErrorCode + 40000, doc["errorMsg"].toString());
                m_parseJsonError = true;
                return;
            }
            replyError(baseErrorCode + 50000, "Error, details missing");
            m_parseJsonError = true;
            return;
        }
        else
        {
            // "status": "ok"
//            DEBUGINFO << "status ok";
            return;
        }
    }
}

void DapNodeWeb3::replyError(int errorCode, const QString errorString)
{
    DEBUGINFO  << "Reply error" << errorCode << errorString;
    emit sigError(errorCode, errorString);
}

