#include "SignInScreenSerialNumberBase.h"

#include "DapDataLocal.h"
#include "SerialKeyField.h"

const QString SignInScreenSerialNumberBase::SCREEN_NAME = "SignIn";

SignInScreenSerialNumberBase::SignInScreenSerialNumberBase(QWidget *a_parent)
    : ScreenWithScreenPopupsAbstract(a_parent)
    , m_inputStates                         (new QStateMachine(this))
    , m_stt_serialKey                       (new QState(m_inputStates))
    , m_stt_serialKey_unactivated           (new QState(m_stt_serialKey))
    , m_stt_serialKey_unactivated_empty     (new QState(m_stt_serialKey_unactivated))
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

#ifndef USING_SIGN_IN_SCREEN_WITH_TARIFF_LINK
    //Add this to inheritable class if you want have working inharitance initVariantUi method:
    this->create(m_ui);

    //And this:
    AdaptiveScreen::initScreen(this);
#endif

}

void SignInScreenSerialNumberBase::initVariantUi(QWidget *a_widget)
{
    qDebug() << "initVariantUi";

#ifdef Q_OS_ANDROID
    m_widgetSizeController = new WidgetInputSizeController(this);

    m_widgetSizeController->addDisappearingWidget(m_ui->lblLogo);

    m_widgetSizeController->addWidgetEmitsSignal(m_ui->ledSerialKey);
#endif

    m_ui->cbbServer->popup()->setObjectName("cbbServer_popup");

#ifndef Q_OS_ANDROID
    m_ui->cbbServer->setPositionPopup(PositionPopup::overlappingPosition);
#endif
//*************************Serial field***************************************
#ifdef Q_OS_MAC
    m_ui->ledSerialKey->setAttribute(Qt::WA_MacShowFocusRect,false);
#endif

    connect(m_ui->btnConnect, &QPushButton::clicked, [this]{
        if (m_ui->ledSerialKey->text().isEmpty())
            emit this->serialKeyError();
        else
            emit this->connectionRequested();
    });


//================= ServersModl: =================
    m_ui->cbbServer->setModel(DapDataLocal::serversData());

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
        Q_UNUSED(a_serverInfo)
        m_ui->cbbServer->setEnabled(true);
    });

    connect(DapDataLocal::serversData(), &DapServersData::serversCleared, [this](){
        m_ui->cbbServer->setEnabled(false);
    });
//================= end of ServersModel: =================

    connect(m_ui->cbbServer  , SIGNAL(currentIndexChanged(int))  , this, SIGNAL(serverChanged(int)));
    connect(m_ui->ledSerialKey, &SerialKeyField::textEdited, this, &SignInScreenSerialNumberBase::serialKeyEdited);


    connect(m_ui->ledSerialKey, &SerialKeyField::textChanged, this, &SignInScreenSerialNumberBase::serialKeyChanged);

    //Default properties:
    m_ui->btnConnect->setText(LOGIN_BUTTON_TEXT_DEFAULT);

    Utils::setPropertyAndUpdateStyle(m_ui->lblStatusMessage, Properties::WRONG, false);
    Utils::setPropertyAndUpdateStyle(m_ui->ledSerialKey, Properties::WRONG, false);

    this->adjustStateMachine();

    ScreenWithScreenPopupsAbstract::initVariantUi(a_widget);
}


