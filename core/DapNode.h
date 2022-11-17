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

#ifndef DAPNODE_H
#define DAPNODE_H

#include <QCoreApplication>
#include <QString>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QMap>
#include <QList>
#include <QThread>
#include "DapConnectClient.h"
#include <DapCrypt.h>
#include "DapDataLocal.h"

#include "dap_client_http.h"
#include "DapNetworkAccessManager.h"
#include "DapNetworkReply.h"

#include "DapCmdNode.h"

#include <QStateMachine>
#include <QState>

class NodeConnectStateMachine
{
public:
    explicit NodeConnectStateMachine()
    { init(); }
    void start() { nodeConnectMachine.start();}
    QState initialState;
    QState nodeDetection;
    QState nodeNotDetected;
    QState nodeConnection;
    QState nodeNotConnected;
    QState nodeGetStatus;
    QState nodeGetWallets;
    QState nodeGetNetworks;
    QState nodeGetDataWallet;
    QState nodeCondTxCreate;
private:
    QStateMachine nodeConnectMachine;
    void init(){
        nodeConnectMachine.addState(&initialState);
        nodeConnectMachine.addState(&nodeDetection);
        nodeConnectMachine.addState(&nodeNotDetected);
        nodeConnectMachine.addState(&nodeConnection);
        nodeConnectMachine.addState(&nodeNotConnected);
        nodeConnectMachine.addState(&nodeGetStatus);
        nodeConnectMachine.addState(&nodeGetWallets);
        nodeConnectMachine.addState(&nodeGetNetworks);
        nodeConnectMachine.addState(&nodeGetDataWallet);
        nodeConnectMachine.addState(&nodeCondTxCreate);
        nodeConnectMachine.setInitialState(&initialState);
        qDebug() << "nodeConnectMachine::init";
    }
};

enum class DapNodeErrors
{
    NetworkReplyConnectError,
    NetworkReplyStatusError,
    NetworkReplyWalletsError,
    NetworkReplyNetworksListError,
    NetworkWrongReplyError,
    TimerElapsed,
    JsonError,
    StatusError
};

int DapNodeErrorCode(DapNodeErrors, const bool httpFinished);

class DapNode : public QObject
{
    Q_OBJECT
private:
    const int m_requestTimeout;
    DapNetworkAccessManager * m_httpClient;
    // cellframe dashboard connect id
    QString m_connectId;
    // wallets list
    QStringList m_walletsList;
    // network list
    QStringList m_networksList;
    DapNetworkReply *m_networkReply;
    QString m_networkRequest;
    //
    bool m_parseJsonError;
    // state machine
    NodeConnectStateMachine *m_stateMachine;
    QJsonObject m_walletsData;

public:
    static const int DEFAULT_REQUEST_TIMEOUT = 10000; // 10 sec
    static const QString WEB3_URL;
    static const quint16 WEB3_PORT;

    DapNode(QObject * obj = Q_NULLPTR, int requestTimeout = DEFAULT_REQUEST_TIMEOUT);
    ~DapNode();
    void start();

private:
    void request_GET(const QString& host,  quint16 port,
                     const QString & urlPath, DapNetworkReply &a_netReply,
                     const QString &headers = "", bool ssl = false);
    bool jsonError() { return m_parseJsonError; }
    void initStates();
    void responseProcessing(const int error, const QString errorString = "", const bool httpFinished = true);


public slots:
    void startCheckingNodeRequest();
    void stopCheckingNodeRequest();
    void slotCondTxCreateRequest(QString walletName, QString networkName, QString tokenName, QString value, QString unit);

private slots:
    void sendRequest(QString request);
    void nodeDetectedRequest();
    void nodeConnectionRequest();
    void connectReply();
    void nodeStatusRequest();
    void nodeStatusReply();
    void walletsRequest();
    void walletsListReply();
    void networksRequest();
    void networksListReply();
    void walletDataRequest();
    void dataWalletReply();
    void replyError(DapNodeErrors errorType, const QString errorString = QString(""), const bool httpFinished = true);
    void parseReplyConnect(const QString replyData);
    void parseReplyNetworks(const QString replyData);
    void parseReplyWallets(const QString replyData);
    void parseDataWallet(const QString replyData);
    void parseJsonError(QString replyData);

signals:
    // -------- output signals --------
    void sigReceivedWalletsList(QStringList);
    void sigReceivedNetworksList(QStringList);
    void sigError(int errorCode, QString errorMessage);
    void sigNodeDetected();
    void walletsDataReady(QJsonObject);
    // --- external (input) signals ---
    void startNodeDetection();
    void stopNodeDetection();
    void dataWalletRequest();
    void condTxCreateRequest(QString walletName, QString networkName, QString tokenName, QString value, QString unit);
    // ------- internal signals --------
    void errorDetect();
    void nodeDetected();
    void nodeNotDetected();
    void repeatNodeDetection();
    void nodeNotConnected();
    void repeatNodeConnection();
    void connectionIdReceived();
    void walletsReceived();
    void networksReceived();
    void checkNodeStatus();
    void statusOk();
    void walletListIsEmpty();

};



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
//    {
//        "address": "mJUUJk6Yk2gBSTjcDvqqLvGTgcNFZeZbCmrD3Mb1QycVPZmZcUVmH9WW8aLixh6M7XfzLWYGbSGSGwMSV6PiVwcLnbjZvoAfmQEfqEsN",
//        "network": "subzero",
//        "tokens": [
//            {
//                "balance": "0",
//                "datoshi": "0",
//                "tokenName": "0"
//            }
//        ]
//    },
//    {
//        "address": "Rj7J7MiX2bWy8sNyaJdTGtjsUzwUbDHsfT3EnKN4Nhcmcf2bRxgx5GoXxgR6q3YDu8s64PGvbbHhh21jSgns6tjTXy6ADf4ga1XdnJnk",
//        "network": "Backbone",
//        "tokens": [
//            {
//                "balance": "0",
//                "datoshi": "0",
//                "tokenName": "0"
//            }
//        ]
//    },
//    {
//        "address": "iDAeSXFFiwH2LyhqupWKEHqfCbSQFvTXoPm4bU9VyboenWTxeviL5AjVo54dXCbGctryNgFiV91UcD8hmMaNChgyGuQusikS5W9dUYGA",
//        "network": "kelvpn-minkowski",
//        "tokens": [
//            {
//                "balance": "0",
//                "datoshi": "0",
//                "tokenName": "0"
//            }
//        ]
//    }
//],
//"errorMsg": "",
//"status": "ok"
//}


#endif // DAPNODE_H
