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


DapNodeWeb3:: DapNodeWeb3(QObject * obj, int requestTimeout) :
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
    // connect reply
    if (networkRequest.contains("method=Connect"))
    {
        if (error == QNetworkReply::NetworkError::NoError)
            connectReply();
        else
            replyError(DapNodeErrors::NetworkReplyConnectError, errorString, httpFinished);
        if (error != QNetworkReply::NetworkError::NoError)
            emit nodeNotConnected();
        return;
    }
    // node status reply
    if (networkRequest.contains("method=GetNodeStatus"))
    {
        responseParsing(error, DapNodeErrors::NetworkReplyStatusError, errorString, httpFinished,
               DapNodeErrors::NetworkReplyStatusError, "Wrong reply status",
                        &DapNodeWeb3::nodeStatusOkReply);
        return;
    }
    // wallets list reply
    if (networkRequest.contains("method=GetWallets"))
    {
        responseParsing(error, DapNodeErrors::NetworkReplyWalletsError, errorString, httpFinished,
               DapNodeErrors::NetworkWrongReplyError, "Wrong reply networks",
               &DapNodeWeb3::parseReplyWallets);
        return;
    }
    // networks list reply
    if (networkRequest.contains("method=GetNetworks"))
    {
        responseParsing(error, DapNodeErrors::NetworkReplyNetworksListError, errorString, httpFinished,
               DapNodeErrors::NetworkWrongReplyError, "Wrong reply nnetworks",
               &DapNodeWeb3::parseReplyNetworks);
        return;
    }
    // data wallets reply
    if (networkRequest.contains("method=GetDataWallet"))
    {
        responseParsing(error, DapNodeErrors::NetworkReplyNetworksListError, errorString, httpFinished,
               DapNodeErrors::NetworkWrongReplyError, "Wrong reply nnetworks",
               &DapNodeWeb3::parseDataWallet);
        return;

    }
    // get sertificates
    if (networkRequest.contains("method=GetCertificates"))
    {
        responseParsing(error, DapNodeErrors::NetworkReplyNetworksListError, errorString, httpFinished,
               DapNodeErrors::NetworkWrongReplyError, "Wrong reply nnetworks",
               &DapNodeWeb3::parseCertificates);
        return;
    }
    // create certificate
    if (networkRequest.contains("method=CreateCertificate"))
    {
        responseParsing(error, DapNodeErrors::NetworkReplyNetworksListError, errorString, httpFinished,
               DapNodeErrors::NetworkWrongReplyError, "Wrong reply nnetworks",
               &DapNodeWeb3::parseCreateCertificate);
        return;
    }
    // create cond transaction reply
    if (networkRequest.contains("method=CondTxCreate"))
    {
        responseParsing(error, DapNodeErrors::NetworkReplyNetworksListError, errorString, httpFinished,
               DapNodeErrors::NetworkWrongReplyError, "Wrong reply nnetworks",
               &DapNodeWeb3::parseCondTxCreateReply);
        return;
    }
    // ledger reply
    if (networkRequest.contains("method=GetLedgerTxHash"))
    {
        responseParsing(error, DapNodeErrors::NetworkReplyNetworksListError, errorString, httpFinished,
               DapNodeErrors::NetworkWrongReplyError, "Wrong reply ledger",
               &DapNodeWeb3::parseLedgerReply);
        return;
    }
}


void DapNodeWeb3::nodeDetectedRequest()
{
    DEBUGINFO << "sendRequest(\"\")";
    sendRequest("");
}

void DapNodeWeb3::nodeConnectionRequest()
{
    // http://127.0.0.1:8045/?method=Connect
    sendRequest("?method=Connect");
}


void DapNodeWeb3::connectReply()
{
    DEBUGINFO << "connectReply()";
    m_networkReply->getReplyData();
    auto reply = m_networkReply->getReplyData();
    if (reply.size() <= 0) {
        replyError(DapNodeErrors::NetworkWrongReplyError, "Wrong reply connect");
        return;
    }
    qInfo() << "Connect reply message: " << reply;
    parseReplyConnect(reply);
}

void DapNodeWeb3::responseParsing(const int error, DapNodeErrors errorType, const QString errorString, const bool httpFinished,
                     DapNodeErrors getReplyDataError, QString messageReplyDataError,
                     void(DapNodeWeb3::*parseMethod)(const QString&))
{
    if (error == QNetworkReply::NetworkError::NoError)
    {
        // no error reply
        auto reply = m_networkReply->getReplyData();
//        qInfo() << "reply message: " << reply;
        // check reply message error
        if (reply.size() == 0) {
            replyError(getReplyDataError, messageReplyDataError);
            return;
        }
        // parse reply
        (this->*parseMethod)(reply);
    }
    else
    {
        replyError(errorType,errorString, httpFinished);
    }
}

