#include "DapNode.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include "DapDataLocal.h"
#include <QTimer>
#include <QDebug>


//#define SIGERROR (QNetworkReply::NetworkError::UnknownServerError + 1)
#define SIGERROR (110)
#define NODE_DETECT_REQUEST_REPEAT_PERIOD (DapNode::DEFAULT_REQUEST_TIMEOUT)
#define NODE_CONNECT_REQUEST_REPEAT_PERIOD (500)
#define LEDGER_REQUEST_REPEAT_PERIOD (2000)
#define DEBUGINFO qDebug()<<"-----<<<-----"

const QString  DapNode::WEB3_URL      ("localhost");
const quint16  DapNode::WEB3_PORT     (8045);


//"method\=\S*\&{1}"

DapNode:: DapNode(QObject * obj, int requestTimeout) :
    QObject(obj), m_requestTimeout(requestTimeout),
    m_connectId(QString()),
    m_networkReply(nullptr),
    m_networkRequest(QString()),
    m_stm(new NodeConnectStateMachine)
{
    m_httpClient = new DapNetworkAccessManager();
    initTransitions();
    initStates();
}


DapNode::~ DapNode()
{
}


void DapNode::request_GET(const QString& host,  quint16 port, const QString & urlPath,
                           DapNetworkReply &a_netReply, const QString& headers, bool ssl)
{
    m_httpClient->requestHttp_GET(host, port, urlPath, headers, a_netReply);
}


void DapNode::stopCheckingNodeRequest()
{

}


void DapNode::startCheckingNodeRequest()
{
    DEBUGINFO << "startCheckingNodeRequest";
    emit startNodeDetection();
}

void DapNode::sendRequest(QString request)
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

void DapNode::responseProcessing(const int error, const QString errorString, const bool httpFinished)
{
    // local network request
    QString networkRequest(m_networkRequest);
    // reset m_networkRequest
    m_networkRequest = QString();
    // debug info
    DEBUGINFO << "responseProcessing" << error << errorString << networkRequest;
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
                        &DapNode::nodeStatusOkReply);
        return;
    }
    // wallets list reply
    if (networkRequest.contains("method=GetWallets"))
    {
        responseParsing(error, DapNodeErrors::NetworkReplyWalletsError, errorString, httpFinished,
               DapNodeErrors::NetworkWrongReplyError, "Wrong reply networks",
               &DapNode::parseReplyWallets);
        return;
    }
    // networks list reply
    if (networkRequest.contains("method=GetNetworks"))
    {
        responseParsing(error, DapNodeErrors::NetworkReplyNetworksListError, errorString, httpFinished,
               DapNodeErrors::NetworkWrongReplyError, "Wrong reply nnetworks",
               &DapNode::parseReplyNetworks);
        return;
    }
    // data wallets reply
    if (networkRequest.contains("method=GetDataWallet"))
    {
        responseParsing(error, DapNodeErrors::NetworkReplyNetworksListError, errorString, httpFinished,
               DapNodeErrors::NetworkWrongReplyError, "Wrong reply nnetworks",
               &DapNode::parseDataWallet);
        return;

    }
    // create cond transaction reply
    if (networkRequest.contains("method=CondTxCreate"))
    {
        responseParsing(error, DapNodeErrors::NetworkReplyNetworksListError, errorString, httpFinished,
               DapNodeErrors::NetworkWrongReplyError, "Wrong reply nnetworks",
               &DapNode::parseDataWallet);
        return;
    }
    // ledger reply
    if (networkRequest.contains("method=GetLedgerTxHash"))
    {
        responseParsing(error, DapNodeErrors::NetworkReplyNetworksListError, errorString, httpFinished,
               DapNodeErrors::NetworkWrongReplyError, "Wrong reply ledger",
               &DapNode::parseLedgerReply);
        return;
    }

}


void DapNode::nodeDetectedRequest()
{
    DEBUGINFO << "sendRequest(\"\")";
    sendRequest("");
}

