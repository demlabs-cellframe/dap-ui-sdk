/*
 Copyright (c) 2017-2018 (c) Project "DeM Labs Inc" https://github.com/demlabsinc
  All rights reserved.

 This file is part of DAP (Deus Applications Prototypes) the open source project

    DAP (Deus Applicaions Prototypes) is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DAP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with any DAP based project.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DapNode.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include "DapDataLocal.h"
#include <QDebug>


const QString  DapNode::WEB3_URL      ("localhost");
const quint16  DapNode::WEB3_PORT     (8045);


#define SIGERROR (QNetworkReply::NetworkError::UnknownServerError + 1)
//"method\=\S*\&{1}"

DapNode:: DapNode(QObject * obj, int requestTimeout) :
    QObject(obj), m_requestTimeout(requestTimeout),
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

}

void DapNode::sendRequest(QString request)
{
    if (!m_networkRequest.isNull())
        return;
    if (m_networkReply == nullptr)
    {
        m_networkReply =  new DapNetworkReply;
        connect( m_networkReply, &DapNetworkReply::finished, this, [=] {
            responseProcessing(m_networkReply->error(), m_networkReply->errorString());
            m_networkRequest = QString();
        });
        connect( m_networkReply, &DapNetworkReply::sigError, this, [=] {
            responseProcessing(SIGERROR);
            m_networkRequest = QString();
        });
    }
    // send request
    request_GET(WEB3_URL, WEB3_PORT, request, *m_networkReply);
}

void DapNode::responseProcessing(const int error, const QString errorString)
{
    // check dashboard reply
    if (m_networkRequest == "")
    {
    }
    // connect reply
    if (m_networkRequest.contains("?method=Connect"))
    {
        if (error == QNetworkReply::NetworkError::NoError)
            connectReply();
        else if (error == SIGERROR)
            replyError(DapNodeErrors::NetworkReplyConnectError, errorString);
        else
            replyError(DapNodeErrors::NetworkReplyFinishedConnectError, errorString);
    }
    // wallets list reply
    if (m_networkRequest.contains("?method=GetWallets&id"))
    {
        if (error == QNetworkReply::NetworkError::NoError)
            walletsListReply();
        else if (error == SIGERROR)
            replyError(DapNodeErrors::NetworkReplyWalletsError,errorString);
        else
            replyError(DapNodeErrors::NetworkReplyFinishedWalletsError, errorString);
    }
    //
}


void DapNode::cellframeDashboardTest()
{
    sendRequest("");
}

void DapNode::connectRequest()
{
    sendRequest("?method=Connect");
}


void DapNode::connectReply()
{
    auto reply = m_connectReply->getReplyData();
    if (reply.size() <= 0) {
        replyError(DapNodeErrors::NetworkWrongReplyError, "Wrong reply connect");
        return;
    }
    qInfo() << "Connect reply message: " << reply;
    parseReplyConnect(reply);
}

void DapNode::walletsRequest()
{
    QString requesString = QString("?method=GetWallets&id=%1").arg(m_connectId);
    sendRequest(requesString);
}


void DapNode::cellframeDashboardReply()
{
    emit nodeFound();
}


void DapNode::walletsListReply()
{
    auto reply = m_walletsListReply->getReplyData();
    if (reply.size() <= 0) {
        replyError(DapNodeErrors::NetworkWrongReplyError, "Wrong reply wallets");
        return;
    }
    qInfo() << "reply message: " << reply;
    parseReplyWallets(reply);
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
    if (doc["data"].isObject())
    {
        QJsonObject data = doc["data"].toObject();
        // connection id
        if (data["id"].isString())
        {
            m_connectId = data["id"].toString();
            qDebug() << "[data][id]" << m_connectId;
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
        qDebug() << "Wallets list: " << m_walletsList;
        emit sigReceivedWalletsList(m_walletsList);
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
    if(doc.isNull())
    {
        replyError(DapNodeErrors::JsonError);
        m_parseJsonError = true;
        return;
    }
    else
    {
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
            return;
        }
    }
}


void DapNode::initStates()
{
    // node search  initialState -> startCheckingNode
    m_stateMachine->initialState.addTransition(this, &DapNode::startCheckingNode,
                                               &m_stateMachine->checkFound);
    // node found, connect request
    m_stateMachine->checkFound.addTransition(this, &DapNode::nodeFound,
                                             &m_stateMachine->connectToNode);
    // connected, wallets list request
    m_stateMachine->connectToNode.addTransition(this, &DapNode::nodeConnected,
                                                &m_stateMachine->nodeGetWallets);


    connect(&m_stateMachine->checkFound, &QState::entered, this, &DapNode::cellframeDashboardTest);
    connect(&m_stateMachine->connectToNode, &QState::entered, this, &DapNode::connectRequest);
    connect(&m_stateMachine->nodeGetWallets, &QState::entered, this, &DapNode::walletsRequest);

}


void DapNode::replyError(DapNodeErrors errorType, const QString errorString)
{
    qDebug() << "Getting wallets error" << DapNodeErrorCode(errorType) << errorString;
    emit sigError(DapNodeErrorCode(errorType),
                  errorString);
}

int DapNodeErrorCode(DapNodeErrors errorType)
{
    int errorCode = 50000;  // unknown type of error
    if (errorType == DapNodeErrors::NetworkReplyFinishedConnectError)      errorCode += 1;
    if (errorType == DapNodeErrors::NetworkReplyConnectError)              errorCode += 2;
    if (errorType == DapNodeErrors::NetworkReplyFinishedWalletsError)      errorCode += 3;
    if (errorType == DapNodeErrors::NetworkReplyWalletsError)              errorCode += 4;
    if (errorType == DapNodeErrors::NetworkWrongReplyError)                errorCode += 5;
    if (errorType == DapNodeErrors::TimerElapsed)                          errorCode += 6;
    if (errorType == DapNodeErrors::JsonError)                             errorCode += 7;
    if (errorType == DapNodeErrors::StatusError)                           errorCode += 8;
    return errorCode;
}