void DapNodeWeb3::nodeStatusRequest()
{
    QString requesString = QString("?method=GetNodeStatus&id=%1").arg(m_connectId);
    sendRequest(requesString);
}

void DapNodeWeb3::nodeStatusOkReply(const QString& replyData)
{
    Q_UNUSED(replyData)
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

void DapNodeWeb3::parseReplyConnect(const QString& replyData)
{
    // connect reply example
    //    "{"
    //    "    \"data\": {"
    //    "        \"id\": \"0xCD33094D27F8ACEBBCC90B31467E119DD2CD8B0581CEC007B126CB6BAC34CB1E\""
    //    "    },"
    //    "    \"errorMsg\": \"\","
    //    "    \"status\": \"ok\""
    //    "}";
    parseJsonError(replyData.toUtf8());
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
            DEBUGINFO << "[data][id]" << m_connectId;
        }
    }
}


void DapNodeWeb3::parseReplyWallets(const QString& replyData)
{
    // wallets reply exmple
    //    "{"
    //    "    \"data\": ["
    //    "        \"CELLTestWallet\","
    //    "        \"walletCELL\""
    //    "    ],"
    //    "    \"errorMsg\": \"\","
    //    "    \"status\": \"ok\""
    //    "}";
    parseJsonError(replyData.toUtf8());
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
        DEBUGINFO << "Wallets list: " << walletsList;
        emit sigReceivedWalletsList(walletsList);
    }
}


void DapNodeWeb3::parseReplyNetworks(const QString& replyData)
{
    // networks reply exmple
    //    "{"
    //    "    \"data\": ["
    //    "        \"CELLTestWallet\","
    //    "        \"walletCELL\""
    //    "    ],"
    //    "    \"errorMsg\": \"\","
    //    "    \"status\": \"ok\""
    //    "}";
    parseJsonError(replyData.toUtf8());
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
        DEBUGINFO << "Networks list: " << networksList;
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

void DapNodeWeb3::parseDataWallet(const QString& replyData)
{
//    DEBUGINFO << "walletDataReply" << replyData;
    parseJsonError(replyData.toUtf8());
    if (jsonError())
        return;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    if (doc["data"].isArray())
    {
        // wallet data
        DEBUGINFO << "walletDataReply" << doc["data"].toArray();
        emit sigWalletDataReady(doc["data"].toArray());
    }
}

void DapNodeWeb3::parseJsonError(QString replyData)
{
    // reply exmple
    //    "{ ... "
    //    "    \"errorMsg\": \"\","
    //    "    \"status\": \"ok\""
    //    "}";
    m_parseJsonError = false;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
//    DEBUGINFO << "parseJsonError" << doc;
    if(doc.isNull())
    {
        replyError(DapNodeErrors::JsonError);
        m_parseJsonError = true;
        return;
    }
    else
    {
        DEBUGINFO << "parseJsonError status" << doc["status"] << doc["status"].toString();
        if (doc["status"].isString() && doc["status"].toString() != "ok")
        {
            if (doc["errorMsg"].isString())
            {
                replyError(DapNodeErrors::StatusError, doc["errorMsg"].toString());
                m_parseJsonError = true;
                return;
            }
            replyError(DapNodeErrors::StatusError, "");
            m_parseJsonError = true;
            return;
        }
        else
        {
            // "status": "ok"
            DEBUGINFO << "status ok";
            return;
        }
    }
}

void DapNodeWeb3::parseCertificates(const QString& replyData)
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
    parseJsonError(replyData.toUtf8());
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
        DEBUGINFO << "Certificates: " << certificates;
        emit sigReceivedCertificatestList(certificates);
    }
}

