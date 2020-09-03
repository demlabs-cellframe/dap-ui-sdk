#include "SignInScreen.h"

#include "DapDataLocal.h"
#include "SerialKeyField.h"

const QString SignInScreen::SCREEN_NAME = "SignIn";

SignInScreen::SignInScreen(QWidget *a_parent)
    : ScreenWithScreenPopupsAbstract(a_parent)
    , m_inputStates                         (new QStateMachine(this))
    , m_stt_serialKey                       (new QState(m_inputStates))
    , m_stt_serialKey_unactivated           (new QState(m_stt_serialKey))
    , m_stt_serialKey_unactivated_input     (new QState(m_stt_serialKey_unactivated))
    , m_stt_serialKey_unactivated_entered   (new QState(m_stt_serialKey_unactivated))
    , m_stt_serialKey_unactivated_wrong     (new QState(m_stt_serialKey_unactivated))
    , m_stt_serialKey_activated             (new QState(m_stt_serialKey))
    , m_stt_serverState                     (new QState(m_inputStates))
    , m_stt_serverState_disconnected        (new QState(m_stt_serverState))
    , m_stt_serverState_loading             (new QState(m_stt_serverState))
    , m_stt_serverState_loading_serverList  (new QState(m_stt_serverState_loading))
    , m_stt_serverState_loading_connecting  (new QState(m_stt_serverState_loading))
    , m_stt_serviceState                    (new QState(m_inputStates))
    , m_stt_serviceState_connected          (new QState(m_stt_serviceState))
    , m_stt_serviceState_connecting         (new QState(m_stt_serviceState))

{
    this->create(m_ui);

    this->adjustStateMachine();
    AdaptiveScreen::initScreen(this);
}

void SignInScreen::initVariantUi(QWidget *a_widget)
{
    qDebug() << "initVariantUi";

    m_ui->cbbServer->popup()->setObjectName("cbbServer_popup");

#ifndef Q_OS_ANDROID
    m_ui->cbbServer->setPositionPopup(PositionPopup::overlappingPosition);
#endif
//*************************Serial field***************************************


    connect(m_ui->btnConnect,&QPushButton::clicked, this, &SignInScreen::connectionRequested);


//================= ServersModl: =================
    for (DapServerInfo& server :DapDataLocal::serversData()->servers())
        m_ui->cbbServer->addItem(server.name);

    if (m_ui->cbbServer->count()>0)
    {
        m_ui->cbbServer->setEnabled(true);
    }
    else
    {
        m_stt_serverState->setInitialState(m_stt_serverState_loading);
        m_stt_serverState_loading->setInitialState(m_stt_serverState_loading_serverList);
    }

    connect(DapDataLocal::serversData(), &DapServersData::serverAdded, [=](const DapServerInfo& a_serverInfo){
        m_ui->cbbServer->addItem(a_serverInfo.name);
        m_ui->cbbServer->setEnabled(true);
    });

    connect(DapDataLocal::serversData(), &DapServersData::serversCleared, [this](){
        m_ui->cbbServer->clear();
        m_ui->cbbServer->setEnabled(false);
    });
//================= end of ServersModel: =================

    connect(m_ui->cbbServer  , SIGNAL(currentIndexChanged(int))  , this, SIGNAL(serverChanged(int)));
    connect(m_ui->ledSerialKey, &SerialKeyField::textEdited, this, &SignInScreen::serialKeyEdited);


    connect(m_ui->ledSerialKey, &SerialKeyField::textChanged, this, &SignInScreen::serialKeyChanged);

    //Default properties:
    m_ui->btnConnect->setText(tr(BUTTON_TEXT_DEFAULT));

    Utils::setPropertyAndUpdateStyle(m_ui->lblStatusMessage, Properties::WRONG, true);
    Utils::setPropertyAndUpdateStyle(m_ui->ledSerialKey, Properties::WRONG, false);

    ScreenWithScreenPopupsAbstract::initVariantUi(a_widget);
}

