#include "DapNode.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include "DapBaseDataLocal.h"
#include <QTimer>
#include <QSignalTransition>
#include <QDebug>


//#define SIGERROR (QNetworkReply::NetworkError::UnknownServerError + 1)
#define SIGERROR (110)
#define DEBUGINFO qDebug()<<"--->SrvNode<---"

const QString  DapNode::WEB3_URL      ("localhost");
const quint16  DapNode::WEB3_PORT     (8045);
const qint32   DapNode::NODE_DETECT_REQUEST_REPEAT_PERIOD   (10000);
const qint32   DapNode::NODE_CONNECT_REQUEST_REPEAT_PERIOD  (500);
const qint32   DapNode::LEDGER_REQUEST_REPEAT_PERIOD        (5000);


DapNode:: DapNode(QObject * obj, int requestTimeout) :
    QObject(obj),
//    m_requestTimeout(requestTimeout),
    m_stm(new NodeConnectStateMachine)
{
    web3 = new DapNodeWeb3();
    
    // Initialize timeouts for detaching from hanging on order list retrieval
    m_orderListTimeout = new QTimer(this);
    m_orderListTimeout->setSingleShot(true);
    m_orderListTimeout->setInterval(15000); // 15 seconds
    connect(m_orderListTimeout, &QTimer::timeout, this, [this]() {
        qDebug() << "[Order List Detach] Order list request timeout, returning to initial state";
        emit errorDetected();
    });
    
    m_listKeysTimeout = new QTimer(this);
    m_listKeysTimeout->setSingleShot(true);
    m_listKeysTimeout->setInterval(10000); // 10 seconds
    connect(m_listKeysTimeout, &QTimer::timeout, this, [this]() {
        qDebug() << "[Order List Detach] List keys request timeout, proceeding to order list";
        emit sigListKeysReceived();
    });
    
    // Initialize timeouts for detaching from hanging on fee retrieval
    m_feeTimeout = new QTimer(this);
    m_feeTimeout->setSingleShot(true);
    m_feeTimeout->setInterval(12000); // 12 seconds
    connect(m_feeTimeout, &QTimer::timeout, this, [this]() {
        qDebug() << "[Fee Debug] Fee request timeout, proceeding to next state";
        emit sigFeeReceived();
    });
    
    m_feeIsolatedTimeout = new QTimer(this);
    m_feeIsolatedTimeout->setSingleShot(true);
    m_feeIsolatedTimeout->setInterval(12000); // 12 seconds
    connect(m_feeIsolatedTimeout, &QTimer::timeout, this, [this]() {
        qDebug() << "[Fee Debug] Fee isolated request timeout, returning to initial state";
        emit sigFeeReceived();
    });

    // Periodic retry for node detection when node is not detected
    m_nodeDetectRetryTimer = new QTimer(this);
    m_nodeDetectRetryTimer->setSingleShot(false);
    m_nodeDetectRetryTimer->setInterval(NODE_DETECT_REQUEST_REPEAT_PERIOD);
    connect(m_nodeDetectRetryTimer, &QTimer::timeout, this, [this]() {
        if (m_stm->nodeNotDetected.active()) {
            emit repeatNodeDetection();
        } else {
            m_nodeDetectRetryTimer->stop();
        }
    });
    
    initWeb3Connections();
    initStmTransitions();
    initStmStates();
    initCommandsStm();
    initStmStorageConnections();
}

DapNode::~ DapNode()
{
}

static DapNode *s_instance   = nullptr;

DapNode *DapNode::instance()
{
  return s_instance;
}

// transaction certificate name
QString DapNode::certificateName(const QString& access)
{
    const QString private_certificateName("node-addr-kelvpn-minkowski-cert");
    const QString public_certificateName = private_certificateName + "_public";
    if (access == "private")
        return private_certificateName;
    // if (access == "public")
    return public_certificateName;
}

void DapNode::stopCheckingNodeRequest()
{

}


void DapNode::startCheckingNodeRequest()
{
    DEBUGINFO << __PRETTY_FUNCTION__;

    if (m_isCDBLogined) {
        qDebug() << __PRETTY_FUNCTION__ << " already using CDB mode";
        return;
    }

    if (m_stm->initialState.active()) {
        DEBUGINFO << __PRETTY_FUNCTION__ << " uiStartNodeDetection()";
        emit uiStartNodeDetection();
        return;
    }

    if (m_stm->nodeConnection.active()) {
        DEBUGINFO << __PRETTY_FUNCTION__ << " emit sigRepeatNodeDetecting after GUI restart";
        emit sigNodeDetected();
        emit sigRepeatNodeConnecting();
        return;
    }

    if (m_stm->ledgerTxHashRequest.active() || m_stm->ledgerTxHashEmpty.active()) {
        DEBUGINFO << __PRETTY_FUNCTION__ << " sigMempoolContainHash()";
        emit sigMempoolContainHash();
        return;
    }

    if (nodeDetected) {
        DEBUGINFO << __PRETTY_FUNCTION__ << " sigNodeDetected()";
        emit sigNodeDetected();
        return;
    }
}


