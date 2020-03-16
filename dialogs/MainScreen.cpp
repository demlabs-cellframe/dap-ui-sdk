#include "MainScreen.h"


const QString MainScreen::SCREEN_NAME = "Main";

MainScreen::MainScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    this->setObjectName(this->screenName());
    create<Ui::MainScreen>();
}

QString MainScreen::screenName()
{
    return MainScreen::SCREEN_NAME;
}

void MainScreen::setState(ConnectionStates a_state)
{
    #ifdef Q_OS_ANDROID
    Q_UNUSED(a_state)
#else
    this->setChildProperties(LBL_STATUS_MESSAGE, Properties::TEXT , statusText(a_state));
    this->setChildProperties(LBL_STATUS_MESSAGE, Properties::STATE, a_state);

    this->updateChildStyle  (LBL_STATUS_MESSAGE);

    this->setChildProperties(BTN_SWITCH, Properties::STATE, a_state);
    this->updateChildStyle  (BTN_SWITCH);

    this->setChildProperties(CBB_SERVER, Properties::ENABLED, a_state == ConnectionStates::Disconnected);
    this->setChildProperties(BTN_SWITCH, Properties::ENABLED, (a_state == ConnectionStates::Disconnected || a_state == ConnectionStates::Connected));
#endif
}

void MainScreen::initVariantUi(QWidget *a_widget)
{

#ifdef Q_OS_ANDROID

    QFrame *frmConnect          = a_widget->findChild<QFrame        *>(FRM_CONNECT);        Q_ASSERT(frmConnect);
    QFrame *frmInfo             = a_widget->findChild<QFrame        *>(FRM_INFO);           Q_ASSERT(frmInfo);
    QFrame *frmStatus           = a_widget->findChild<QFrame        *>(FRM_STATUS);         Q_ASSERT(frmStatus);
    QPushButton *btnChangeServer = a_widget->findChild<QPushButton  *>(BTN_CHANGE_SERVER);  Q_ASSERT(btnChangeServer);
    QLabel *lblActualServer     = a_widget->findChild<QLabel        *>(LBL_ACTUAL_SERVER);  Q_ASSERT(lblActualServer);
    QPushButton *btnConnect     = a_widget->findChild<QPushButton   *>(BTN_CONNECT);        Q_ASSERT(btnConnect);

    QLabel *lblLoginTime        = a_widget->findChild<QLabel        *>(LBL_LOGIN_TIME);     Q_ASSERT(lblLoginTime);
    QLabel *lblTimeConnect      = a_widget->findChild<QLabel        *>(LBL_TIME_CONNECT);   Q_ASSERT(lblTimeConnect);
    QLabel *lblPacketsRec       = a_widget->findChild<QLabel        *>(LBL_PACKETS_REC);    Q_ASSERT(lblPacketsRec);
    QLabel *lblPacetsSent       = a_widget->findChild<QLabel        *>(LBL_PACKETS_SENT);   Q_ASSERT(lblPacetsSent);

    QCheckBox *cbbAuth          = a_widget->findChild<QCheckBox     *>(CBB_AUTH);           Q_ASSERT(cbbAuth);
    QCheckBox *cbbStream        = a_widget->findChild<QCheckBox     *>(CBB_STREAM);         Q_ASSERT(cbbStream);
    QCheckBox *cbbNetwork       = a_widget->findChild<QCheckBox     *>(CBB_NETWORK);        Q_ASSERT(cbbNetwork);


    //To test appearance
    cbbAuth->setChecked(true);
    //This is done in order to remove the reaction to a click on the checkbox
    connect(cbbAuth,&QCheckBox::clicked,[=]{
        cbbAuth->setChecked(!cbbAuth->isChecked());
    });
    connect(cbbStream,&QCheckBox::clicked,[=]{
        cbbStream->setChecked(!cbbStream->isChecked());
    });
    connect(cbbNetwork,&QCheckBox::clicked,[=]{
        cbbNetwork->setChecked(!cbbNetwork->isChecked());
    });
    //========================================================================
    btnChangeServer->setGraphicsEffect(new StyledDropShadowEffect(btnChangeServer));
    frmConnect->setGraphicsEffect(new StyledDropShadowEffect(frmConnect));
    frmInfo->setGraphicsEffect(new StyledDropShadowEffect(frmInfo));
    frmStatus->setGraphicsEffect(new StyledDropShadowEffect(frmStatus));
#else
    CustomComboBox *cbbServer        = a_widget->findChild<CustomComboBox*>(CBB_SERVER)        ; Q_ASSERT(cbbServer       );
    QLabel         *lblStatusMessage = a_widget->findChild<QLabel        *>(LBL_STATUS_MESSAGE); Q_ASSERT(lblStatusMessage);
    QPushButton    *btnSwitch        = a_widget->findChild<QPushButton   *>(BTN_SWITCH)        ; Q_ASSERT(btnSwitch       );

    connect(btnSwitch, &QPushButton::clicked, [this]{
        emit connectionSwitched();
    });


    if (m_serversModel)
        cbbServer->setModel(m_serversModel);
    else
    {
        m_serversModel = cbbServer->model();

        for (DapServerInfo& server :DapDataLocal::me()->servers())
            cbbServer->addItem(server.name);

        connect(DapDataLocal::me(), &DapDataLocal::serverAdded, [cbbServer](const DapServerInfo& a_serverInfo){
            cbbServer->addItem(a_serverInfo.name);
        });

        connect(DapDataLocal::me(), SIGNAL(serversCleared()), cbbServer, SLOT(clear()));
    }

    cbbServer->QComboBox::setCurrentText(DapDataLocal::me()->serverName());

//    QIcon icon(":/pics/flag.svg");
//    cbbServer->setItemIcon(0,icon);

//    cbbServer->addItem("Natherlans");
//    QIcon icon(":/pics/flag.svg");
//    cbbServer->setItemIcon(0,icon);

#endif

}

QString MainScreen::statusText(ConnectionStates a_state)
{
    switch (a_state)
    {
        case ConnectionStates::Disconnected:
            return  "Not connected";
        case ConnectionStates::Connecting:
            return "Connecting...";
        case ConnectionStates::Connected:
            return  "Connected";
        case ConnectionStates::Disconnecting:
            return  "Server down";
    }
    return QString();
}