void DapNode::nodeConnectionRequest()
{
    // http://127.0.0.1:8045/?method=Connect
    sendRequest("?method=Connect");
}


void DapNode::connectReply()
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

void DapNode::responseParsing(const int error, DapNodeErrors errorType, const QString errorString, const bool httpFinished,
                     DapNodeErrors getReplyDataError, QString messageReplyDataError,
                     void(DapNode::*parseMethod)(const QString&))
{
    if (error == QNetworkReply::NetworkError::NoError)
    {
        // no error reply
        auto reply = m_networkReply->getReplyData();
        qInfo() << "reply message: " << reply;
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

void DapNode::nodeStatusRequest()
{
    QString requesString = QString("?method=GetNodeStatus&id=%1").arg(m_connectId);
    sendRequest(requesString);
}

void DapNode::nodeStatusOkReply(const QString& replyData)
{
    Q_UNUSED(replyData)
    emit statusOk();
}

void DapNode::walletsRequest()
{
    //http://127.0.0.1:8045/?method=GetWallets&id=0x7D6878E27691240294BE6DE326F56FD1344087024F0FBBEA98B4A04321D21F73
    QString requesString = QString("?method=GetWallets&id=%1").arg(m_connectId);
    sendRequest(requesString);
}

void DapNode::networksRequest()
{
    QString requesString = QString("?method=GetNetworks&id=%1").arg(m_connectId);
    sendRequest(requesString);
}

void DapNode::parseReplyConnect(const QString& replyData)
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
            emit connectionIdReceived();
            DEBUGINFO << "[data][id]" << m_connectId;
        }
    }
}


void DapNode::parseReplyWallets(const QString& replyData)
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
        m_walletsList.clear();
        QJsonArray dataArray = doc["data"].toArray();
        for (auto i = dataArray.cbegin(), e = dataArray.cend(); i != e; i++)
            if (i->isString())
                m_walletsList << i->toString();
        DEBUGINFO << "Wallets list: " << m_walletsList;
        emit sigReceivedWalletsList(m_walletsList);
        emit walletsReceived();
    }
}


void DapNode::parseReplyNetworks(const QString& replyData)
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
        m_networksList.clear();
        QJsonArray dataArray = doc["data"].toArray();
        for (auto i = dataArray.cbegin(), e = dataArray.cend(); i != e; i++)
            if (i->isString())
                m_networksList << i->toString();
        DEBUGINFO << "Networks list: " << m_walletsList;
        emit sigReceivedNetworksList(m_networksList);
        emit networksReceived();
    }
}

void DapNode::walletDataRequest()
{
    DEBUGINFO << "walletDataRequest" << m_walletsList;
    if (m_walletsList.size() > 0)
    {
        QString requesString = QString("?method=GetDataWallet&id=%1&walletName=%2")
                .arg(m_connectId)
                .arg(m_walletsList[0]);
        sendRequest(requesString);
    }
    else
    {
        emit sigWalletsDataReady(m_walletsData);
        emit walletListIsEmpty();
    }
}

void DapNode::parseDataWallet(const QString& replyData)
{
    DEBUGINFO << "walletDataReply" << replyData;
    parseJsonError(replyData.toUtf8());
    if (jsonError())
        return;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    if (doc["data"].isArray())
    {
        // wallet data
        m_walletsData[m_walletsList[0]] = doc["data"];
        m_walletsList.pop_front();
        walletDataRequest();
    }
}