void DapNode::initCommandsStm()
{
    m_stm->waitingCommand.addTransition(this, &DapNode::uiStartNodeDetection,
    &m_stm->gettingWalletsData);
    m_stm->waitingCommand.addTransition(this, &DapNode::sigCondTxCreateRequest,
    &m_stm->transactionProcessing);
    m_stm->waitingCommand.addTransition(this, &DapNode::sigGetOrderListRequest,
    &m_stm->gettingOrderList);

    m_stm->gettingWalletsData.addTransition(this, &DapNode::waitingCommand,
    &m_stm->waitingCommand);
    m_stm->transactionProcessing.addTransition(this, &DapNode::waitingCommand,
    &m_stm->waitingCommand);
    m_stm->gettingOrderList.addTransition(this, &DapNode::waitingCommand,
    &m_stm->waitingCommand);

}

void DapNode::initStmStorageConnections()
{
  connect (web3, &DapNodeWeb3::sigIncorrectId,
           m_stm, &NodeConnectStateMachine::sigGotIncorrectId);
  connect (web3, &DapNodeWeb3::connectionIdReceived,
           m_stm, &NodeConnectStateMachine::sigConnectionIdReceived);
  connect (web3, &DapNodeWeb3::nodeNotConnected,
           m_stm, &NodeConnectStateMachine::sigNodeNotConnected);
  connect (this, &DapNode::sigError,
           m_stm, &NodeConnectStateMachine::sigErrorDetected);
  connect (m_stm, &NodeConnectStateMachine::sigRestartWaiting,
           this, [this]
  {
    web3->nodeConnectionRequest();
  });
}

