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
    initTransitions();
    initStates();
}


DapNode::~ DapNode()
{
}


void DapNode::stopCheckingNodeRequest()
{

}


void DapNode::startCheckingNodeRequest()
{
    DEBUGINFO << "startCheckingNodeRequest";
    emit uiStartNodeDetection();
}

void DapNode::initTransitions()
{
    DEBUGINFO << "initStates";
    // node search  initialState -> startCheckingNode
    m_stm->initialState.addTransition(this, &DapNode::uiStartNodeDetection,
    &m_stm->nodeDetection);

    // node detection -> node detected
    m_stm->nodeDetection.addTransition(web3, &DapNodeWeb3::nodeDetected,
    &m_stm->nodeConnection);
    // node detection -> node detected
    m_stm->nodeDetection.addTransition(web3, &DapNodeWeb3::checkNodeStatus,
    &m_stm->nodeGetStatus);
    // node detection -> get node status
    m_stm->nodeDetection.addTransition(web3, &DapNodeWeb3::nodeNotDetected,
    &m_stm->nodeNotDetected);

    // node not detected -> node detection, repeat detection
    m_stm->nodeNotDetected.addTransition(this, &DapNode::repeatNodeDetection,
    &m_stm->nodeDetection);

    // node connection -> node connected, wallets list request
    m_stm->nodeConnection.addTransition(web3, &DapNodeWeb3::connectionIdReceived,
    &m_stm->nodeGetStatus);
    // node connection -> node not connected, repeat connection
    m_stm->nodeConnection.addTransition(web3, &DapNodeWeb3::nodeNotConnected,
    &m_stm->nodeNotConnected);

    // node not connected -> node connection, repeat connection
    m_stm->nodeNotConnected.addTransition(this, &DapNode::repeatNodeConnection,
    &m_stm->nodeConnection);

    // node status !ok -> nodeGetWallets
    m_stm->nodeGetStatus.addTransition(this, &DapNode::errorDetected,
    &m_stm->initialState);
    // node status ok -> initialState
    m_stm->nodeGetStatus.addTransition(web3, &DapNodeWeb3::statusOk,
    &m_stm->getWallets);
    // get wallets -> wallets received
    m_stm->getWallets.addTransition(this, &DapNode::walletsReceived,
    &m_stm->getNetworks);
    // get wallets -> init state
    m_stm->getWallets.addTransition(this, &DapNode::errorDetected,
    &m_stm->initialState);
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
    // to conditional tx create
    m_stm->initialState.addTransition(this, &DapNode::sigCondTxCreateRequest,
    &m_stm->condTxCreate);
    // to check ledger if create successfuly
    m_stm->condTxCreate.addTransition(web3, &DapNodeWeb3::sigCondTxCreateSuccess,
    &m_stm->ledgerTxHashRequest);
    // to ledger empty
    m_stm->ledgerTxHashRequest.addTransition(this, &DapNode::errorDetected,
    &m_stm->ledgerTxHashEmpty);
    // to init state
    m_stm->ledgerTxHashRequest.addTransition(web3, &DapNodeWeb3::sigLedgerContainHash,
    &m_stm->initialState);
    // to init state if error
    m_stm->ledgerTxHashEmpty.addTransition(this, &DapNode::repeatReadLedger,
    &m_stm->ledgerTxHashRequest);
}


void DapNode::initStates()
{
    // initial state
    connect(&m_stm->initialState, &QState::entered, this, [=](){
        DEBUGINFO << "&initialState, &QState::entered";
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
    connect(&m_stm->nodeConnection, &QState::entered, web3, &DapNodeWeb3::nodeConnectionRequest);
    // get node status
    connect(&m_stm->nodeGetStatus,  &QState::entered, web3, &DapNodeWeb3::nodeStatusRequest);
    // get wallets
    connect(&m_stm->getWallets, &QState::entered, web3, &DapNodeWeb3::walletsRequest);
    // get networks
    connect(&m_stm->getNetworks, &QState::entered, web3, &DapNodeWeb3::networksRequest);
    // get wallets data
    connect (&m_stm->getDataWallet, &QState::entered, this, [=](){
        m_walletsData = QJsonObject();
        walletDataRequest();
    });
    // create conditional transaction
    connect(&m_stm->condTxCreate, &QState::entered, this, [=](){
            web3->DapNodeWeb3::condTxCreateRequest(
                        m_walletName,
                        m_networkName,
                        m_sertificateName,
                        m_tokenName,
                        m_value,
                        m_unit);
    });
    // check ledger
    connect(&m_stm->ledgerTxHashRequest, &QState::entered, web3, &DapNodeWeb3::getLedgerTxHashRequest);
    connect(&m_stm->ledgerTxHashEmpty, &QState::entered, this, [=](){
        DEBUGINFO  << "&ledgerTxHashEmpty, &QState::entered";
        QTimer::singleShot(LEDGER_REQUEST_REPEAT_PERIOD, [=](){
            DEBUGINFO  << "&singleShotledgerTxHashEmpty";
            emit repeatReadLedger();
        });
    });
}

void DapNode::initWeb3Connections()
{
    // node detected signal
    connect(web3, &DapNodeWeb3::nodeDetected, this, &DapNode::sigNodeDetected);
    connect(web3, &DapNodeWeb3::checkNodeStatus, this, &DapNode::sigNodeDetected);

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
        emit sigError(errorCode, errorMessage);
    });
    // get wallet data
    connect(web3, &DapNodeWeb3::sigWalletDataReady, this, &DapNode::parseDataWallet);
    // ledger contain transaction hash
    connect(web3, &DapNodeWeb3::sigLedgerContainHash, this, &DapNode::sigLedgerContainHash);
}

void DapNode::slotCondTxCreateRequest(QString walletName, QString networkName, QString tokenName, QString value, QString unit)
{
    m_walletName = walletName;
    m_networkName = networkName;
    m_tokenName = tokenName;
    m_value = value;
    m_unit =unit;
    m_sertificateName = QString("rslCert01_public"); // TODO
    emit sigCondTxCreateRequest();
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


void DapNode::start()
{
    DEBUGINFO  << "start DapNode";
    m_stm->start();
}