void SignInScreen::adjustStateMachine()
{
    m_inputStates->setChildMode(QState::ChildMode::ParallelStates);
    m_inputStates->setGlobalRestorePolicy(QState::RestorePolicy::RestoreProperties);
    m_stt_serialKey             ->setInitialState(m_stt_serialKey_unactivated);

    if (m_ui->ledSerialKey->isFilledOut())
        m_stt_serialKey_unactivated ->setInitialState(m_stt_serialKey_unactivated_entered);
    else
        m_stt_serialKey_unactivated ->setInitialState(m_stt_serialKey_unactivated_input);

    m_stt_serverState           ->setInitialState(m_stt_serverState_disconnected);
//    m_stt_serverState_inactivity->setInitialState(m_stt_serverState_inactivity_normal);
    m_stt_serverState_loading   ->setInitialState(m_stt_serverState_loading_connecting);
    m_stt_serviceState          ->setInitialState(m_stt_serviceState_connected);


    m_stt_serialKey_unactivated->addTransition(this, &SignInScreen::serialKeyError, m_stt_serialKey_unactivated_wrong);
    m_stt_serialKey_unactivated_wrong->addTransition(m_ui->ledSerialKey, &SerialKeyField::textChanged, m_stt_serialKey_unactivated_input);
    m_stt_serialKey_unactivated_input->addTransition(m_ui->ledSerialKey, &SerialKeyField::textEditedAndFilledOut, m_stt_serialKey_unactivated_entered);
    m_stt_serialKey_unactivated_wrong->addTransition(m_ui->ledSerialKey, &SerialKeyField::textEditedAndFilledOut, m_stt_serialKey_unactivated_entered);


    m_stt_serialKey_unactivated_entered->addTransition(m_ui->ledSerialKey, &SerialKeyField::textChanged, m_stt_serialKey_unactivated_input);

    m_stt_serialKey_unactivated->addTransition(this, &SignInScreen::activated, m_stt_serialKey_activated);
    m_stt_serialKey_activated->addTransition(this, &SignInScreen::unactivated, m_stt_serialKey_unactivated);

    m_stt_serverState_disconnected->addTransition(this, &SignInScreen::connectionRequested, m_stt_serverState_loading_connecting);
    m_stt_serverState_loading_connecting->addTransition(this, &SignInScreen::connectionError, m_stt_serverState_disconnected);

    m_stt_serverState_loading_serverList->addTransition(m_ui->cbbServer->model(), &QAbstractItemModel::rowsInserted, m_stt_serverState_disconnected);
    m_stt_serverState_disconnected->addTransition(this, &SignInScreen::serversListCleared, m_stt_serverState_loading_serverList);
    m_stt_serverState_loading_connecting->addTransition(this, &SignInScreen::connected, m_stt_serverState_disconnected);

    m_stt_serviceState_connected->addTransition(this, &SignInScreen::serviceDisconnected, m_stt_serviceState_connecting);
    m_stt_serviceState_connecting->addTransition(this, &SignInScreen::serviceConnected, m_stt_serviceState_connected);


    connect(m_stt_serialKey_unactivated, &QState::entered, []{qDebug() << " ============ m_stt_serialKey_unactivated entered";});
    connect(m_stt_serialKey_unactivated_input, &QState::entered, []{
        qDebug() << " ============ m_stt_serialKey_unactivated_input entered";
    });
    connect(m_stt_serialKey_unactivated_wrong, &QState::entered, []{qDebug() << " ============ m_stt_serialKey_unactivated_wrong entered";});
    connect(m_stt_serialKey_unactivated_entered, &QState::entered, []{
        qDebug() << " ============ m_stt_serialKey_unactivated_entered entered";
    });
    connect(m_stt_serialKey_activated, &QState::entered, []{qDebug() << " ============ m_stt_serialKey_activated entered";});
    connect(m_stt_serverState_disconnected, &QState::entered, []{qDebug() << " ============ m_stt_serverState_normal entered";});
    connect(m_stt_serverState_loading, &QState::entered, []{qDebug() << " ============ m_stt_serverState_loading entered";});
    connect(m_stt_serverState_loading_connecting, &QState::entered, []{qDebug() << " ============ m_stt_serverState_loading_connecting entered";});
    connect(m_stt_serverState_loading_serverList, &QState::entered, []{qDebug() << " ============ m_stt_serverState_loading_serverList entered";});
    connect(m_stt_serviceState_connected, &QState::entered, []{qDebug() << " ============ m_stt_serviceState_connected entered";});
    connect(m_stt_serviceState_connecting, &QState::entered, []{qDebug() << " ============ m_stt_serviceState_connecting entered";});



    connect(m_stt_serialKey_unactivated_input   , &QState::entered, [this]{
        m_ui->btnConnect->setEnabled(false);
    });

    connect(m_stt_serialKey_unactivated_wrong   , &QState::entered, [this]{
        m_ui->btnConnect->setEnabled(false);
        Utils::setPropertyAndUpdateStyle(m_ui->ledSerialKey, Properties::WRONG, true);
    });
    connect(m_stt_serialKey_unactivated_wrong   , &QState::exited, [this]{
        Utils::setPropertyAndUpdateStyle(m_ui->ledSerialKey, Properties::WRONG, false);
    });

    connect(m_stt_serialKey_unactivated_entered , &QState::entered, [this]{
        if (!this->isLoadingState() && this->serviceIsConnected())
            m_ui->btnConnect->setEnabled(true);
    });

    connect(m_stt_serialKey_activated           , &QState::entered, [this]{
        if (!this->isLoadingState() && this->serviceIsConnected())
            m_ui->btnConnect->setEnabled(true);

        m_ui->ledSerialKey->hide();
#ifndef Q_OS_ANDROID
        m_ui->wgtFrameBottom->hide();
#endif
    });

    connect(m_stt_serialKey_activated           , &QState::exited, [this]{
        m_ui->ledSerialKey->show();
#ifndef Q_OS_ANDROID
        m_ui->wgtFrameBottom->show();
#endif
    });

    // Server
    connect(m_stt_serverState_disconnected      , &QState::entered, [this] {
        if (this->serialKeyIsEntered() && this->serviceIsConnected())
        {
            m_ui->btnConnect->setEnabled(true);
        }
    });

    connect(m_stt_serverState_loading           , &QState::entered, [this]{
        m_ui->btnConnect->setEnabled(false);
    });

    m_stt_serverState_loading_serverList->assignProperty(m_ui->cbbServer, qPrintable(Properties::CAPTION), tr("Updating server list..."));

    // Service
    connect(m_stt_serviceState_connected        , &QState::entered, [this]
    {
        if (this->serialKeyIsEntered()&& !this->isLoadingState())
            m_ui->btnConnect->setEnabled(true);
    });
    connect(m_stt_serviceState_connecting       , &QState::entered, [this]
    {
        m_ui->btnConnect->setEnabled(false);
        m_ui->lblStatusMessage->setText(tr(STATUS_TEXT_CONNECTING_TO_SERVICE));
    });


    connect(m_stt_serviceState_connecting       , &QState::exited, [this]{
        m_ui->lblStatusMessage->clear();
    });

    m_stt_serverState_loading_connecting->assignProperty(m_ui->lblStatusMessage, qPrintable(Properties::TEXT), "");

    m_stt_serverState_loading_connecting->assignProperty(m_ui->btnConnect, qPrintable(Properties::TEXT), BUTTON_TEXT_CONNECTING);

    this->m_inputStates->start();
}