void DapNode::initStmTransitions()
{
    DEBUGINFO << "initStates";
    // node search  initialState -> startCheckingNode
    m_stm->initialState.addTransition(this, &DapNode::uiStartNodeDetection,
    &m_stm->nodeDetection);
    m_stm->initialState.addTransition(this, &DapNode::sigCondTxCreateRequest,
    &m_stm->getFee);
    m_stm->initialState.addTransition(this, &DapNode::sigFeeRequest,
    &m_stm->getFeeIsolated);
    //&m_stm->nodeGetStatus);
    m_stm->initialState.addTransition(this, &DapNode::sigGetOrderListRequest,
    &m_stm->getListKeys);
    //&m_stm->nodeGetStatus);
    m_stm->initialState.addTransition(this, &DapNode::sigNodeIpRequest,
    &m_stm->getNodeConnectionData);
    m_stm->initialState.addTransition(this, &DapNode::sigWalletsRequest,
    &m_stm->getWallets);

    // node detection -> node detected
    m_stm->nodeDetection.addTransition(web3, &DapNodeWeb3::nodeDetected,
    &m_stm->nodeConnection);
    // node detection -> node detected
    m_stm->nodeDetection.addTransition(web3, &DapNodeWeb3::statusOk,
    &m_stm->getWallets);
    // node detection -> get node status
    m_stm->nodeDetection.addTransition(web3, &DapNodeWeb3::nodeNotDetected,
    &m_stm->nodeNotDetected);

    // node detection -> node not detected on generic web3 error
    // (e.g. invalid/missing status, unknown method, connection refused)
    m_stm->nodeDetection.addTransition(this, &DapNode::errorDetected,
    &m_stm->nodeNotDetected);

    // node not detected -> node detection, repeat detection
    m_stm->nodeNotDetected.addTransition(this, &DapNode::repeatNodeDetection,
    &m_stm->nodeDetection);

    // node connection -> node connected, wallets list request
    m_stm->nodeConnection.addTransition(web3, &DapNodeWeb3::connectionIdReceived,
    //&m_stm->nodeGetStatus);
    &m_stm->getWallets);
    // node connection -> node not connected, repeat connection
    m_stm->nodeConnection.addTransition(web3, &DapNodeWeb3::nodeNotConnected,
    &m_stm->nodeNotConnected);

    // node not connected -> node connection, repeat connection
    m_stm->nodeNotConnected.addTransition(this, &DapNode::repeatNodeConnection,
    &m_stm->nodeConnection);

    // fee received -> condition transaction create request
    m_stm->getFee.addTransition(this, &DapNode::sigFeeReceived,
    &m_stm->getNetId);

    // fee received (isolated) -> send result
    m_stm->getFeeIsolated.addTransition(this, &DapNode::sigFeeReceived,
    &m_stm->initialState);
    m_stm->getFeeIsolated.addTransition(this, &DapNode::errorDetected,
    &m_stm->initialState);

    // fee received -> condition transaction create request
    m_stm->getNetId.addTransition(this, &DapNode::sigNetIdReceived,
    &m_stm->checkTransactionCertificate);

    // node status !ok -> nodeGetWallets
//    m_stm->nodeGetStatus.addTransition(this, &DapNode::errorDetected,
//    &m_stm->initialState);

    // node status ok -> getWallets
//    m_stm->nodeGetStatus.addTransition(this, &DapNode::gettingWalletsData,
//    &m_stm->getWallets);
//    m_stm->nodeGetStatus.addTransition(web3, &DapNodeWeb3::statusOk,
//    &m_stm->getWallets);

//    m_stm->nodeGetStatus.addTransition(this, &DapNode::transactionProcessing,
//    &m_stm->checkTransactionCertificate);

    // get wallets -> wallets received
    m_stm->getWallets.addTransition(this, &DapNode::walletsReceived,
    &m_stm->getNetworks);

    // get wallets -> init state
    m_stm->getWallets.addTransition(this, &DapNode::errorDetected,
    &m_stm->initialState);

    // get wallets -> node connection
    m_stm->getWallets.addTransition(web3, &DapNodeWeb3::sigIncorrectId,
    &m_stm->nodeConnection);

    // get networks -> networks received
    m_stm->getNetworks.addTransition(this, &DapNode::networksReceived,
    &m_stm->getDataWallet);

    // get networks -> init state
    m_stm->getNetworks.addTransition(this, &DapNode::errorDetected,
    &m_stm->initialState);

    //
    m_stm->getDataWallet.addTransition(this, &DapNode::walletListIsEmpty,
    &m_stm->initialState);

    //
    m_stm->getDataWallet.addTransition(this, &DapNode::errorDetected,
    &m_stm->initialState);

    //
    /***************************************
    * transaction workflow
    ***************************************/
    // Transition to conditional transaction create
//    m_stm->initialState.addTransition(this, &DapNode::sigCondTxCreateRequest,
//    &m_stm->checkTransactionCertificate);
    // Transition to check certificate
    m_stm->checkTransactionCertificate.addTransition(this, &DapNode::certificateExist,
    &m_stm->condTxCreate);
    // Transition to create certificate
    m_stm->checkTransactionCertificate.addTransition(this, &DapNode::certificateNotFound,
    &m_stm->createTransactionCertificate);
    // Transition to conditional transaction create
    m_stm->createTransactionCertificate.addTransition(this, &DapNode::certificateExist,
    &m_stm->condTxCreate);
    // Return to initial state
    m_stm->createTransactionCertificate.addTransition(this, &DapNode::errorDetected,
    &m_stm->initialState);
    // to check mempool if create successfuly
    m_stm->condTxCreate.addTransition(this, &DapNode::sigCondTxCreateSuccess,
    &m_stm->mempoolTxHashRequest);
    // to mempool empty
    m_stm->mempoolTxHashRequest.addTransition(this, &DapNode::errorDetected,
    &m_stm->mempoolTxHashEmpty);
    // to check ledger if hash in mempool
    m_stm->mempoolTxHashRequest.addTransition(this, &DapNode::sigMempoolContainHash,
    &m_stm->initialState);
//    &m_stm->ledgerTxHashRequest);
    // to check mempool
    m_stm->mempoolTxHashEmpty.addTransition(this, &DapNode::repeatReadMempool,
    &m_stm->mempoolTxHashRequest);
    // to ledger empty
    m_stm->ledgerTxHashRequest.addTransition(this, &DapNode::errorDetected,
    &m_stm->ledgerTxHashEmpty);
    // to init state
    m_stm->ledgerTxHashRequest.addTransition(web3, &DapNodeWeb3::sigLedgerContainHash,
    &m_stm->initialState);
    // to init state if error
    m_stm->ledgerTxHashEmpty.addTransition(this, &DapNode::repeatReadLedger,
    &m_stm->ledgerTxHashRequest);
    // to create certificate
    m_stm->initialState.addTransition(this, &DapNode::sigCondTxCreateRequest,
    &m_stm->condTxCreate);

    //
    // getting order list
    // node status ok -> getWallets
//    m_stm->nodeGetStatus.addTransition(this, &DapNode::sigGettingOrderList,
//    &m_stm->getOrderList);
    m_stm->getListKeys.addTransition(this, &DapNode::sigListKeysReceived,
    &m_stm->getOrderList);
    m_stm->getListKeys.addTransition(this, &DapNode::errorDetected,
    &m_stm->getOrderList);

    // getOrderList -> initialState
    m_stm->getOrderList.addTransition(this, &DapNode::sigOrderListReceived,
    &m_stm->initialState);
    m_stm->getOrderList.addTransition(this, &DapNode::errorDetected,
    &m_stm->initialState);

    // getting node ip
    m_stm->getNodeConnectionData.addTransition(this, &DapNode::sigNodeListReceived,
    &m_stm->initialState);
    m_stm->getNodeConnectionData.addTransition(this, &DapNode::errorDetected,
    &m_stm->initialState);

}