void SignInScreenSerialNumberBase::adjustStateMachine()
{
    m_inputStates->setChildMode(QState::ChildMode::ParallelStates);
    m_inputStates->setGlobalRestorePolicy(QState::RestorePolicy::RestoreProperties);
    m_stt_serialKey             ->setInitialState(m_stt_serialKey_unactivated);

    if (!m_ui->ledSerialKey->text().isEmpty())
        m_stt_serialKey_unactivated ->setInitialState(m_stt_serialKey_unactivated_entered);
    else
        m_stt_serialKey_unactivated ->setInitialState(m_stt_serialKey_unactivated_empty);

    m_stt_serverState           ->setInitialState(m_stt_serverState_disconnected);
//    m_stt_serverState_inactivity->setInitialState(m_stt_serverState_inactivity_normal);
    m_stt_serverState_loading   ->setInitialState(m_stt_serverState_loading_connecting);
    m_stt_serviceState          ->setInitialState(m_stt_serviceState_connected);


    m_stt_serialKey_unactivated->addTransition(this, &SignInScreenSerialNumberBase::serialKeyError, m_stt_serialKey_unactivated_wrong);
    m_stt_serialKey_activated->addTransition(this, &SignInScreenSerialNumberBase::serialKeyError, m_stt_serialKey_unactivated_wrong);
    m_stt_serialKey_unactivated_wrong->addTransition(m_ui->ledSerialKey, &SerialKeyField::textChanged, m_stt_serialKey_unactivated_entered);
    m_stt_serialKey_unactivated_empty->addTransition(m_ui->ledSerialKey, &SerialKeyField::textChanged, m_stt_serialKey_unactivated_entered);
    m_stt_serialKey_unactivated_wrong->addTransition(m_ui->ledSerialKey, &SerialKeyField::textEditedAndFilledOut, m_stt_serialKey_unactivated_entered);
    m_stt_serialKey_unactivated_entered->addTransition(m_ui->ledSerialKey, &SerialKeyField::textChangedAndCleaned, m_stt_serialKey_unactivated_empty);

    connect(m_ui->ledSerialKey, &SerialKeyField::textChanged, [this]{
       m_ui->lblStatusMessage->clear();
    });


//    m_stt_serialKey_unactivated_entered->addTransition(m_ui->ledSerialKey, &SerialKeyField::textChanged, m_stt_serialKey_unactivated_input);

    m_stt_serialKey_unactivated->addTransition(this, &SignInScreenSerialNumberBase::activated, m_stt_serialKey_activated);
    m_stt_serialKey_activated->addTransition(this, &SignInScreenSerialNumberBase::unactivated, m_stt_serialKey_unactivated);

    m_stt_serverState_disconnected->addTransition(this, &SignInScreenSerialNumberBase::connectionRequested, m_stt_serverState_loading_connecting);
    m_stt_serverState_loading_connecting->addTransition(this, &SignInScreenSerialNumberBase::connectionError, m_stt_serverState_disconnected);

    m_stt_serverState_loading_serverList->addTransition(m_ui->cbbServer->model(), &QAbstractItemModel::rowsInserted, m_stt_serverState_disconnected);
    m_stt_serverState_disconnected->addTransition(this, &SignInScreenSerialNumberBase::serversListCleared, m_stt_serverState_loading_serverList);
    m_stt_serverState_loading_connecting->addTransition(this, &SignInScreenSerialNumberBase::connected, m_stt_serverState_disconnected);
    m_stt_serverState_loading_connecting->addTransition(this, &SignInScreenSerialNumberBase::disconnected, m_stt_serverState_disconnected);

    m_stt_serviceState_connected->addTransition(this, &SignInScreenSerialNumberBase::serviceDisconnected, m_stt_serviceState_connecting);
    m_stt_serviceState_connecting->addTransition(this, &SignInScreenSerialNumberBase::serviceConnected, m_stt_serviceState_connected);


    connect(m_stt_serialKey_unactivated, &QState::entered, []{
        qDebug() << " ============ m_stt_serialKey_unactivated entered";});
    connect(m_stt_serialKey_unactivated_empty, &QState::entered, []{
        qDebug() << " ============ m_stt_serialKey_unactivated_empty entered";
    });
    connect(m_stt_serialKey_unactivated_wrong, &QState::entered, []{qDebug() << " ============ m_stt_serialKey_unactivated_wrong entered";});
    connect(m_stt_serialKey_unactivated_entered, &QState::entered, []{
        qDebug() << " ============ m_stt_serialKey_unactivated_entered entered";
    });
    connect(m_stt_serialKey_activated, &QState::entered, []{qDebug() << " ============ m_stt_serialKey_activated entered";});
    connect(m_stt_serverState_disconnected, &QState::entered, []{qDebug() << " ============ m_stt_serverState_normal entered";});
    connect(m_stt_serverState_loading, &QState::entered, []{qDebug() << " ============ m_stt_serverState_loading entered";});
    connect(m_stt_serverState_loading_connecting, &QState::entered, [this]{
        Utils::setPropertyAndUpdateStyle(m_ui->lblStatusMessage, Properties::WRONG, false);
        qDebug() << " ============ m_stt_serverState_loading_connecting entered";});
    connect(m_stt_serverState_loading_serverList, &QState::entered, []{qDebug() << " ============ m_stt_serverState_loading_serverList entered";});
    connect(m_stt_serviceState_connected, &QState::entered, []{qDebug() << " ============ m_stt_serviceState_connected entered";});
    connect(m_stt_serviceState_connecting, &QState::entered, []{qDebug() << " ============ m_stt_serviceState_connecting entered";});



    connect(m_stt_serialKey_unactivated_empty   , &QState::entered, [this]{
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
        if (this->serialKey().isEmpty())
        {
            emit this->serialKeyError();
            return;
        }
        if (!this->isLoadingState() && this->serviceIsConnected())
            m_ui->btnConnect->setEnabled(true);

        m_ui->ledSerialKey->hide();

    });

    connect(m_stt_serialKey_activated           , &QState::exited, [this]{
        m_ui->ledSerialKey->show();

    });

    // Server
    connect(m_stt_serverState_disconnected      , &QState::entered, [this] {
        if (this->serialKeyIsEntered() && this->serviceIsConnected())
        {
            m_ui->btnConnect->setText(LOGIN_BUTTON_TEXT_DEFAULT);
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
        m_ui->lblStatusMessage->setText(STATUS_TEXT_CONNECTING_TO_SERVICE);
    });


    connect(m_stt_serviceState_connecting       , &QState::exited, [this]{
        m_ui->lblStatusMessage->clear();
    });



    m_stt_serverState_loading_connecting->assignProperty(m_ui->lblStatusMessage, qPrintable(Properties::TEXT), LOGIN_TEXT_CONNECTING);

    m_stt_serverState_loading_connecting->assignProperty(m_ui->btnConnect, qPrintable(Properties::TEXT), LOGIN_BUTTON_TEXT_CONNECTING);

    this->m_inputStates->start();
}

QString SignInScreenSerialNumberBase::screenName()
{
    return SCREEN_NAME;
}

QString SignInScreenSerialNumberBase::serialKey()
{
    return m_ui->ledSerialKey->text();
}

void SignInScreenSerialNumberBase::setState(ConnectionState a_state)
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

    if (a_state == ConnectionState::Disconnected)
    {
        emit this->disconnected();
        m_ui->lblStatusMessage->clear();
    }
}

