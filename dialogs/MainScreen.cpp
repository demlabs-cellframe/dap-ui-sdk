#include "MainScreen.h"
#include "StyledDropShadowEffect.h"
#include <QTimer>
#include "CustomComboBoxPopup.h"

#include "defines.h"
#include "DapDataLocal.h"

const QString MainScreen::SCREEN_NAME = "Main";

MainScreen::MainScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::MainScreen>();
}

QString MainScreen::screenName()
{
    return MainScreen::SCREEN_NAME;
}

void MainScreen::setState(ConnectionStates a_state)
{
    this->setChildProperties(LBL_STATUS_MESSAGE, Properties::TEXT , statusText(a_state));
    this->setChildProperties(LBL_STATUS_MESSAGE, Properties::STATE, a_state);
    this->updateChildStyle  (LBL_STATUS_MESSAGE);

    this->setChildProperties(BTN_SWITCH, Properties::STATE, a_state);
    this->updateChildStyle  (BTN_SWITCH);

    this->setChildProperties(CBB_SERVER, Properties::ENABLED, a_state == ConnectionStates::Disconnected);
    this->setChildProperties(BTN_SWITCH, Properties::ENABLED, (a_state == ConnectionStates::Disconnected || a_state == ConnectionStates::Connected));
}

void MainScreen::initVariantUi(QWidget *a_widget)
{
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

    connect(cbbServer,&CustomComboBox::showCustomWindow,[=]
    {
        qDebug() << "server clicked";

//        QList<DataModel> *dataList = new QList<DataModel>;
//        DataModel tmpModel;
//        for(int i = 0; i < 5;i++)
//        {
//            tmpModel.text = "kelvin-testnet.Cellframe";
//            tmpModel.iconPath =":/pics/flag.svg";
//            dataList->append(tmpModel);
//        }

        CustomComboBoxPopup *s_comboBoxPopup = new CustomComboBoxPopup(a_widget);
        s_comboBoxPopup->setModel(cbbServer->model());
        s_comboBoxPopup->setCaption("Choose server");
        s_comboBoxPopup->show();

        connect(s_comboBoxPopup, &CustomComboBoxPopup::itemSelected, [this, cbbServer](int a_index){
            QString serverName = cbbServer->itemText(a_index);
            cbbServer->setCurrentText(serverName);
            emit this->serverChanged(serverName);
        });

    });
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
