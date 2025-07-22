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
#include "DapBaseDataLocal.h"

#include "dap_client_http.h"
#include "DapNetworkAccessManager.h"
#include "DapNetworkReply.h"

#include "DapCmdNode.h"
#include "DapNodeWeb3.h"

#include <QStateMachine>
#include <QState>
#include <QTimer>

void orderListFiltr(const QJsonArray& in, QJsonArray& out, QStringList keys);

class NodeInfo
{
public:
    NodeInfo() {}
    QString address;
    uint16_t port;
    QString ipv4;
    void setNodeAddress(const QString& a_address) { address = a_address; }
    bool serverDataFromList(const QList<QMap<QString, QString>>& nodeList);
};

/****************************************//**
 * @brief state machine class
 * @date 23.10.2022
 * @authors Stanislav Ratseburzhinskii & Mikhail Shilenko
 *******************************************/

class NodeConnectStateMachine : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name VARIABLES
   *******************************************/
  /// @{
public:
  //
  QState waitingCommand;
  QState gettingWalletsData;
  QState transactionProcessing;
  QState gettingOrderList;
  //
  QState initialState;
  QState nodeDetection;
  QState nodeNotDetected;
  QState nodeConnection;
  QState nodeNotConnected;
  QState getWallets;
  QState getNetworks;
  QState getDataWallet;
  QState getFee;
  QState getFeeIsolated;
  QState getNetId;
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
  QState getNodeConnectionData;

  /// state which waits for correct id and returns to last state (stateToReturn)
  QState waitingForCorrectId;
  /// state which tried to get correct id, but got no answer from dashboard
  QState waitingAndReconnecting;

private:
  QStateMachine commandState;
  QStateMachine nodeConnectMachine;

  /// pointer to a state to return after correct id is received
  QState *stateToReturn;

  /// transition from waitingForCorrectId to returning state
  QSignalTransition *returningStateTransition;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit NodeConnectStateMachine();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  void start();

private:
  void init();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigGotIncorrectId();
  void sigConnectionIdReceived();
  void sigErrorDetected();
  void sigNodeNotConnected();
  void sigRestartWaiting();
  /// @}
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
    // key path for tx create
    QString m_keyPath;
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
    QString m_netId;
    NodeInfo m_nodeInfo;
    QStringList m_listKeys;
    bool m_isCDBLogined = false;

public:
    static const int DEFAULT_REQUEST_TIMEOUT = 10000; // 10 sec
    static const QString WEB3_URL;
    static const quint16 WEB3_PORT;
    static const qint32 NODE_DETECT_REQUEST_REPEAT_PERIOD;
    static const qint32 NODE_CONNECT_REQUEST_REPEAT_PERIOD;
    static const qint32 LEDGER_REQUEST_REPEAT_PERIOD;

    DapNode(QObject * obj = Q_NULLPTR, int requestTimeout = DEFAULT_REQUEST_TIMEOUT);
    ~DapNode();
    static DapNode *instance();
    void start();
    QString txCondHash();
    QString netId();
    static QString certificateName(const QString& access = "public");

    void setCDBLogined(bool a_logined){
        m_isCDBLogined = a_logined;
    }

    // Web3 connection ID management
    void handleDashboardRestart();
    QString getWeb3ConnectionId();
    void clearWeb3ConnectionId();

private:
    void initStmTransitions();
    void initStmStates();
    void initWeb3Connections();
    void initCommandsStm();
    void initStmStorageConnections();

    bool nodeDetected = false;
    // transaction certificate name
    
    // Timeouts for detaching from hanging on order list retrieval
    QTimer* m_orderListTimeout = nullptr;
    QTimer* m_listKeysTimeout = nullptr;
    
    // Timeouts for detaching from hanging on fee retrieval
    QTimer* m_feeTimeout = nullptr;
    QTimer* m_feeIsolatedTimeout = nullptr;


public slots:
    void startCheckingNodeRequest();
    void stopCheckingNodeRequest();
    void slotCondTxCreateRequest(QString walletName, QString networkName, QString tokenName, QString value, QString unit, QString keyPath);
    void slotGetOrdersList(QString networkName, QString tokenName, QString minPrice, QString maxPrice, QString unit);
    void slotNodeIpReqest(const QString & srvUid, const QString & nodeAddress, const QString & orderHash, const QString & network);
    void slotGetNodeIpForOrderListReqest(QString srvUid, QJsonArray orderList);
    void slotGetNetIdReqest(QString networkName);
    void slotWalletsRequest();
    void slotFeeRequest (QString a_networkName);

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
    void sigSendNodeIp(QJsonObject);
    void sigMempoolContainHash();
    void sigLedgerContainHash();
    void sigCondTxCreateSuccess(QString hash);
    void sigTransactionInQueue(QString idQueue);
    void sigConnectByOrder(const QString &networkName, const QString &txCondHash, const QString &token, const QString &srvUid, const QString &address, const uint16_t &port);
    void sigRepeatNodeConnecting();

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
    void sigNetIdReceived();
    void sigGetOrderListRequest();
    void walletListIsEmpty();
    void checkCertificate();
    void createCertificate();
    void certificateExist();
    void certificateNotFound();
    void sigNodeIpRequest();
    void sigGetNodeIpRequest(QJsonArray orderList);
    void sigFeeRequest();
    void sigNodeListReceived();
    void sigFeeReceived();
    void sigFeeReceivedData(QJsonObject);
    void sigWalletsRequest();

};

/*-----------------------------------------*/
#endif // DAPNODE_H
