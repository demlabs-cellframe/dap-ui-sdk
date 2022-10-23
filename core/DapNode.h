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
    QState checkFound;
    QState nodeNotFound;
    QState connectToNode;
    QState nodeGetWallets;
    QState nodeGetDataWallet;
    QState nodeCondTxCreate;
private:
    QStateMachine nodeConnectMachine;
    void init(){
        nodeConnectMachine.addState(&initialState);
        nodeConnectMachine.addState(&checkFound);
        nodeConnectMachine.addState(&nodeNotFound);
        nodeConnectMachine.addState(&connectToNode);
        nodeConnectMachine.addState(&nodeGetWallets);
        nodeConnectMachine.addState(&nodeGetDataWallet);
        nodeConnectMachine.addState(&nodeCondTxCreate);
        nodeConnectMachine.setInitialState(&initialState);
    }
};

enum class DapNodeErrors
{
    NetworkReplyFinishedConnectError,
    NetworkReplyConnectError,
    NetworkReplyFinishedWalletsError,
    NetworkReplyWalletsError,
    NetworkWrongReplyError,
    TimerElapsed,
    JsonError,
    StatusError
};

int DapNodeErrorCode(DapNodeErrors);

class DapNode : public QObject
{
    Q_OBJECT
private:
    static const int DEFAULT_REQUEST_TIMEOUT = 10000; // 10 sec
    const int m_requestTimeout;
    DapNetworkAccessManager * m_httpClient;
    // cellframe dashboard connect id
    QString m_connectId;
    // wallets list
    QStringList m_walletsList;
    //
    DapNetworkReply *m_networkReply;
    QString m_networkRequest;
    //
    bool m_parseJsonError;
    // state machine
    NodeConnectStateMachine *m_stateMachine;

public:
    static const QString WEB3_URL;
    static const quint16 WEB3_PORT;

    DapNode(QObject * obj = Q_NULLPTR, int requestTimeout = DEFAULT_REQUEST_TIMEOUT);
    ~DapNode();

private:
    void request_GET(const QString& host,  quint16 port,
                     const QString & urlPath, DapNetworkReply &a_netReply,
                     const QString &headers = "", bool ssl = false);
    bool jsonError() { return m_parseJsonError = true; }
    void initStates();
    void responseProcessing(const int error, const QString errorString = "");


public slots:
    void startCheckingNodeRequest();
    void stopCheckingNodeRequest();

protected:
    DapNetworkReply *m_cellframeDashboardTestReply;
    DapNetworkReply *m_connectReply;
    DapNetworkReply *m_walletsListReply;

private slots:
    void sendRequest(QString request);
    void cellframeDashboardTest();
    void cellframeDashboardReply();
    void connectRequest();
    void connectReply();
    void walletsRequest();
    void walletsListReply();
    void replyError(DapNodeErrors errorType, const QString errorString = QString(""));
    void parseReplyConnect(const QString replyData);
    void parseReplyWallets(const QString replyData);
    void parseJsonError(QString replyData);
signals:
    // -------- output signals --------
    void sigReceivedWalletsList(QStringList);
    void sigError(int errorCode, QString errorMessage);
    // --- external (input) signals ---
    void startCheckingNode();
    void stopCheckingNode();
    void dataWalletRequest();
    void condTxCreateRequest(QString tokenName, QString walletName, QString certName, qreal value);
    // ------- internal signals --------
    void nodeFound();
    void nodeConnected();


};

#endif // DAPNODE_H
