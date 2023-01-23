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
#include "DapNodeWeb3.h"

#include <QStateMachine>
#include <QState>

class NodeConnectStateMachine
{
public:
    explicit NodeConnectStateMachine()
    { init(); }
    void start() {
        nodeConnectMachine.start();
        commandState.start();
    }
    ///
    QState waitingCommand;
    QState gettingWalletsData;
    QState transactionProcessingt;
    ///
    QState initialState;
    QState nodeDetection;
    QState nodeNotDetected;
    QState nodeConnection;
    QState nodeNotConnected;
    QState nodeGetStatus;
    QState getWallets;
    QState getNetworks;
    QState getDataWallet;
    QState condTxCreate;
    QState mempoolTxHashRequest;
    QState mempoolTxHashEmpty;
    QState ledgerTxHashRequest;
    QState ledgerTxHashEmpty;
    QState createCertificate;
    QState checkTransactionCertificate;
    QState createTransactionCertificate;
private:
    QStateMachine commandState;
    QStateMachine nodeConnectMachine;
    void init(){
        commandState.addState(&waitingCommand);
        commandState.addState(&gettingWalletsData);
        commandState.addState(&transactionProcessingt);
        commandState.setInitialState(&waitingCommand);
        //
        nodeConnectMachine.addState(&initialState);
        nodeConnectMachine.addState(&nodeDetection);
        nodeConnectMachine.addState(&nodeNotDetected);
        nodeConnectMachine.addState(&nodeConnection);
        nodeConnectMachine.addState(&nodeNotConnected);
        nodeConnectMachine.addState(&nodeGetStatus);
        nodeConnectMachine.addState(&getWallets);
        nodeConnectMachine.addState(&getNetworks);
        nodeConnectMachine.addState(&getDataWallet);
        nodeConnectMachine.addState(&condTxCreate);
        nodeConnectMachine.addState(&mempoolTxHashRequest);
        nodeConnectMachine.addState(&mempoolTxHashEmpty);
        nodeConnectMachine.addState(&ledgerTxHashRequest);
        nodeConnectMachine.addState(&ledgerTxHashEmpty);
        nodeConnectMachine.addState(&createCertificate);
        nodeConnectMachine.addState(&checkTransactionCertificate);
        nodeConnectMachine.addState(&createTransactionCertificate);
        nodeConnectMachine.setInitialState(&initialState);
        qDebug() << "nodeConnectMachine::init";
    }
};

class DapNode : public QObject
{
    Q_OBJECT
private:
//    const int m_requestTimeout;
    DapNodeWeb3* web3;
    // cellframe dashboard connect id
    // user selected data
    // transaction network name
    QString m_networkName;
    // transaction token name
    QString m_tokenName;
    // transaction wallet name
    QString m_walletName;
    // transaction value
    QString m_value;
    // transaction unit
    QString m_unit;
    // transaction hash
    QString m_transactionHash;
    // wallets list
    QStringList m_walletsList;
    // network list
    QStringList m_networksList;
    // state machine
    NodeConnectStateMachine *m_stm;
    //wallets data
    QJsonObject m_walletsData;

public:
    static const int DEFAULT_REQUEST_TIMEOUT = 10000; // 10 sec
    static const QString WEB3_URL;
    static const quint16 WEB3_PORT;
    static const qint32 NODE_DETECT_REQUEST_REPEAT_PERIOD;
    static const qint32 NODE_CONNECT_REQUEST_REPEAT_PERIOD;
    static const qint32 LEDGER_REQUEST_REPEAT_PERIOD;

    DapNode(QObject * obj = Q_NULLPTR, int requestTimeout = DEFAULT_REQUEST_TIMEOUT);
    ~DapNode();
    void start();

private:
    void initStmTransitions();
    void initStmStates();
    void initWeb3Connections();
    void initCommandsStm();

    bool nodeDetected;
    // transaction certificate name
    QString certificateName(const QString& access = "public") const;


public slots:
    void startCheckingNodeRequest();
    void stopCheckingNodeRequest();
    void slotCondTxCreateRequest(QString walletName, QString networkName, QString tokenName, QString value, QString unit);

private slots:
    void walletDataRequest();
    void parseDataWallet(const QJsonArray walletData);

signals:
    // -------- output signals --------
    void sigError(int errorCode, QString errorMessage);
    void sigReceivedWalletsList(QStringList);
    void sigReceivedNetworksList(QStringList);
    void sigNodeDetected();
    void sigWalletsDataReady(QJsonObject);
    void sigMempoolContainHash();
    void sigLedgerContainHash();
    void sigCondTxCreateSuccess();
    // ------- internal signals --------
    void waitingCommand();
    void transactionProcessing();
    void gettingWalletsData();
    //
    void uiStartNodeDetection();
    void errorDetected();
    void repeatNodeDetection();
    void repeatNodeConnection();
    void repeatReadMempool();
    void repeatReadLedger();
    void walletsReceived();
    void networksReceived();
    void sigCondTxCreateRequest();
    void walletListIsEmpty();
    void checkCertificate();
    void createCertificate();
    void certificateExist();
    void certificateNotFound();

};

#endif // DAPNODE_H
