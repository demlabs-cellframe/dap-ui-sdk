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

void orderListFiltr(const QJsonArray& in, QJsonArray& out, QStringList keys);

class NodeInfo
{
public:
    NodeInfo() {}
    QString address;
    uint16_t port;
    QString ipv4;
    void setNodeAddress(const QString& a_address) { address = a_address; }
    bool serverDataFromList(const QList<QMap<QString, QString>>& nodeDump);
};


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
    QState transactionProcessing;
    QState gettingOrderList;
    ///
    QState initialState;
    QState nodeDetection;
    QState nodeNotDetected;
    QState nodeConnection;
    QState nodeNotConnected;
    QState getWallets;
    QState getNetworks;
    QState getDataWallet;
    QState getFee;
    QState condTxCreate;
    QState mempoolTxHashRequest;
    QState mempoolTxHashEmpty;
    QState ledgerTxHashRequest;
    QState ledgerTxHashEmpty;
    QState createCertificate;
    QState checkTransactionCertificate;
    QState createTransactionCertificate;
    QState getListKeys;
    QState getOrderList;
    QState getNetId;
    QState getNodeConnectionData;
private:
    QStateMachine commandState;
    QStateMachine nodeConnectMachine;
    void init(){
        commandState.addState(&waitingCommand);
        commandState.addState(&gettingWalletsData);
        commandState.addState(&transactionProcessing);
        commandState.addState(&gettingOrderList);
        commandState.setInitialState(&waitingCommand);
        //
        nodeConnectMachine.addState(&initialState);
        nodeConnectMachine.addState(&nodeDetection);
        nodeConnectMachine.addState(&nodeNotDetected);
        nodeConnectMachine.addState(&nodeConnection);
        nodeConnectMachine.addState(&nodeNotConnected);
        nodeConnectMachine.addState(&getWallets);
        nodeConnectMachine.addState(&getNetworks);
        nodeConnectMachine.addState(&getDataWallet);
        nodeConnectMachine.addState(&condTxCreate);
        nodeConnectMachine.addState(&getFee);
        nodeConnectMachine.addState(&mempoolTxHashRequest);
        nodeConnectMachine.addState(&mempoolTxHashEmpty);
        nodeConnectMachine.addState(&ledgerTxHashRequest);
        nodeConnectMachine.addState(&ledgerTxHashEmpty);
        nodeConnectMachine.addState(&createCertificate);
        nodeConnectMachine.addState(&checkTransactionCertificate);
        nodeConnectMachine.addState(&createTransactionCertificate);
        nodeConnectMachine.addState(&getListKeys);
        nodeConnectMachine.addState(&getOrderList);
        nodeConnectMachine.addState(&getNetId);
        nodeConnectMachine.addState(&getNodeConnectionData);
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
    // orders request
    QString m_minPrice;
    QString m_maxPrice;
    QString m_srvUid;
    QString m_fee;
    NodeInfo m_nodeInfo;
    QStringList m_listKeys;
    QString m_netId;

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
    QString txCondHash();
    static QString certificateName(const QString& access = "public");

private:
    void initStmTransitions();
    void initStmStates();
    void initWeb3Connections();
    void initCommandsStm();

    bool nodeDetected;
    // transaction certificate name


public slots:
    void startCheckingNodeRequest();
    void stopCheckingNodeRequest();
    void slotCondTxCreateRequest(QString walletName, QString networkName, QString tokenName, QString value, QString unit);
    void slotGetOrdersList(QString networkName, QString tokenName, QString minPrice, QString maxPrice, QString unit);
    void slotNodeIpReqest(QString srvUid, QString nodeAddress);

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
    void sigOrderListReady(QJsonArray);
    void sigMempoolContainHash();
    void sigLedgerContainHash();
    void sigCondTxCreateSuccess(QString hash);
    void sigConnectByOrder(QString netId, QString txCondHash, QString token, QString srvUid, QString nodeIp, uint16_t port);

    // ------- internal signals --------
    void waitingCommand();
    void transactionProcessing();
    void gettingWalletsData();
    void sigGettingOrderList();
    //
    void uiStartNodeDetection();
    void errorDetected();
    void repeatNodeDetection();
    void repeatNodeConnection();
    void repeatReadMempool();
    void repeatReadLedger();
    void walletsReceived();
    void sigOrderListReceived();
    void networksReceived();
    void sigCondTxCreateRequest();
    void sigListKeysReceived();
    void sigGetOrderListRequest();
    void walletListIsEmpty();
    void checkCertificate();
    void createCertificate();
    void certificateExist();
    void certificateNotFound();
    void sigNodeIpRequest();
    void sigNodeDumpReceived();
    void sigFeeReceived();
    void sigNetIdReceived();


};

#endif // DAPNODE_H
