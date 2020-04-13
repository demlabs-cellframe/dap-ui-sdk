#include <QtDebug>
#include <QSettings>
#include <QApplication>

#include "ScreenLogin.h"

Q_DECLARE_METATYPE(DapServerInfo)

/**
 * @brief ScreenLogin::ScreenLogin
 * @param a_parent
 * @param a_sw
 */
ScreenLogin::ScreenLogin(QObject * a_parent, QStackedWidget * a_sw)
    :DapUiScreen(a_parent,a_sw)
{
    // Инициализация формы ( в зависимости от размера экрана)
#ifdef Q_OS_ANDROID
    create<Ui::LoginMobile>();
#else
    create<Ui::Login>();
#endif
/*
    QWidget * cbUpstreamHor=getWidget("cbUpstream",Hor);
    QWidget * cbUpstreamVer=getWidget("cbUpstream",Ver);
    QLineEdit * edPasswordHor=getWidgetCustom<QLineEdit>("edPassword",Hor);
    QLineEdit * edPasswordVer=getWidgetCustom<QLineEdit>("edPassword",Ver);
    Q_ASSERT(edPasswordHor);
    Q_ASSERT(edPasswordVer);
    Q_ASSERT(cbUpstreamHor);
    Q_ASSERT(cbUpstreamVer);
    // перекрёсное связывание выбора сервера
    // сервер выбранный в одном интерфейсе, окажется выбран и в другом
    connect(cbUpstreamHor, SIGNAL(currentIndexChanged(int))
            ,cbUpstreamVer, SLOT(setCurrentIndex(int)));
    connect(cbUpstreamVer, SIGNAL(currentIndexChanged(int))
            ,cbUpstreamHor, SLOT(setCurrentIndex(int)));

    // перекрёсное связывание паролей
    // пароль введённый в одном окне, появится и в другом
    connect( edPasswordHor,&QLineEdit::textChanged
            ,edPasswordVer,&QLineEdit::setText );
    connect(edPasswordVer ,&QLineEdit::textChanged
            ,edPasswordHor,&QLineEdit::setText );
*/
}


/// Form initialization.
/// @param a_w Window GUI widget.
/// @param a_rotation Device display orientation.

void ScreenLogin::initUi(QWidget * a_w,ScreenRotation a_rotation)
{
    Q_UNUSED(a_rotation);
    
    // Initializing pointers to window widgets
    edMail = a_w->findChild<QLineEdit*>("edMail");
    edPassword = a_w->findChild<QLineEdit*>("edPassword");
    btShow = a_w->findChild<QPushButton*>("btShow");
    cbUpstream = a_w->findChild<QComboBox*>("cbUpstream");
    btLogin = a_w->findChild<QPushButton*>("btLogin");


    Q_ASSERT(edMail);
    Q_ASSERT(edPassword);
    Q_ASSERT(btShow);
    Q_ASSERT(cbUpstream);
    Q_ASSERT(btLogin);

    // Turn on the scrolling display in the server combo box as needed 
    // (in our case, if the number of maximum permissible volumes of 
    // displayed items is exceeded).
    // fillUpstreams(a_w);

    reloadServers();

    cbUpstream->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //synchronize with DataLocal
    DapDataLocal::me()->connectComboBox(cbUpstream);
    cbUpstream->setCurrentText( DapDataLocal::me()->serverName() );

    connect(edPassword, &QLineEdit::returnPressed, btLogin, &QPushButton::click);
    // Signal-slot connection that changes the display of the password depending on the button state.
    connect(btShow, &QPushButton::clicked, [=] 
    {
        edPassword->setEchoMode(btShow->isChecked() ? QLineEdit::Normal : QLineEdit::Password);
    });

    // Signal-slot connection performing field filling check: if the fields are not filled, 
    // then emit an error signal, otherwise emit a signal to continue authorization of the user.
    connect(btLogin, &QPushButton::clicked, [=] {
        if(!checkField())
        {
            emit sigFieldError("Fields are not filled or have the wrong value");
        }
        else if(checkUserData())
        {
            emit sigFieldError("Incorrect login or password");
        }
        else if (!checkSrv()) {
            emit sigFieldError("No uplink nodes available");
        }
        else
        {
            DapServerInfo dsi = cbUpstream->currentData().value<DapServerInfo>();
            emit reqConnect(dsi, edMail->text(), edPassword->text());
        }
    });


    cbUpstream->setItemDelegate(new ComboBoxDelegate(cbUpstream, cbUpstream));

    QSettings settings;
    edMail->setText(settings.value("username").toString());
    edPassword->setText(settings.value("password").toString());

}

/**
 * @brief ScreenLogin::reloadServers
 */
void ScreenLogin::reloadServers(void)
{
    int sepIndex = 0;
    cbUpstream->clear();
    for(const DapServerInfo& i: DapDataLocal::me()->servers()) {
        cbUpstream->addItem(QIcon(DapDataLocal::me()->locationToIconPath(i.location)),
                            i.name , QVariant::fromValue(i));
        if (sepIndex != 0)
            cbUpstream->insertSeparator(sepIndex*2-1);
        sepIndex++;
    }
}
/// Check field filling.
/// @return Returns true - if the fields are correct, 
/// false - if the fields are incorrect.
bool ScreenLogin::checkField()
{
    edMail = getWidgetCustom<QLineEdit>("edMail", rotation());
    edPassword = getWidgetCustom<QLineEdit>("edPassword", rotation());

    qDebug()<<"checkFields" << edMail->text() << "; " << edMail->text();
    return edMail->text().isEmpty() || edMail->text().isNull() || 
            edPassword->text().isEmpty() || edPassword->text().isNull()
             ? false : true;
}

/// Check login and password for correct input.
/// @return Returns true if the login or password is entered correctly, otherwise - false.
bool ScreenLogin::checkUserData() const
{
    return edMail->text().contains(' ') || edPassword->text().contains(' ');
}

bool ScreenLogin::checkSrv() const {
    return cbUpstream->currentText().isEmpty() ||
            cbUpstream->currentText().isNull() ? false : true;
}
