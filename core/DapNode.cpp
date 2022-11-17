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
#define DEBUGINFO qDebug()<<"-----<<<-----"

const QString  DapNode::WEB3_URL      ("localhost");
const quint16  DapNode::WEB3_PORT     (8045);


//"method\=\S*\&{1}"

DapNode:: DapNode(QObject * obj, int requestTimeout) :
    QObject(obj), m_requestTimeout(requestTimeout),
    m_connectId(QString()),
    m_networkReply(nullptr),
    m_networkRequest(QString()),
    m_stateMachine(new NodeConnectStateMachine)
{
    m_httpClient = new DapNetworkAccessManager();
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
        if (error == QNetworkReply::NetworkError::NoError)
            nodeStatusReply();
        else
            replyError(DapNodeErrors::NetworkReplyStatusError,errorString, httpFinished);
        return;
    }
    // wallets list reply
    if (networkRequest.contains("method=GetWallets"))
    {
        if (error == QNetworkReply::NetworkError::NoError)
            walletsListReply();
        else
            replyError(DapNodeErrors::NetworkReplyWalletsError,errorString, httpFinished);
        return;
    }
    // networks list reply
    if (networkRequest.contains("method=GetNetworks"))
    {
        if (error == QNetworkReply::NetworkError::NoError)
            networksListReply();
        else
            replyError(DapNodeErrors::NetworkReplyNetworksListError,errorString, httpFinished);
        return;
    }
    // data wallets reply
    if (networkRequest.contains("method=GetDataWallet"))
    {
        if (error == QNetworkReply::NetworkError::NoError)
            dataWalletReply();
        else
            replyError(DapNodeErrors::NetworkReplyNetworksListError,errorString, httpFinished);
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

void DapNode::nodeStatusRequest()
{
    QString requesString = QString("?method=GetNodeStatus&id=%1").arg(m_connectId);
    sendRequest(requesString);
}

void DapNode::nodeStatusReply()
{
    auto reply = m_networkReply->getReplyData();
    if (reply.size() <= 0) {
        replyError(DapNodeErrors::NetworkReplyStatusError, "Wrong reply status");
        return;
    }
    qInfo() << "reply message: " << reply;
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

void DapNode::walletsListReply()
{
    auto reply = m_networkReply->getReplyData();
    qInfo() << "reply message: " << reply;
    if (reply.size() <= 0) {
        replyError(DapNodeErrors::NetworkWrongReplyError, "Wrong reply wallets");
        return;
    }
    parseReplyWallets(reply);
}


void DapNode::networksListReply()
{
    auto reply = m_networkReply->getReplyData();
    qInfo() << "reply message: " << reply;
    if (reply.size() <= 0) {
        replyError(DapNodeErrors::NetworkWrongReplyError, "Wrong reply nnetworks");
        return;
    }
    parseReplyNetworks(reply);
}


void DapNode::dataWalletReply()
{
    auto reply = m_networkReply->getReplyData();
    qInfo() << "reply message: " << reply;
    if (reply.size() <= 0) {
        replyError(DapNodeErrors::NetworkWrongReplyError, "Wrong reply nnetworks");
        return;
    }
    parseDataWallet(reply);
}


void DapNode::parseReplyConnect(const QString replyData)
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


void DapNode::parseReplyWallets(const QString replyData)
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


void DapNode::parseReplyNetworks(const QString replyData)
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
        emit walletsDataReady(m_walletsData);
        emit walletListIsEmpty();
    }
}

void DapNode::parseDataWallet(const QString replyData)
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


