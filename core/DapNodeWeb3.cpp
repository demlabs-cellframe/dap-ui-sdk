#include "DapNodeWeb3.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include "DapDataLocal.h"
#include <QTimer>
#include <QDebug>


#define SIGERROR (110)
#define DEBUGINFO qDebug()<<"--->Web3<---"

const QString  DapNodeWeb3::WEB3_URL      ("localhost");
const quint16  DapNodeWeb3::WEB3_PORT     (8045);

const QList<ReplyMethod> DapNodeWeb3::replyItems = {
    // connect reply
    {"method=Connect",              &DapNodeWeb3::parseReplyConnect,        &DapNodeWeb3::replyConnectError, "Wrong reply connect", 100000},
    // node status reply
    {"method=GetNodeStatus",        &DapNodeWeb3::nodeStatusOkReply,        nullptr, "Wrong reply status", 200000},
    // wallets list reply
    {"method=GetWallets",           &DapNodeWeb3::parseReplyWallets,        nullptr, "Wrong reply when get wallets list", 300000},
    // networks list reply
    {"method=GetNetworks",          &DapNodeWeb3::parseReplyNetworks,       nullptr, "Wrong reply when get networks list", 400000},
    // data wallets reply
    {"method=GetDataWallet",        &DapNodeWeb3::parseDataWallet,          nullptr, "Wrong reply when get data wallets", 500000},
    // get certificates
    {"method=GetCertificates",      &DapNodeWeb3::parseCertificates,        nullptr, "Wrong reply when get certificates", 600000},
    // create certificate
    {"method=CreateCertificate",    &DapNodeWeb3::parseCreateCertificate,   nullptr, "Wrong reply when create certificates", 700000},
    // create cond transaction reply
    {"method=CondTxCreate",         &DapNodeWeb3::parseCondTxCreateReply,   nullptr, "Wrong reply when create transaction", 800000},
    // check mempool
    {"method=GetMempoolTxHash",     &DapNodeWeb3::parseMempoolReply,        nullptr, "Wrong reply when get mempool transaction hash", 900000},
    // check ledger
    {"method=GetLedgerTxHash",      &DapNodeWeb3::parseLedgerReply,         nullptr, "Wrong reply when get ledger transaction hash", 1000000},
    // get orders
    {"method=GetOrdersList",        &DapNodeWeb3::parseOrderList,           nullptr, "Wrong reply when get orders list", 1100000},
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
    connect( m_networkReply, &DapNetworkReply::finished, this, [=] {
        responseProcessing(m_networkReply->error(), m_networkReply->errorString());
    });
    connect( m_networkReply, &DapNetworkReply::sigError, this, [=] {
        responseProcessing(m_networkReply->error(), m_networkReply->errorString(), false);
    });
    // send request
    m_networkRequest = request;
    request_GET(WEB3_URL, WEB3_PORT, request, *m_networkReply);
}

void DapNodeWeb3::responseProcessing(const int error, const QString errorString, const bool httpFinished)
{
    // local network request
    QString networkRequest(m_networkRequest);
    // reset m_networkRequest
    m_networkRequest = QString();
    // debug info
    // node connection reply
    if (networkRequest == "")
    {
        if (error == SIGERROR)
            // TODO check (error == SIGERROR) on other platforms (windows, android, ios)
            emit nodeNotDetected();
        if (error == QNetworkReply::NetworkError::NoError)
        {
            if (m_connectId.isEmpty())
                // request connect id
                emit nodeDetected();
            else
                // send request for check m_connectId
                emit checkNodeStatus();
        }
        return;
    }

    for (int k = 0; k < replyItems.count(); k++)
    {
        auto* r = &replyItems[k];
        if (networkRequest.contains(r->request))
        {
            responseParsing(error, errorString, httpFinished, r->baseErrorCode,
                            r->wrongReplyText, r->parseMethod, r->replyError);
            return;
        }
    }
}

void DapNodeWeb3::nodeDetectedRequest()
{
    //DEBUGINFO << "sendRequest(\"\")";
    sendRequest("");
}

void DapNodeWeb3::nodeConnectionRequest()
{
    // example connect request
    // http://127.0.0.1:8045/?method=Connect
    sendRequest("?method=Connect");
}

void DapNodeWeb3::replyConnectError(int code)
{
    Q_UNUSED(code)
    emit nodeNotConnected();
}

void DapNodeWeb3::responseParsing(const int error,
                                  const QString wrongReplyerrorString,
                                  const bool httpFinished,
                                  int baseErrorCode,
                                  QString messageReplyDataError,
                                  void(DapNodeWeb3::*parseMethod)(const QString&, int baseErrorCode),
                                  void(DapNodeWeb3::*responceError)(int code))
{
    if (error == QNetworkReply::NetworkError::NoError)
    {
        // no error reply
        auto reply = m_networkReply->getReplyData();
//        qInfo() << "reply message: " << reply;
        // check reply message error
        if (reply.size() == 0) {
            replyError(baseErrorCode + 20000, messageReplyDataError);
            return;
        }
        // parse reply
        if (parseMethod)
            (this->*parseMethod)(reply, baseErrorCode);
    }
    else
    {
        if (responceError)
            (this->*responceError)(error);
        if (httpFinished)
            replyError(baseErrorCode + 10000 + error , wrongReplyerrorString);
        else
            replyError(baseErrorCode + error, messageReplyDataError);
    }
}

void DapNodeWeb3::nodeStatusRequest()
{
    QString requesString = QString("?method=GetNodeStatus&id=%1").arg(m_connectId);
    sendRequest(requesString);
}

void DapNodeWeb3::nodeStatusOkReply(const QString& replyData, int baseErrorCode)
{
    Q_UNUSED(replyData)
    Q_UNUSED(baseErrorCode)
    emit statusOk();
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

void DapNodeWeb3::condTxCreateRequest(QString walletName, QString networkName, QString sertificateName, QString tokenName, QString value, QString unit)
{
    QString requesString = QString("?method=CondTxCreate&"
                "id=%1&"
                "net=%2&"
                "tokenName=%3&"
                "walletName=%4&"
                "certName=%5&"
                "value=%6&"
                "unit=%7&"
                "srv_uid=1")
            .arg(m_connectId)
            .arg(networkName)
            .arg(tokenName)
            .arg(walletName)
            .arg(sertificateName)
            .arg(value)
            .arg(unit);
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
    //DEBUGINFO << "parseReplyConnect" << replyData << doc;
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
//    DEBUGINFO << "orderListReply" << replyData;
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

void DapNodeWeb3::parseJsonError(QString replyData, int baseErrorCode)
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
        if (doc["status"].isString() && doc["status"].toString() != "ok")
        {
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