void DapNode::initStmStates()
{
    // initial state
    connect(&m_stm->initialState, &QState::entered, this, [=](){
        DEBUGINFO << "&initialState, &QState::entered";
        qDebug() << "[Order List Debug] Entered initialState - ready for new requests";
        emit waitingCommand();
        // autostart node detection
        // startCheckingNodeRequest();
    });
    // node detection
    connect(&m_stm->nodeDetection, &QState::entered, this, [=](){
        DEBUGINFO  << "&nodeDetection, &QState::entered";
    });
    // node detected
    connect(&m_stm->nodeNotDetected, &QState::entered, this, [=](){
        DEBUGINFO  << "&nodeNotDetected, &QState::entered";
        if (!m_isCDBLogined && m_nodeDetectRetryTimer && !m_nodeDetectRetryTimer->isActive()) {
            m_nodeDetectRetryTimer->start();
        }
    });
    connect(&m_stm->nodeNotDetected, &QState::exited, this, [=](){
        if (m_nodeDetectRetryTimer && m_nodeDetectRetryTimer->isActive()) {
            m_nodeDetectRetryTimer->stop();
        }
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
    connect(&m_stm->nodeDetection,  &QState::entered, web3, &DapNodeWeb3::nodeDetectedRequest);
    // connection request
    connect(&m_stm->nodeConnection, &QState::entered, this, [=](){
        if (!m_isCDBLogined)
            web3->nodeConnectionRequest();
    }); //web3, &DapNodeWeb3::nodeConnectionRequest);

    connect(this,  &DapNode::sigRepeatNodeConnecting, web3, &DapNodeWeb3::nodeConnectionRequest);


    // get node status
    //connect(&m_stm->nodeGetStatus,  &QState::entered, web3, &DapNodeWeb3::nodeStatusRequest);
    // get wallets
    connect(&m_stm->getWallets, &QState::entered, web3, &DapNodeWeb3::walletsRequest);
    // get networks
    connect(&m_stm->getNetworks, &QState::entered, web3, &DapNodeWeb3::networksRequest);
    // get wallets data
    connect (&m_stm->getDataWallet, &QState::entered, this, [=](){
        m_walletsData = QJsonObject();
        walletDataRequest();
    });
    
    // connect app type detection signal from web3
    connect(web3, &DapNodeWeb3::statusOk, this, [this](){
        QString detectedAppType = web3->connectedAppType();
        if (!detectedAppType.isEmpty()) {
            emit sigConnectedAppTypeDetected(detectedAppType);
        }
    });
    
    // get fee
    connect (&m_stm->getFee, &QState::entered, this, [=](){
        qDebug() << "[Fee Debug] Entering getFee state, starting timeout for network:" << m_networkName;
        m_feeTimeout->start();
        web3->getFeeRequest(m_networkName);
    });
    
    connect (&m_stm->getFee, &QState::exited, this, [=](){
        qDebug() << "[Fee Debug] Exiting getFee state, stopping timeout";
        m_feeTimeout->stop();
    });
    
    // get fee isolated
    connect (&m_stm->getFeeIsolated, &QState::entered, this, [=](){
        qDebug() << "[Fee Debug] Entering getFeeIsolated state, starting timeout for network:" << m_networkName;
        m_feeIsolatedTimeout->start();
        web3->getFeeRequest (m_networkName);
    });
    
    connect (&m_stm->getFeeIsolated, &QState::exited, this, [=](){
        qDebug() << "[Fee Debug] Exiting getFeeIsolated state, stopping timeout";
        m_feeIsolatedTimeout->stop();
    });

    connect (&m_stm->getNetId, &QState::entered, this, [=](){
        web3->getNetIdRequest(m_networkName); //web3->DapNodeWeb3::getNetIdRequest(netId);
    });

    // check certificate
    connect (&m_stm->checkTransactionCertificate, &QState::entered, web3, &DapNodeWeb3::getCertificates);
    // certificate create
    connect (&m_stm->createTransactionCertificate, &QState::entered, this, [=](){
        // only private certificates can be created
        web3->createCertificate("sig_dil", certificateName("private"));
    });
    // create conditional transaction
    connect(&m_stm->condTxCreate, &QState::entered, this, [=](){
            web3->DapNodeWeb3::condTxCreateRequest(
                        m_walletName,
                        m_networkName,
//                        certificateName("public"),
                        m_keyPath,
                        m_tokenName,
                        m_value,
                        m_unit);
    });
    // mempool check
    connect(&m_stm->mempoolTxHashRequest, &QState::entered, this, [=](){
        QTimer::singleShot(2000, [&]{
            web3->getMempoolTxHashRequest(m_transactionHash, m_networkName);
        });
    });
    connect(&m_stm->mempoolTxHashEmpty, &QState::entered, this, [=](){
        DEBUGINFO  << "&mempoolTxHashEmpty, &QState::entered";
        QTimer::singleShot(LEDGER_REQUEST_REPEAT_PERIOD, this, [=](){
            DEBUGINFO  << "&singleShotledgerTxHashEmpty";
            emit repeatReadMempool();
        });
    });
    // ledger check
    connect(&m_stm->ledgerTxHashRequest, &QState::entered, this, [=](){
        web3->getLedgerTxHashRequest(m_transactionHash, m_networkName);
    });
    connect(&m_stm->ledgerTxHashEmpty, &QState::entered, this, [=](){
        DEBUGINFO  << "&ledgerTxHashEmpty, &QState::entered";
        QTimer::singleShot(LEDGER_REQUEST_REPEAT_PERIOD, this, [=](){
            DEBUGINFO  << "&singleShotledgerTxHashEmpty";
            emit repeatReadLedger();
        });
    });

    // get keys
    connect(&m_stm->getListKeys, &QState::entered, this, [=](){
        qDebug() << "[Order List Debug] SUCCESS: Transition initialState → getListKeys worked!";
        qDebug() << "[Order List Detach] Entering getListKeys state, starting timeout";
        m_listKeysTimeout->start();
        web3->getListKeysRequest(m_networkName);
    });
    
    connect(&m_stm->getListKeys, &QState::exited, this, [=](){
        qDebug() << "[Order List Detach] Exiting getListKeys state, stopping timeout";
        m_listKeysTimeout->stop();
    });

    // get orders list
    connect(&m_stm->getOrderList, &QState::entered, this, [=](){
        qDebug() << "[Order List Detach] Entering getOrderList state, starting timeout";
        m_orderListTimeout->start();
        web3->DapNodeWeb3::getOrdersListRequest(
                    m_networkName,
                    m_tokenName,
                    m_minPrice,
                    m_maxPrice,
                    m_unit);
    });
    
    connect(&m_stm->getOrderList, &QState::exited, this, [=](){
        qDebug() << "[Order List Detach] Exiting getOrderList state, stopping timeout";
        m_orderListTimeout->stop();
    });

    connect(&m_stm->getNodeConnectionData, &QState::entered, this, [=](){
        web3->nodeListRequest(m_networkName);
    });
}

void DapNode::initWeb3Connections()
{
    // node detected signal
    connect(web3, &DapNodeWeb3::nodeDetected, this, &DapNode::sigNodeDetected);
    connect(web3, &DapNodeWeb3::checkNodeStatus, this, &DapNode::sigNodeDetected);
    // Consider node as detected once Dashboard approves connection (Connect → connectionIdReceived)
    connect(web3, &DapNodeWeb3::connectionIdReceived, this, [=](const QString &){
        DEBUGINFO << "[Node Detect] connectionIdReceived -> emitting sigNodeDetected()";
        emit sigNodeDetected();
    });
    connect(web3, &DapNodeWeb3::statusOk, this, [=](){
        if (m_stm->transactionProcessing.active()) emit transactionProcessing();
        if (m_stm->gettingWalletsData.active()) emit gettingWalletsData();
        if (m_stm->gettingOrderList.active()) emit sigGettingOrderList();
    });

    // web3 wallets received
    connect(web3, &DapNodeWeb3::sigReceivedWalletsList, this, [=](QStringList wallets){
        m_walletsList = wallets;
        emit walletsReceived();
        emit sigReceivedWalletsList(wallets);
    });
    // web3 networks received
    connect(web3, &DapNodeWeb3::sigReceivedNetworksList, this, [=](QStringList networks){
        emit networksReceived();
        emit sigReceivedNetworksList(networks);
    });
    // web3 error detected
    connect(web3, &DapNodeWeb3::sigError, this, [=](int errorCode, QString errorMessage){
        emit errorDetected();

        /* ignore and do not send "Wrong reply connect" error */
        if (errorCode == 100110
            || errorMessage == "Wrong reply connect"
            || errorMessage == "Wrong reply status")
          return;

        emit sigError(errorCode, errorMessage);
    });

    // web3 connect denied -> show friendly notification to UI layer
    connect(web3, &DapNodeWeb3::sigConnectDenied, this, [=](){
        DEBUGINFO << "[CONNECT DENIED] Forwarding friendly UI notification";
        emit sigError(0, QStringLiteral("Connection denied by wallet"));
    });
    // get wallet data
    connect(web3, &DapNodeWeb3::sigWalletDataReady, this, &DapNode::parseDataWallet);
    // mempool contain transaction hash
    connect(web3, &DapNodeWeb3::sigMempoolContainHash, this, &DapNode::sigMempoolContainHash);
    // ledger contain transaction hash
    connect(web3, &DapNodeWeb3::sigLedgerContainHash, this, &DapNode::sigLedgerContainHash);
    // check m_certificateName certificates list
    connect(web3, &DapNodeWeb3::sigReceivedCertificatestList, this, [=](QStringList certList){
        if (certList.contains(certificateName("public"))) emit certificateExist();
        // Certificate is not found
        else emit certificateNotFound();
    });
    // check m_certificateName after certificate create
    connect(web3, &DapNodeWeb3::sigCreatedCertificate, this, [=](QString certName){
        if (certName == certificateName("public")) emit certificateExist();
    });
    //
    connect(web3, &DapNodeWeb3::sigCondTxCreateSuccess, this, [=](QString hash){
       DEBUGINFO  << "&sigCondTxCreateSuccess" << hash;
       m_transactionHash = hash;
       emit sigCondTxCreateSuccess(m_transactionHash);
    });
    // transaction queued in Dashboard
    connect(web3, &DapNodeWeb3::sigTransactionInQueue, this, [=](QString idQueue, QString appType){
       DEBUGINFO  << "&sigTransactionInQueue" << idQueue << "App type:" << appType;
       emit sigTransactionInQueue(idQueue, appType);
    });
    // list keys ready
    connect(web3, &DapNodeWeb3::sigListKeys, this, [=](QStringList listKeys) {
        qDebug() << "🔍 [FILTER DEBUG] DapNode: Received listKeys count =" << listKeys.size();
        qDebug() << "🔍 [FILTER DEBUG] DapNode: Keys list =" << listKeys;
        qDebug() << "[Order List Detach] List keys received successfully, stopping timeout";
        m_listKeysTimeout->stop();
        m_listKeys = listKeys;
        emit sigListKeysReceived();
    });
    // order list ready
    connect(web3, &DapNodeWeb3::sigOrderList, this, [=](QJsonArray ordersList) {
        qDebug() << "🔍 [FILTER DEBUG] DapNode: Received ordersList count =" << ordersList.size();
        qDebug() << "[Order List Detach] Order list received successfully, stopping timeout";
        m_orderListTimeout->stop();
        QJsonArray orders;
        orderListFiltr(ordersList, orders, m_listKeys);
        qDebug() << "🔍 [FILTER DEBUG] DapNode: After filtering orders count =" << orders.size();
        emit sigOrderListReceived();
        emit sigOrderListReady(orders); //emit sigOrderListReady(orders);
    });

    connect(web3, &DapNodeWeb3::sigNodeIp, this, [=](QJsonObject data) {
        emit sigSendNodeIp(data);
    });

    //connect(web3, &DapNodeWeb3::sigOrderList, this, &DapNode::sigOrderListReady);
    connect (web3, &DapNodeWeb3::sigFeeData,
             this, [=] (const QJsonObject &a_data)
    {
      qDebug() << "[Fee Debug] sigFeeData received, getFeeIsolated active:" << m_stm->getFeeIsolated.active() 
               << "getFee active:" << m_stm->getFee.active();
      
      if (m_stm->getFeeIsolated.active()) {
        qDebug() << "[Fee Debug] Fee data received successfully for getFeeIsolated, stopping timeout";
        m_feeIsolatedTimeout->stop();
        qDebug() << "[Fee Debug] Emitting sigFeeReceivedData for getFeeIsolated";
        emit sigFeeReceivedData (a_data);
        qDebug() << "[Fee Debug] Also emitting sigFeeReceived to trigger state transition";
        emit sigFeeReceived();
      }
      
      if (m_stm->getFee.active()) {
        qDebug() << "[Fee Debug] Fee data received successfully for getFee, stopping timeout";
        m_feeTimeout->stop();
        qDebug() << "[Fee Debug] Emitting sigFeeReceived for getFee";
        emit sigFeeReceived();
      }
    });
    // connect to stream
    connect(web3, &DapNodeWeb3::sigNodeList, this, [=](QList<QMap<QString, QString>> nodeList) {
//         riemann, use dap_chain_net_id_by_name() "0x000000000000dddd"
        m_nodeInfo.serverDataFromList(nodeList);
        emit sigNodeListReceived();
        emit sigConnectByOrder(m_networkName, m_transactionHash, m_tokenName, m_srvUid, m_nodeInfo.ipv4, m_nodeInfo.port);
    });

    connect(this, &DapNode::sigGetNodeIpRequest, this, [=](QJsonArray orderList) {
        web3->DapNodeWeb3::getNodeIPRequest(m_networkName, orderList);
    });

    connect(web3, &DapNodeWeb3::sigNetId, this, [=](QString netId) {
        qDebug() << "NetId received - " + netId;
        m_netId = netId;
        emit sigNetIdReceived();
    });



}

void DapNode::slotCondTxCreateRequest(QString walletName, QString networkName, QString tokenName, QString value, QString unit, QString keyPath)
{
    m_walletName = walletName;
    m_networkName = networkName;
    m_tokenName = tokenName;
    m_value = value;
    m_unit = unit;
    m_keyPath = keyPath;
    emit sigCondTxCreateRequest();
}

void DapNode::slotGetOrdersList(QString networkName, QString tokenName, QString minPrice, QString maxPrice, QString unit)
{
    qDebug() << "[Order List Debug] slotGetOrdersList called for network:" << networkName << "token:" << tokenName;
    qDebug() << "[Order List Debug] Current state - initialState active:" << m_stm->initialState.active();
    qDebug() << "[Order List Debug] Current state - getFeeIsolated active:" << m_stm->getFeeIsolated.active();
    qDebug() << "[Order List Debug] Current state - getListKeys active:" << m_stm->getListKeys.active();
    qDebug() << "[Order List Debug] Current state - getOrderList active:" << m_stm->getOrderList.active();
    
    m_networkName = networkName;
    m_tokenName = tokenName;
    m_minPrice = minPrice;
    m_maxPrice = maxPrice;
    m_unit = unit;
    
    qDebug() << "[Order List Debug] Emitting sigGetOrderListRequest";
    emit sigGetOrderListRequest();
}

void DapNode::walletDataRequest()
{
    DEBUGINFO << "walletDataRequest" << m_walletsList;
    if (m_walletsList.size() > 0)
    {
        web3->walletDataRequest(m_walletsList[0]);
    }
    else
    {
        emit sigWalletsDataReady(m_walletsData);
        emit walletListIsEmpty();
    }
}

void DapNode::parseDataWallet(const QJsonArray walletData)
{
    DEBUGINFO << "walletDataReply" << walletData;
    // add wallet data to wallets list
    m_walletsData[m_walletsList[0]] = walletData;
    m_walletsList.pop_front();
    walletDataRequest();
}

QString DapNode::txCondHash()
{
    return m_transactionHash;
}
QString DapNode::netId()
{
    return m_netId;
}


void DapNode::start()
{
    DEBUGINFO  << "start DapNode";
    m_stm->start();
}

void DapNode::slotNodeIpReqest(const QString & srvUid, const QString & nodeAddress, const QString & orderHash, const QString & network)
{
    Q_UNUSED(orderHash)
    
    m_srvUid = srvUid;
    m_nodeInfo.setNodeAddress(nodeAddress);
    if (!network.isEmpty())
      m_networkName = network;
    emit sigNodeIpRequest();
}
void DapNode::slotGetNodeIpForOrderListReqest(QString srvUid, QJsonArray orderList)
{
    m_srvUid = srvUid;
    emit sigGetNodeIpRequest(orderList);
}

bool NodeInfo::serverDataFromList(const QList<QMap<QString, QString>>& nodeList)
{
    foreach (const auto item, nodeList)
    {
        if (item["node address"] == address)
        {
            bool ok;
            ipv4 = item["ipv4"];
            port = item["port"].toInt(&ok);
            if (ok)
                return true;
            else
                return false;
        }
    }
    return false;
}

void orderListFiltr(const QJsonArray& inOrders, QJsonArray& outOrders, QStringList keys)
{
    qDebug() << "🔍 [FILTER DEBUG] orderListFiltr: Input orders count =" << inOrders.size();
    qDebug() << "🔍 [FILTER DEBUG] orderListFiltr: Available keys count =" << keys.size();
    qDebug() << "🔍 [FILTER DEBUG] orderListFiltr: Keys list =" << keys;
    
    foreach(const QJsonValue& order, inOrders)
    {
        QString orderPkey = order.toObject()["pkey"].toString();
        bool found = false;
        foreach (const QString& key, keys)
        {
            // Normalize keys for comparison - remove leading dot if present
            QString normalizedKey = key.startsWith(".") ? key.mid(1) : key;
            QString normalizedOrderPkey = orderPkey.startsWith(".") ? orderPkey.mid(1) : orderPkey;
            
            if (normalizedKey == normalizedOrderPkey)
            {
                outOrders.append(order);
                found = true;
                qDebug() << "🔍 [FILTER DEBUG] orderListFiltr: Order with pkey" << orderPkey << "FOUND! Matched with key" << key;
                break;
            }
        }
        if (!found) {
            qDebug() << "🔍 [FILTER DEBUG] orderListFiltr: Order with pkey" << orderPkey << "NOT FOUND in keys";
        }
    }
    
    qDebug() << "🔍 [FILTER DEBUG] orderListFiltr: Output orders count =" << outOrders.size();
}

void DapNode::slotGetNetIdReqest(QString networkName)
{
  web3->DapNodeWeb3::getNetIdRequest(networkName);
}

void DapNode::slotWalletsRequest()
{
  emit sigWalletsRequest();
}

void DapNode::slotFeeRequest (QString a_networkName)
{
  m_networkName = a_networkName;
  emit sigFeeRequest();
}

/****************************************//**
 * @name WEB3 CONNECTION ID MANAGEMENT
 *******************************************/
/// @{

void DapNode::handleDashboardRestart()
{
  qDebug() << "🔗 [WEB3 ID] Dashboard restart detected - forcing reconnection";
  if (web3) {
    web3->forceReconnect();
  }
}

QString DapNode::getWeb3ConnectionId()
{
  if (web3) {
    return web3->connectedId();
  }
  return QString();
}

void DapNode::clearWeb3ConnectionId()
{
  qDebug() << "🔗 [WEB3 ID] Clearing Web3 connection ID";
  if (web3) {
    web3->clearStoredConnectionId();
  }
}

/// @}

/*-----------------------------------------*/

NodeConnectStateMachine::NodeConnectStateMachine()
{
  init();
}

void NodeConnectStateMachine::start()
{
  nodeConnectMachine.start();
  commandState.start();
}

/* <DEFINES> */
#define ADDCONNECTMACHINESTATE(a_state, a_enableStoraging) \
  addConnectMachineState (&a_state, #a_state, a_enableStoraging)
#define PRINT_SIGNAL_EMITION(a_name) \
  connect (this, &NodeConnectStateMachine::a_name, [] { DEBUGINFO << "stm->" #a_name; });
/* </DEFINES> */

void NodeConnectStateMachine::init()
{
  /* variables */
  stateToReturn             = nullptr;
  returningStateTransition  = nullptr;

  /* lambda's */
  auto clearOldStateTransition = [this]
  {
    if (stateToReturn == nullptr
        || returningStateTransition == nullptr)
      return;

    auto sourceState  = returningStateTransition->sourceState();
    if (sourceState)
      sourceState->removeTransition (returningStateTransition);

    delete returningStateTransition;
    returningStateTransition  = nullptr;
  };

  auto addConnectMachineState = [this, clearOldStateTransition] (QState *a_state, const char *a_stateName, bool a_enableStoraging)
  {
    DEBUGINFO << QString ("addConnectMachineState: \"%1\" state").arg (a_stateName);

    /* store into state machine */
    nodeConnectMachine.addState (a_state);

    /* set state's name */
    a_state->setObjectName (a_stateName);

    /* if required to store state and wait when incorrect id is received */
    if (a_enableStoraging)
    {
      /* add transition to waiting state */
      a_state->addTransition (this, &NodeConnectStateMachine::sigGotIncorrectId,
                              &waitingForCorrectId);

      /* attach storaging behavior */
      QObject::connect (a_state, &QState::entered, this, [this, a_state, clearOldStateTransition]
      {
        /* check */
        if (a_state == nullptr)
          return;

        /* re-entered */
        bool reEntered  = (a_state == stateToReturn);

        /* clear old state 'transition back' */
        clearOldStateTransition();

        /* store this state */
        stateToReturn = a_state;

        /* create 'transition back' */
        returningStateTransition =
          waitingForCorrectId.addTransition(
            this,
            &NodeConnectStateMachine::sigConnectionIdReceived,
            a_state);

        /* print state name */
        DEBUGINFO << QString ("%2 \"%1\" state [return-state-storage]")
                     .arg (a_state->objectName(), reEntered ? "Re-entered" : "Entered");
      });
    }
  };

  /* command states */
  commandState.addState(&waitingCommand);
  commandState.addState(&gettingWalletsData);
  commandState.addState(&transactionProcessing);
  commandState.addState(&gettingOrderList);

  commandState.setInitialState(&waitingCommand);

  /* connect states */
  ADDCONNECTMACHINESTATE (initialState,    false);
  ADDCONNECTMACHINESTATE (nodeDetection,   false);
  ADDCONNECTMACHINESTATE (nodeNotDetected, false);
  ADDCONNECTMACHINESTATE (nodeConnection,  false);
  ADDCONNECTMACHINESTATE (nodeNotConnected,false);
  ADDCONNECTMACHINESTATE (getWallets,      true);
  ADDCONNECTMACHINESTATE (getNetworks,     true);
  ADDCONNECTMACHINESTATE (getDataWallet,   true);
  ADDCONNECTMACHINESTATE (condTxCreate,    true);
  ADDCONNECTMACHINESTATE (getFee,          true);
  ADDCONNECTMACHINESTATE (getFeeIsolated,  true);
  ADDCONNECTMACHINESTATE (getNetId,        true);
  ADDCONNECTMACHINESTATE (mempoolTxHashRequest,  true);
  ADDCONNECTMACHINESTATE (mempoolTxHashEmpty,    true);
  ADDCONNECTMACHINESTATE (ledgerTxHashRequest,   true);
  ADDCONNECTMACHINESTATE (ledgerTxHashEmpty,     true);
  ADDCONNECTMACHINESTATE (createCertificate,     true);
  ADDCONNECTMACHINESTATE (checkTransactionCertificate,  true);
  ADDCONNECTMACHINESTATE (createTransactionCertificate, true);
  ADDCONNECTMACHINESTATE (getListKeys,     true);
  ADDCONNECTMACHINESTATE (getOrderList,    true);
  ADDCONNECTMACHINESTATE (getNodeConnectionData,  true);
  ADDCONNECTMACHINESTATE (waitingForCorrectId,    false);
  ADDCONNECTMACHINESTATE (waitingAndReconnecting, false);

  nodeConnectMachine.setInitialState(&initialState);

  connect (&waitingForCorrectId, &QState::entered, this, []
  {
    DEBUGINFO << "Entered \"waitingForCorrectId\" state";
  });

  /* signals */
  PRINT_SIGNAL_EMITION(sigGotIncorrectId);
  PRINT_SIGNAL_EMITION(sigConnectionIdReceived);
  PRINT_SIGNAL_EMITION(sigErrorDetected);
  PRINT_SIGNAL_EMITION(sigNodeNotConnected);
  PRINT_SIGNAL_EMITION(sigRestartWaiting);

  /* return to initial state */
  waitingForCorrectId.addTransition (this, &NodeConnectStateMachine::sigErrorDetected,
                                     &initialState);
  connect (&initialState, &QState::entered,
           clearOldStateTransition);

  /* node not responding */
  waitingForCorrectId.addTransition (this, &NodeConnectStateMachine::sigNodeNotConnected,
                                     &waitingAndReconnecting);

  /* restart waiting */
  waitingAndReconnecting.addTransition (this, &NodeConnectStateMachine::sigRestartWaiting,
                                        &waitingForCorrectId);
  connect (&waitingAndReconnecting, &QState::entered, this, [this]
  {
    DEBUGINFO << "Entered \"waitingAndReconnecting\" state";
    QTimer::singleShot (DapNode::NODE_CONNECT_REQUEST_REPEAT_PERIOD, this, [this]
    {
      DEBUGINFO << "repeat waiting for correct id";
      emit sigRestartWaiting();
    });
  });

  /* finish */
  qDebug() << "nodeConnectMachine::init";
}

QString DapNode::getConnectedAppType() const
{
    // Access web3 component to get the detected app type
    if (web3) {
        return web3->connectedAppType();
    }
    return QString("Dashboard"); // Default fallback
}

/*-----------------------------------------*/