void DapNode::initStates()
{
    DEBUGINFO << "initStates";
    // node search  initialState -> startCheckingNode
    m_stateMachine->initialState.addTransition(this, &DapNode::startNodeDetection,
                                               &m_stateMachine->nodeDetection);

    // node detection -> node detected
    m_stateMachine->nodeDetection.addTransition(this, &DapNode::nodeDetected,
                                             &m_stateMachine->nodeConnection);
    // node detection -> node detected
    m_stateMachine->nodeDetection.addTransition(this, &DapNode::checkNodeStatus,
                                             &m_stateMachine->nodeGetStatus);
    // node detection -> get node status
    m_stateMachine->nodeDetection.addTransition(this, &DapNode::nodeNotDetected,
                                             &m_stateMachine->nodeNotDetected);

    // node not detected -> node detection, repeat detection
    m_stateMachine->nodeNotDetected.addTransition(this, &DapNode::repeatNodeDetection,
                                                  &m_stateMachine->nodeDetection);

    // node connection -> node connected, wallets list request
    m_stateMachine->nodeConnection.addTransition(this, &DapNode::connectionIdReceived,
                                                &m_stateMachine->nodeGetStatus);
    // node connection -> node not connected, repeat connection
    m_stateMachine->nodeConnection.addTransition(this, &DapNode::nodeNotConnected,
                                                &m_stateMachine->nodeNotConnected);

    // node not connected -> node connection, repeat connection
    m_stateMachine->nodeNotConnected.addTransition(this, &DapNode::repeatNodeConnection,
                                                &m_stateMachine->nodeConnection);

    // node status !ok -> nodeGetWallets
    m_stateMachine->nodeGetStatus.addTransition(this, &DapNode::errorDetect,
                                             &m_stateMachine->initialState);
    // node status ok -> initialState
    m_stateMachine->nodeGetStatus.addTransition(this, &DapNode::statusOk,
                                             &m_stateMachine->nodeGetWallets);
    //
    m_stateMachine->nodeGetWallets.addTransition(this, &DapNode::walletsReceived,
                                                 &m_stateMachine->nodeGetNetworks);
    //
    m_stateMachine->nodeGetWallets.addTransition(this, &DapNode::errorDetect,
                                                 &m_stateMachine->initialState);
    //
    m_stateMachine->nodeGetNetworks.addTransition(this, &DapNode::networksReceived,
                                                 &m_stateMachine->nodeGetDataWallet);
    //
    m_stateMachine->nodeGetNetworks.addTransition(this, &DapNode::errorDetect,
                                                 &m_stateMachine->initialState);

    m_stateMachine->nodeGetDataWallet.addTransition(this, &DapNode::walletListIsEmpty,
                                                    &m_stateMachine->initialState);

    m_stateMachine->nodeGetDataWallet.addTransition(this, &DapNode::errorDetect,
                                                    &m_stateMachine->initialState);

    //
    //
    connect(&m_stateMachine->initialState, &QState::entered, this, [=](){
        DEBUGINFO << "connect(&m_stateMachine->initialState, &QState::entered";
        // autostart node detection
//        startCheckingNodeRequest();
    });
    //
    connect(&m_stateMachine->nodeDetection, &QState::entered, this, [=](){
        DEBUGINFO  << "connect(&m_stateMachine->nodeDetection, &QState::entered";
    });
    //
    connect(&m_stateMachine->nodeNotDetected, &QState::entered, this, [=](){
        DEBUGINFO  << "connect(&m_stateMachine->nodeNotDetected, &QState::entered";
        QTimer::singleShot(NODE_DETECT_REQUEST_REPEAT_PERIOD, [=](){
            emit repeatNodeDetection();
        });
    });
    //
    connect(&m_stateMachine->nodeConnection, &QState::entered, this, [=](){
        DEBUGINFO  << "connect(&m_stateMachine->nodeConnection, &QState::entered";
    });
    //
    connect(&m_stateMachine->nodeNotConnected, &QState::entered, this, [=](){
        DEBUGINFO  << "connect(&m_stateMachine->nodeNotConnected, &QState::entered";
        QTimer::singleShot(NODE_CONNECT_REQUEST_REPEAT_PERIOD, [=](){
            emit repeatNodeConnection();
        });
    });

    connect(&m_stateMachine->nodeDetection,  &QState::entered, this, &DapNode::nodeDetectedRequest);
    connect(&m_stateMachine->nodeConnection, &QState::entered, this, &DapNode::nodeConnectionRequest);
    connect(&m_stateMachine->nodeGetStatus,  &QState::entered, this, &DapNode::nodeStatusRequest);
    connect(&m_stateMachine->nodeGetWallets, &QState::entered, this, &DapNode::walletsRequest);
    connect(&m_stateMachine->nodeGetNetworks, &QState::entered, this, &DapNode::networksRequest);

    connect (&m_stateMachine->nodeGetDataWallet, &QState::entered, this, [=](){
        m_walletsData = QJsonObject();
        walletDataRequest();
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
    QString requesString = QString("http://127.0.0.1:8045/?method=CondTxCreate&"
                "id=%1&net=%2&"
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
            .arg("rslCert01_public") // TODO
            .arg(value)
            .arg(unit);
    sendRequest(requesString);
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
    m_stateMachine->start();
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
