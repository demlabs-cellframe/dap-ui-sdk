#include "SignUpScreen.h"

#include "ui_SignUpScreen.h"


SignUpScreen::SignUpScreen(QObject * a_parent, QStackedWidget * a_sw)
    : DapUiScreen(a_parent, a_sw)
{
    create<Ui::SignUpScreen>();
}

void SignUpScreen::initUi(QWidget *a_w, DapUiScreen::ScreenRotation a_rotation)
{
    Q_UNUSED(a_rotation)

    QPushButton *btnSignUp = a_w->findChild<QPushButton*>("btnSignUp");
    QLineEdit *edtEmail = a_w->findChild<QLineEdit*>("edtEmail");
    QLineEdit *edtPassword = a_w->findChild<QLineEdit*>("edtPassword");
    QLineEdit *edtRptPassword = a_w->findChild<QLineEdit*>("edtRptPassword");
    QCheckBox *cbInfo= a_w->findChild<QCheckBox*>("cbInfo");
    QLabel *lblInfo= a_w->findChild<QLabel*>("lblInfo");
    QLabel *lblRptPasswordError= a_w->findChild<QLabel*>("lblRptPasswordError");
    QLabel *lblPasswordError= a_w->findChild<QLabel*>("lblPasswordError");
    QLabel *lblEmailError= a_w->findChild<QLabel*>("lblEmailError");

    Q_ASSERT(btnSignUp);
    Q_ASSERT(edtEmail);
    Q_ASSERT(edtPassword);
    Q_ASSERT(edtRptPassword);
    Q_ASSERT(cbInfo);
    Q_ASSERT(lblInfo);
    Q_ASSERT(lblRptPasswordError);
    Q_ASSERT(lblPasswordError);
    Q_ASSERT(lblEmailError);

    btnSignUp->setGraphicsEffect(new StyledDropShadowEffect(btnSignUp));

    edtEmail->setPlaceholderText("Email");
    edtPassword->setPlaceholderText("Password");
    edtPassword->setEchoMode(QLineEdit::Password);
    edtRptPassword->setPlaceholderText("Repet password");
    edtRptPassword->setEchoMode(QLineEdit::Password);


    lblInfo->setText("<p>I argee to the <span style='color:#D61F5D;'>terms</span> and <span style='color:#D61F5D;'>privacy policy</span></p>");

    connect(btnSignUp,&QPushButton::clicked,[=]{
        lblEmailError->setMaximumWidth(1000);
        lblPasswordError->setMaximumWidth(1000);
        lblRptPasswordError->setMaximumWidth(1000);
    });
}