void DapNode::parseJsonError(QString replyData)
{
    // reply exmple
    //    "{ ... "
    //    "    \"errorMsg\": \"\","
    //    "    \"status\": \"ok\""
    //    "}";
    m_parseJsonError = false;
    QJsonDocument doc = QJsonDocument::fromJson(replyData.toUtf8());
    DEBUGINFO << "parseJsonError" << doc;
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


void DapNode::initTransitions()
{
    DEBUGINFO << "initStates";
    // node search  initialState -> startCheckingNode
    m_stm->initialState.addTransition(this, &DapNode::startNodeDetection,
    &m_stm->nodeDetection);

    // node detection -> node detected
    m_stm->nodeDetection.addTransition(this, &DapNode::nodeDetected,
    &m_stm->nodeConnection);
    // node detection -> node detected
    m_stm->nodeDetection.addTransition(this, &DapNode::checkNodeStatus,
    &m_stm->nodeGetStatus);
    // node detection -> get node status
    m_stm->nodeDetection.addTransition(this, &DapNode::nodeNotDetected,
    &m_stm->nodeNotDetected);

    // node not detected -> node detection, repeat detection
    m_stm->nodeNotDetected.addTransition(this, &DapNode::repeatNodeDetection,
    &m_stm->nodeDetection);

    // node connection -> node connected, wallets list request
    m_stm->nodeConnection.addTransition(this, &DapNode::connectionIdReceived,
    &m_stm->nodeGetStatus);
    // node connection -> node not connected, repeat connection
    m_stm->nodeConnection.addTransition(this, &DapNode::nodeNotConnected,
    &m_stm->nodeNotConnected);

    // node not connected -> node connection, repeat connection
    m_stm->nodeNotConnected.addTransition(this, &DapNode::repeatNodeConnection,
    &m_stm->nodeConnection);

    // node status !ok -> nodeGetWallets
    m_stm->nodeGetStatus.addTransition(this, &DapNode::errorDetect,
    &m_stm->initialState);
    // node status ok -> initialState
    m_stm->nodeGetStatus.addTransition(this, &DapNode::statusOk,
    &m_stm->getWallets);
    //
    m_stm->getWallets.addTransition(this, &DapNode::walletsReceived,
    &m_stm->getNetworks);
    //
    m_stm->getWallets.addTransition(this, &DapNode::errorDetect,
    &m_stm->initialState);
    //
    m_stm->getNetworks.addTransition(this, &DapNode::networksReceived,
    &m_stm->getDataWallet);
    //
    m_stm->getNetworks.addTransition(this, &DapNode::errorDetect,
    &m_stm->initialState);

    m_stm->getDataWallet.addTransition(this, &DapNode::walletListIsEmpty,
    &m_stm->initialState);

    m_stm->getDataWallet.addTransition(this, &DapNode::errorDetect,
    &m_stm->initialState);
    //
    /***************************************
    * transaction workflow
    ***************************************/
    // to conditional tx create
    m_stm->initialState.addTransition(this, &DapNode::condTxCreateRequest,
    &m_stm->condTxCreate);
    // to check ledger if create successfuly
    m_stm->condTxCreate.addTransition(this, &DapNode::sigCondTxCreateSuccess,
    &m_stm->ledgerTxHashRequest);
    // to ledger empty
    m_stm->ledgerTxHashRequest.addTransition(this, &DapNode::errorDetect,
    &m_stm->ledgerTxHashEmpty);
    // to init state
    m_stm->ledgerTxHashRequest.addTransition(this, &DapNode::sigLedgerContainHash,
    &m_stm->initialState);
    // to init state if error
    m_stm->condTxCreate.addTransition(this, &DapNode::errorDetect,
    &m_stm->initialState);
}


void DapNode::initStates()
{
    // initial state
    connect(&m_stm->initialState, &QState::entered, this, [=](){
        DEBUGINFO << "&initialState, &QState::entered";
        // autostart node detection
//        startCheckingNodeRequest();
    });
    // node detection
    connect(&m_stm->nodeDetection, &QState::entered, this, [=](){
        DEBUGINFO  << "&nodeDetection, &QState::entered";
    });
    // node detected
    connect(&m_stm->nodeNotDetected, &QState::entered, this, [=](){
        DEBUGINFO  << "&nodeNotDetected, &QState::entered";
        QTimer::singleShot(NODE_DETECT_REQUEST_REPEAT_PERIOD, [=](){
            emit repeatNodeDetection();
        });
    });
    //
    connect(&m_stm->nodeConnection, &QState::entered, this, [=](){
        DEBUGINFO  << "&nodeConnection, &QState::entered";
    });
    // node connected
    connect(&m_stm->nodeNotConnected, &QState::entered, this, [=](){
        DEBUGINFO  << "&nodeNotConnected, &QState::entered";
        QTimer::singleShot(NODE_CONNECT_REQUEST_REPEAT_PERIOD, [=](){
            emit repeatNodeConnection();
        });
    });
    // node detection request
    connect(&m_stm->nodeDetection,  &QState::entered, this, &DapNode::nodeDetectedRequest);
    // connection request
    connect(&m_stm->nodeConnection, &QState::entered, this, &DapNode::nodeConnectionRequest);
    // get node status
    connect(&m_stm->nodeGetStatus,  &QState::entered, this, &DapNode::nodeStatusRequest);
    // get wallets
    connect(&m_stm->getWallets, &QState::entered, this, &DapNode::walletsRequest);
    // get networks
    connect(&m_stm->getNetworks, &QState::entered, this, &DapNode::networksRequest);
    // get wallets data
    connect (&m_stm->getDataWallet, &QState::entered, this, [=](){
        m_walletsData = QJsonObject();
        walletDataRequest();
    });
    // create conditional transaction
    connect(&m_stm->condTxCreate, &QState::entered, this, &DapNode::slotCondTxCreateRequest);
    // check ledger
    connect(&m_stm->ledgerTxHashRequest, &QState::entered, this, [=](){
        DEBUGINFO  << "&ledgerTxHashRequest, &QState::entered";
        QTimer::singleShot(LEDGER_REQUEST_REPEAT_PERIOD, [=](){
            DapNode::getLedgerTxHashRequest();
        });
    });
    // node detected signal
    connect(this, &DapNode::nodeDetected, this, &DapNode::sigNodeDetected);
    connect(this, &DapNode::checkNodeStatus, this, &DapNode::sigNodeDetected);

    // create conditional transaction
    connect(this, &DapNode::condTxCreateRequest, this, &DapNode::slotCondTxCreateRequest);

}

void DapNode::slotCondTxCreateRequest(QString walletName, QString networkName, QString tokenName, QString value, QString unit)
{
    qDebug() << "uuuuuuuuuuuuuuuuuuuuuuuu" << walletName << networkName << tokenName << value << unit;
    walletName = m_walletName;
    networkName = m_selectedNetwork;
    tokenName = m_tokenName;
    value = m_value;
    unit = m_unit;
    m_sertificateName = QString("rslCert01_public"); // TODO
}

void DapNode::condTxCreateRequest()
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
            .arg(m_selectedNetwork)
            .arg(m_tokenName)
            .arg(m_walletName)
            .arg(m_sertificateName)
            .arg(m_value)
            .arg(m_unit);
    sendRequest(requesString);
}


void DapNode::parseCondTxCreateReply(const QString& replyData)
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
        m_transactionHash = doc["data"].toObject()["hash"].toString();
        m_transactionHash = "0x" + m_transactionHash;
        emit sigCondTxCreateSuccess();
    }
}

void DapNode::getLedgerTxHashRequest()
{
    QString requesString = QString("http://127.0.0.1:8045/?method=GetLedgerTxHash&"
                "id=%1&hashTx=%2&net=%3")
            .arg(m_connectId)
            .arg(m_transactionHash)
            .arg(m_selectedNetwork);
    sendRequest(requesString);
}

void DapNode::parseLedgerReply(const QString& replyData)
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

void DapNode::replyError(DapNodeErrors errorType, const QString errorString, const bool httpFinished)
{
    DEBUGINFO  << "Reply error" << DapNodeErrorCode(errorType, httpFinished) << errorString;
    emit errorDetect();
    emit sigError(DapNodeErrorCode(errorType, httpFinished),
                  errorString);
}

void DapNode::start()
{
    DEBUGINFO  << "start DapNode";
    m_stm->start();
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
