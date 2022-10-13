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


DapNode:: DapNode(QObject * obj, int requestTimeout) :
    QObject(obj), m_requestTimeout(requestTimeout)
{
    m_httpClient = new DapNetworkAccessManager();
}


DapNode::~ DapNode()
{
}


void DapNode::request_GET(const QString& host,  quint16 port, const QString & urlPath,
                           DapNetworkReply &a_netReply, const QString& headers, bool ssl)
{
    m_httpClient->requestHttp_GET(host, port, urlPath, headers, a_netReply);
}


void DapNode::requestWalletsListStop()
{

}


void DapNode::requestWalletsList()
{

}


void DapNode::connectRequest()
{
    DapNetworkReply *networkReply =  new DapNetworkReply;
    request_GET(WEB3_URL, WEB3_PORT, "?method=Connect", *networkReply );
    connect( networkReply, &DapNetworkReply::finished, this, [=] {
      if ( networkReply->error() == QNetworkReply::NetworkError::NoError ) {
//        emit sigResponse(timer->elapsed());
          connectReply();
      } else {
          replyError(DapNodeErrors::NetworkReplyFinishedConnectError, networkReply->errorString());
      }
    });
    connect( networkReply, &DapNetworkReply::sigError, this, [=] {
        replyError(DapNodeErrors::NetworkReplyConnectError, networkReply->errorString());
    });
    m_connectReply = networkReply;
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
    DapNetworkReply *networkReply =  new DapNetworkReply;
    QString requesString = QString("?method=GetWallets&id=%1").arg(m_connectId);
    request_GET(WEB3_URL, WEB3_PORT, requesString, *networkReply);
    connect( networkReply, &DapNetworkReply::finished, this, [=] {
      if ( networkReply->error() == QNetworkReply::NetworkError::NoError ) {
//        emit sigResponse(timer->elapsed());
          walletsListReply();
      } else {
          replyError(DapNodeErrors::NetworkReplyFinishedWalletsError, networkReply->errorString());
      }
    });
    connect( networkReply, &DapNetworkReply::sigError, this, [=] {
        replyError(DapNodeErrors::NetworkReplyWalletsError, networkReply->errorString());
    });
    m_walletsListReply = networkReply;
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