void SignInScreenSerialNumberBase::setActivated(bool a_activated)
{
    emit a_activated ? this->activated() : this->unactivated();

    Utils::setPropertyAndUpdateStyle(m_ui->wgtUnderSerialMargin,Properties::ACTIVE,a_activated);

    if (!m_inputStates->isRunning())
        m_stt_serialKey->setInitialState(a_activated ? m_stt_serialKey_activated : m_stt_serialKey_unactivated);
}

void SignInScreenSerialNumberBase::setCurrentServerName(const QString &a_serverName)
{
    m_ui->cbbServer->setCurrentText(a_serverName);
}

void SignInScreenSerialNumberBase::setSerialKey(const QString &a_serial)
{
    m_ui->ledSerialKey->setText(a_serial);

    if (!m_inputStates->isRunning())
    {
        if (!m_ui->ledSerialKey->text().isEmpty())
            m_stt_serialKey_unactivated->setInitialState(m_stt_serialKey_unactivated_entered);
        else
            m_stt_serialKey_unactivated->setInitialState(m_stt_serialKey_unactivated_empty);
    }
}

void SignInScreenSerialNumberBase::setErrorMessage(const QString &a_errorMsg /*= ""*/)
{
    if (a_errorMsg == "Incorrect serial key"
            || a_errorMsg == "Serial key not found in database"
            || a_errorMsg == "Serial key already activated on another device")
    {
        emit this->serialKeyError();
        emit this->connectionError();
    }
    else
        emit this->connectionError();

    Utils::setPropertyAndUpdateStyle(m_ui->lblStatusMessage, Properties::WRONG, true);
    m_ui->lblStatusMessage->setText(translatedErrorMsg(a_errorMsg));
}

QString SignInScreenSerialNumberBase::translatedErrorMsg(QString a_errorMsg)
{
    if (a_errorMsg == "Incorrect serial key")
        return tr("Incorrect serial key");

    else if (a_errorMsg == "Serial key not found in database")
        return  tr("Serial key not found in database");

    else if (a_errorMsg == "Serial key activated, try to authorize")
        return  tr("Serial key activated, try to authorize");

    else if (a_errorMsg == "Server not available. Please, try others or the same later")
            return tr("Server not available. Please, try others or the same later");

    else return a_errorMsg;
}

bool SignInScreenSerialNumberBase::serialKeyIsEntered()
{
    return  m_stt_serialKey_unactivated_entered->active() || m_stt_serialKey_activated->active();
}

bool SignInScreenSerialNumberBase::serviceIsConnected()
{
    return m_stt_serviceState->active();
}

bool SignInScreenSerialNumberBase::isLoadingState()
{
    return !m_stt_serverState_disconnected->active();
}

QList<CustomPopup *> SignInScreenSerialNumberBase::customPopups()
{
    return {m_ui->cbbServer->popup()};
}