void DapNodeWeb3::parseCreateCertificate(const QString& replyData)
{
//    {
//        "data": {
//            "name": "rslCert02",
//            "type": "sig_dil"
//        },
//        "errorMsg": "",
//        "status": "ok"
//    }
    parseJsonError(replyData.toUtf8());
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


void DapNodeWeb3::condTxCreateRequest(QString walletName, QString networkName, QString sertificateName, QString tokenName, QString value, QString unit)
{
    QString requesString = QString("http://127.0.0.1:8045/?method=CondTxCreate&"
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
    QString requesString = QString("http://127.0.0.1:8045/?method=CreateCertificate&"
                "id=%1&"
                "certType=%2&"
                "certName=%3&")
            .arg(m_connectId)
            .arg(certType)
            .arg(certName);
    sendRequest(requesString);
}

void DapNodeWeb3::getCertificates()
{
    QString requesString = QString("http://127.0.0.1:8045/?method=GetCertificates&"
                "id=%1&")
            .arg(m_connectId);
    sendRequest(requesString);
}

void DapNodeWeb3::parseCondTxCreateReply(const QString& replyData)
{
//    {
//        "data": {
//            "hash": "385A0652191C419AB7C09907D84EB1779DC0EF6EC55C952D02EA871325B2A80C"
//        },
//        "errorMsg": "",
//        "status": "ok"
//    }
    DEBUGINFO << "parseCondTxCreateReply" << replyData;
    parseJsonError(replyData.toUtf8());
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

void DapNodeWeb3::getLedgerTxHashRequest(QString transactionHash, QString networkName)
{
    QString requesString = QString("http://127.0.0.1:8045/?method=GetLedgerTxHash&"
                "id=%1&hashTx=%2&net=%3")
            .arg(m_connectId)
            .arg(transactionHash)
            .arg(networkName);
    sendRequest(requesString);
}

void DapNodeWeb3::parseLedgerReply(const QString& replyData)
{
//    {
//        "data": {
//            "string": "transaction: hash: 0x385A0652191C419AB7C09907D84EB1779DC0EF6EC55C952D02EA871325B2A80C\n TS Created: Wed Nov 16 22:42:18 2022\n Token ticker: tKEL\n Items:\n IN:\nTx_prev_hash: 0x62C0CDEB24CFDE8ED40367CB180AE0C660DC4E47318698C91B26CD4942D9F30F\n Tx_out_prev_idx: 1\n OUT COND:\n Header:\n ts_expires: never\n value: 0.000000000000000001 (1)\n subtype: DAP_CHAIN_TX_OUT_COND_SUBTYPE_SRV_PAY\n unit: 0x00000003\n uid: 0x0000000000000001\n pkey: 0xD304A59B5C0A0A95149A2B34F77270E40ACBEBCCBC9536099928DBDB8DF3882C\n max price: 0.0 (0) \n OUT:\n Value: 989.899999999999999988 (989899999999999999988)\n Address: iDAeSXFFiwH2LyhquYZn7ce7wydGX8QUDRmCW1CKsMDDRRMvAA9CvRooswfp91fEz8hSqH6XxCysMWSxGUZGQSB9FXdUuMfgvWxgwKK7\nSignature: \nType: sig_dil\nPublic key hash: 0xAC75D1E675BB182FE1A5171BD50B6833C50BD29AC06A75491CCF7F411D53CB80\nPublic key size: 1196\nSignature size: 2096\n\n\r\n\n"
//        },
//        "errorMsg": "",
//        "status": "ok"
//    }
    DEBUGINFO << "parseLedgerReply" << replyData;
    parseJsonError(replyData.toUtf8());
    if (jsonError())
        return;
    emit sigLedgerContainHash();
}

void DapNodeWeb3::replyError(DapNodeErrors errorType, const QString errorString, const bool httpFinished)
{
    DEBUGINFO  << "Reply error" << DapNodeErrorCode(errorType, httpFinished) << errorString;
    emit sigError(DapNodeErrorCode(errorType, httpFinished),
                  errorString);
}

int DapNodeErrorCode(DapNodeErrors errorType, const bool httpFinished)
{
    int errorCode = 500000;  // unknown type of error
    if (errorType == DapNodeErrors::NetworkReplyConnectError && !httpFinished)          errorCode += 20000;
    if (errorType == DapNodeErrors::NetworkReplyConnectError && httpFinished)           errorCode += 20000;
    if (errorType == DapNodeErrors::NetworkReplyWalletsError && !httpFinished)          errorCode += 30000;
    if (errorType == DapNodeErrors::NetworkReplyWalletsError && httpFinished)           errorCode += 40000;
    if (errorType == DapNodeErrors::NetworkWrongReplyError)                             errorCode += 50000;
    if (errorType == DapNodeErrors::TimerElapsed)                                       errorCode += 60000;
    if (errorType == DapNodeErrors::JsonError)                                          errorCode += 70000;
    if (errorType == DapNodeErrors::StatusError)                                        errorCode += 80000;
    if (errorType == DapNodeErrors::NetworkReplyStatusError && httpFinished)            errorCode += 90000;
    if (errorType == DapNodeErrors::NetworkReplyStatusError && !httpFinished)           errorCode += 100000;
    if (errorType == DapNodeErrors::NetworkReplyNetworksListError && httpFinished)      errorCode += 110000;
    if (errorType == DapNodeErrors::NetworkReplyNetworksListError && !httpFinished)     errorCode += 120000;
    return errorCode;
}