QString SignInScreen::screenName()
{
    return SCREEN_NAME;
}

void SignInScreen::setState(ConnectionState a_state)
{
    //initial states adjusting:
    if (!m_inputStates->isRunning())
    {
        if (a_state == ConnectionState::ConnectingToService)
            m_stt_serviceState->setInitialState(m_stt_serviceState_connecting);
        else
            m_stt_serviceState->setInitialState(m_stt_serviceState_connected);


        if (a_state == ConnectionState::Connecting)
        {
            m_stt_serverState->setInitialState(m_stt_serverState_loading);
            m_stt_serverState_loading->setInitialState(m_stt_serverState_loading_connecting);
        }
        else
            m_stt_serverState->setInitialState(m_stt_serverState_disconnected);

        return;
    }

    if (m_stt_serviceState_connected->active() && a_state == ConnectionState::ConnectingToService)
        emit this->serviceDisconnected();

    else if (m_stt_serviceState_connecting->active() && a_state != ConnectionState::ConnectingToService)
        emit this->serviceConnected();

    if (a_state == ConnectionState::Connected)
    {
        emit this->connected();
        m_ui->lblStatusMessage->clear();
    }
}

void SignInScreen::setActivated(bool a_activated)
{
    if(a_activated)
    {
        if(m_ui->ledSerialKey->text().isEmpty())
            a_activated = false;
        else
            a_activated = true;
    }
    else a_activated = false;

    emit a_activated ? this->activated() : this->unactivated();

    Utils::setPropertyAndUpdateStyle(m_ui->wgtUnderSerialMargin,Properties::ACTIVE,a_activated);

    if (!m_inputStates->isRunning())
        m_stt_serialKey->setInitialState(a_activated ? m_stt_serialKey_activated : m_stt_serialKey_unactivated);
}

void SignInScreen::setCurrentServerName(const QString &a_serverName)
{
    m_ui->cbbServer->setCurrentText(a_serverName);
}

void SignInScreen::setSerialKey(const QString &a_serial)
{
    m_ui->ledSerialKey->setText(a_serial);

    if (!m_inputStates->isRunning())
    {
        if (m_ui->ledSerialKey->isFilledOut())
            m_stt_serialKey_unactivated->setInitialState(m_stt_serialKey_unactivated_entered);
        else
            m_stt_serialKey_unactivated->setInitialState(m_stt_serialKey_unactivated_input);
    }
}

void SignInScreen::setErrorMessage(const QString &a_errorMsg /*= ""*/)
{
    if (a_errorMsg == "Incorrect serial key"
            || a_errorMsg == "Serial key not found in database"
            || a_errorMsg == "Serial key activated, try to authorize")
    {
        emit this->serialKeyError();
        emit this->connectionError();
    }
    else
        emit this->connectionError();

    m_ui->lblStatusMessage->setText(translatedErrorMsg(a_errorMsg));
}

QString SignInScreen::translatedErrorMsg(QString a_errorMsg)
{
    if (a_errorMsg == "Incorrect serial key")
        return tr("Incorrect serial key");

    else if (a_errorMsg == "Serial key not found in database")
        return  tr("Serial key not found in database");

    else if (a_errorMsg == "Serial key activated, try to authorize")
        return  tr("Serial key activated, try to authorize");

    else return a_errorMsg;
}

bool SignInScreen::serialKeyIsEntered()
{
    return  m_stt_serialKey_unactivated_entered->active() || m_stt_serialKey_activated->active();
}

bool SignInScreen::serviceIsConnected()
{
    return m_stt_serviceState->active();
}

bool SignInScreen::isLoadingState()
{
    return !m_stt_serverState_disconnected->active();
}


QList<CustomPopup *> SignInScreen::customPopups()
{
    return {m_ui->cbbServer->popup()};
}

