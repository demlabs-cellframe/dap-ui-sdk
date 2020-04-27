#include "MainScreen.h"
#include "Utils.h"


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

#endif
}

void MainScreen::initVariantUi(QWidget *a_widget)
{
    QPushButton *btnConnection; Utils::findChild(a_widget, BTN_CONNECTION, btnConnection);
    connect(btnConnection, &QCheckBox::clicked, this, &MainScreen::disconnect);



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

    QCheckBox *cbbAuth          = a_widget->findChild<QCheckBox     *>(CBB_AUTH);           Q_ASSERT(cbbAuth);
    QCheckBox *cbbStream        = a_widget->findChild<QCheckBox     *>(CBB_STREAM);         Q_ASSERT(cbbStream);
    QCheckBox *cbbNetwork       = a_widget->findChild<QCheckBox     *>(CBB_NETWORK);        Q_ASSERT(cbbNetwork);


    //To test appearance
    cbbAuth->setChecked(true);
    cbbStream->setChecked(true);
    cbbNetwork->setChecked(true);

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
    QComboBox *cbbServer            = a_widget->findChild<QComboBox*>(CBB_SERVER);          Q_ASSERT(cbbServer  );
    QCheckBox *chbConnection        = a_widget->findChild<QCheckBox*>(CHB_CONNECTION );     Q_ASSERT(chbConnection  );
    QCheckBox *chbIPrequsted        = a_widget->findChild<QCheckBox*>(CHB_IP_PREQUESTED);   Q_ASSERT(chbIPrequsted  );
    QCheckBox *chbVirtualNetwork    = a_widget->findChild<QCheckBox*>(CHB_VIRTUAL_NETWORK); Q_ASSERT(chbVirtualNetwork  );
    QPushButton *btnBytes           = a_widget->findChild<QPushButton*>(BTN_BYTES);         Q_ASSERT(btnBytes  );
    QPushButton *btnPackets         = a_widget->findChild<QPushButton*>(BTN_PACKETS);       Q_ASSERT(btnPackets  );
    QLabel *lblBytesPacketsCaption  = a_widget->findChild<QLabel*>(LBL_BYTES_PACKETS_CAPTION); Q_ASSERT(lblBytesPacketsCaption  );
    QLabel *lblDownloadSpeed        = a_widget->findChild<QLabel*>(LBL_DOWNLOAD_SPEED);     Q_ASSERT(lblDownloadSpeed  );
    QLabel *lblTimeConnected        = a_widget->findChild<QLabel*>(LBL_TIME_CONNECTED);     Q_ASSERT(lblTimeConnected  );
    QLabel *lblDownload             = a_widget->findChild<QLabel*>(LBL_DOWNLOAD);           Q_ASSERT(lblDownload  );
    QLabel *lblBytesPackets         = a_widget->findChild<QLabel*>(LBL_PATES_PACKETS);      Q_ASSERT(lblBytesPackets  );
    QLabel *lblStatusMessage        = a_widget->findChild<QLabel*>(LBL_STATUS_MESSAGE);     Q_ASSERT(lblStatusMessage  );

    /*chbConnection->setChecked(true);
    chbIPrequsted->setChecked(true);
    chbVirtualNetwork->setChecked(true);*/

    for(int i = 0; i<5;i++)
    {
        cbbServer->addItem(QString("Server name%1").arg(i));
    }

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

    connect(this, &MainScreen::setChbConnection, [=]{
        chbConnection->setChecked(true);
    });
    connect(this, &MainScreen::setChbNotConnection, [=]{
        chbConnection->setChecked(false);
    });
    connect(this, &MainScreen::setChbIpRequest, [=]{
        chbIPrequsted->setChecked(true);
    });
    connect(this, &MainScreen::setChbNotIpRequest, [=]{
        chbIPrequsted->setChecked(false);
    });
    connect(this, &MainScreen::setChbVirtualNetwork, [=]{
        chbVirtualNetwork->setChecked(true);
    });
    connect(this, &MainScreen::setChbNotVirtualNetwork, [=]{
        chbVirtualNetwork->setChecked(false);
    });

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
