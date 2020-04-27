#include "MainScreen.h"
#include "Utils.h"


const QString MainScreen::SCREEN_NAME = "Main";

MainScreen::MainScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    this->setObjectName(this->screenName());
    create<Ui::MainScreen>();

    AdaptiveScreen::initScreen(this);
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
#endif
}

void MainScreen::initVariantUi(QWidget *a_widget)
{
    this->connectBtnToSignall(BTN_CONNECTION, &MainScreen::disconnect, a_widget);

    QCheckBox *chbAuthorized    ; Utils::findChild(a_widget, CHB_AUTHORIZED     , chbAuthorized);
    QCheckBox *chbStreamOpened  ; Utils::findChild(a_widget, CHB_STREAM_OPENED  , chbStreamOpened);
    QCheckBox *chbVirtualNetwork; Utils::findChild(a_widget, CHB_VIRTUAL_NETWORK, chbVirtualNetwork);


#ifdef Q_OS_ANDROID

    QFrame *frmConnect          = a_widget->findChild<QFrame        *>(FRM_CONNECT);        Q_ASSERT(frmConnect);
    QFrame *frmInfo             = a_widget->findChild<QFrame        *>(FRM_INFO);           Q_ASSERT(frmInfo);
    QFrame *frmStatus           = a_widget->findChild<QFrame        *>(FRM_STATUS);         Q_ASSERT(frmStatus);
    QPushButton *btnChangeServer = a_widget->findChild<QPushButton  *>(BTN_CHANGE_SERVER);  Q_ASSERT(btnChangeServer);
    QLabel *lblActualServer     = a_widget->findChild<QLabel        *>(LBL_ACTUAL_SERVER);  Q_ASSERT(lblActualServer);

    QLabel *lblLoginTime        = a_widget->findChild<QLabel        *>(LBL_LOGIN_TIME);     Q_ASSERT(lblLoginTime);
    QLabel *lblTimeConnect      = a_widget->findChild<QLabel        *>(LBL_TIME_CONNECT);   Q_ASSERT(lblTimeConnect);
    QLabel *lblPacketsRec       = a_widget->findChild<QLabel        *>(LBL_PACKETS_REC);    Q_ASSERT(lblPacketsRec);
    QLabel *lblPacetsSent       = a_widget->findChild<QLabel        *>(LBL_PACKETS_SENT);   Q_ASSERT(lblPacetsSent);



    //To test appearance
    chbAuthorized->setChecked(true);
    chbStreamOpened->setChecked(true);
    chbVirtualNetwork->setChecked(true);


    //========================================================================
    btnChangeServer->setGraphicsEffect(new StyledDropShadowEffect(btnChangeServer));
    frmConnect->setGraphicsEffect(new StyledDropShadowEffect(frmConnect));
    frmInfo->setGraphicsEffect(new StyledDropShadowEffect(frmInfo));
    frmStatus->setGraphicsEffect(new StyledDropShadowEffect(frmStatus));
#else
    QComboBox *cbbServer            = a_widget->findChild<QComboBox*>(CBB_SERVER);          Q_ASSERT(cbbServer  );
    QPushButton *btnBytes           = a_widget->findChild<QPushButton*>(BTN_BYTES);         Q_ASSERT(btnBytes  );
    QPushButton *btnPackets         = a_widget->findChild<QPushButton*>(BTN_PACKETS);       Q_ASSERT(btnPackets  );
    QLabel *lblBytesPacketsCaption  = a_widget->findChild<QLabel*>(LBL_BYTES_PACKETS_CAPTION); Q_ASSERT(lblBytesPacketsCaption  );
    QLabel *lblDownloadSpeed        = a_widget->findChild<QLabel*>(LBL_DOWNLOAD_SPEED);     Q_ASSERT(lblDownloadSpeed  );
    QLabel *lblTimeConnected        = a_widget->findChild<QLabel*>(LBL_TIME_CONNECTED);     Q_ASSERT(lblTimeConnected  );
    QLabel *lblDownload             = a_widget->findChild<QLabel*>(LBL_DOWNLOAD);           Q_ASSERT(lblDownload  );
    QLabel *lblBytesPackets         = a_widget->findChild<QLabel*>(LBL_PATES_PACKETS);      Q_ASSERT(lblBytesPackets  );
    QLabel *lblStatusMessage        = a_widget->findChild<QLabel*>(LBL_STATUS_MESSAGE);     Q_ASSERT(lblStatusMessage  );


    connect(btnBytes,&QPushButton::clicked,[=]{
        btnBytes->setChecked(true);
        btnPackets->setChecked(false);
        lblBytesPacketsCaption->setText("Bytes Received");
    });
    connect(btnPackets,&QPushButton::clicked,[=]{
        btnBytes->setChecked(false);
        btnPackets->setChecked(true);
        lblBytesPacketsCaption->setText("Packets Received");
    });
#endif

}

void MainScreen::setAuthorized(bool a_authorized /*= true*/)
{
    this->setChildProperties(CHB_AUTHORIZED, Properties::CHECKED, a_authorized);
}

void MainScreen::setStreamOpened(bool a_streamOpened /*= true*/)
{
    this->setChildProperties(CHB_STREAM_OPENED, Properties::CHECKED, a_streamOpened);
}

void MainScreen::setVirtualNetwork(bool a_virtualNetwork /*= true*/)
{
    this->setChildProperties(CHB_VIRTUAL_NETWORK, Properties::CHECKED, a_virtualNetwork);
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
