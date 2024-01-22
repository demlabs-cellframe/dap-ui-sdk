#include "DapNode.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include "DapDataLocal.h"
#include <QTimer>
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
    initWeb3Connections();
    initStmTransitions();
    initStmStates();
    initCommandsStm();
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
    DEBUGINFO << "startCheckingNodeRequest";
    if (m_stm->initialState.active())
    {
        DEBUGINFO << "startCheckingNodeRequest << uiStartNodeDetection()";
        emit uiStartNodeDetection();
    }
    else
    {
        if (m_stm->ledgerTxHashRequest.active() || m_stm->ledgerTxHashEmpty.active())
        {
            DEBUGINFO << "startCheckingNodeRequest << sigMempoolContainHash()";
            emit sigMempoolContainHash();
            return;
        }
        if (nodeDetected)
        {
            DEBUGINFO << "startCheckingNodeRequest << sigNodeDetected()";
            emit sigNodeDetected();
            return;
        }
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

    // get wallets -> node сonnection
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

    // paused getListKeys
    m_stm->getListKeys.addTransition(web3, &DapNodeWeb3::sigIncorrectId,
    &m_stm->getListKeysPaused);
    m_stm->getListKeysPaused.addTransition(web3, &DapNodeWeb3::connectionIdReceived,
    &m_stm->getListKeys);
    m_stm->getListKeysPaused.addTransition(this, &DapNode::errorDetected,
    &m_stm->initialState);

    // getOrderList -> initialState
    m_stm->getOrderList.addTransition(this, &DapNode::sigOrderListReceived,
    &m_stm->initialState);
    m_stm->getOrderList.addTransition(this, &DapNode::errorDetected,
    &m_stm->initialState);

    // paused getOrderList
    m_stm->getOrderList.addTransition(web3, &DapNodeWeb3::sigIncorrectId,
    &m_stm->getOrderListPaused);
    m_stm->getOrderListPaused.addTransition(web3, &DapNodeWeb3::connectionIdReceived,
    &m_stm->getOrderList);
    m_stm->getOrderListPaused.addTransition(this, &DapNode::errorDetected,
    &m_stm->initialState);

    // getting node ip
    m_stm->getNodeConnectionData.addTransition(this, &DapNode::sigNodeDumpReceived,
    &m_stm->initialState);
    m_stm->getNodeConnectionData.addTransition(this, &DapNode::errorDetected,
    &m_stm->initialState);

}

void DapNode::initStmStates()
{
    // initial state
    connect(&m_stm->initialState, &QState::entered, this, [=](){
        DEBUGINFO << "&initialState, &QState::entered";
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
        if (!m_isCDBLogined)
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
    connect(&m_stm->nodeDetection,  &QState::entered, web3, &DapNodeWeb3::nodeDetectedRequest);
    // connection request
    connect(&m_stm->nodeConnection, &QState::entered, this, [=](){
        if (!m_isCDBLogined)
            web3->nodeConnectionRequest();
    }); //web3, &DapNodeWeb3::nodeConnectionRequest);

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
    // get fee
    connect (&m_stm->getFee, &QState::entered, this, [=](){
        web3->getFeeRequest(m_networkName);
    });
    // get fee isolated
    connect (&m_stm->getFeeIsolated, &QState::entered, this, [=](){
        web3->getFeeRequest (m_networkName);
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
                        m_unit,
                        m_fee);
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
        web3->getListKeysRequest(m_networkName);
    });

    // get orders list
    connect(&m_stm->getOrderList, &QState::entered, this, [=](){
        web3->DapNodeWeb3::getOrdersListRequest(
                    m_networkName,
                    m_tokenName,
                    m_minPrice,
                    m_maxPrice,
                    m_unit);
    });

    connect(&m_stm->getNodeConnectionData, &QState::entered, this, [=](){
        web3->nodeDumpRequest(m_networkName);
    });
}

void DapNode::initWeb3Connections()
{
    // node detected signal
    connect(web3, &DapNodeWeb3::nodeDetected, this, &DapNode::sigNodeDetected);
    connect(web3, &DapNodeWeb3::checkNodeStatus, this, &DapNode::sigNodeDetected);
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
    // list keys ready
    connect(web3, &DapNodeWeb3::sigListKeys, this, [=](QStringList listKeys) {
        m_listKeys = listKeys;
        emit sigListKeysReceived();
    });
    // order list ready
    connect(web3, &DapNodeWeb3::sigOrderList, this, [=](QJsonArray ordersList) {
        QJsonArray orders;
        orderListFiltr(ordersList, orders, m_listKeys);
        emit sigOrderListReceived();
        emit sigOrderListReady(orders); //emit sigOrderListReady(orders);
    });

    connect(web3, &DapNodeWeb3::sigNodeIp, this, [=](QJsonObject data) {
        emit sigSendNodeIp(data);
    });

    //connect(web3, &DapNodeWeb3::sigOrderList, this, &DapNode::sigOrderListReady);
    // recieved fee
    connect(web3, &DapNodeWeb3::sigFee, this, [=](QString fee){
        m_fee = fee;
        emit sigFeeReceived();
    });
    connect (web3, &DapNodeWeb3::sigFeeData,
             this, [=] (const QJsonObject &a_data)
    {
      if (m_stm->getFeeIsolated.active())
        emit sigFeeReceivedData (a_data);
    });
    // connect to stream
    connect(web3, &DapNodeWeb3::sigNodeDump, this, [=](QList<QMap<QString, QString>> nodeDump) {
//         riemann, use dap_chain_net_id_by_name() "0x000000000000dddd"
        m_nodeInfo.serverDataFromList(nodeDump);
        emit sigNodeDumpReceived();
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
    m_networkName = networkName;
    m_tokenName = tokenName;
    m_minPrice = minPrice;
    m_maxPrice = maxPrice;
    m_unit = unit;
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

bool NodeInfo::serverDataFromList(const QList<QMap<QString, QString>>& nodeDump)
{
    foreach (const auto item, nodeDump)
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
    foreach(const QJsonValue& order, inOrders)
    {
        foreach (const QString& key, keys)
        {
            if (key == order.toObject()["pkey"].toString())
            {
                outOrders.append(order);
                break;
            }
        }
    }
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

