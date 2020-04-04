#include "PasswordRecoveryScreen.h"


const QString PasswordRecoveryScreen::SCREEN_NAME = "PasswordRecovery";

PasswordRecoveryScreen::PasswordRecoveryScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::PasswordRecoveryScreen>();
}

QString PasswordRecoveryScreen::screenName()
{
    return SCREEN_NAME;
}

void PasswordRecoveryScreen::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)


        QPushButton             *btnSendMail = a_widget->findChild<QPushButton*>(BTN_SEND_MAIL);                               Q_ASSERT(btnSendMail);
        CustomLineHeightLabel   *lblStatusMessage = a_widget->findChild<CustomLineHeightLabel*>(LBL_STATUS_MESSAGE);           Q_ASSERT(lblStatusMessage);
        QLineEdit               *edtEmail = a_widget->findChild<QLineEdit*>(EDT_EMAIL);                                        Q_ASSERT(edtEmail);
        QLabel                  *lblEmailError = a_widget->findChild<QLabel*>(LBL_EMAIL_ERROR);                                Q_ASSERT(lblEmailError);
        QLabel                  *lblCaption = a_widget->findChild<QLabel*>(LBL_CAPTION);                                       Q_ASSERT(lblCaption);

#ifdef Q_OS_ANDROID
        QVBoxLayout             *vltMainLayout = a_widget->findChild<QVBoxLayout*>(VLT_MAIN_LAYOUT);                           Q_ASSERT(vltMainLayout);
        QLabel                  *lblLogo = a_widget->findChild<QLabel*>(LBL_LOGO);                                             Q_ASSERT(lblLogo);
        QWidget                 *wgtMarginBottomMessage = a_widget->findChild<QWidget*>(WGT_MARGIN_BOTTOM_MESSAGE);            Q_ASSERT(wgtMarginBottomMessage);

        vltMainLayout->setAlignment(lblLogo,Qt::AlignBaseline);
        vltMainLayout->setAlignment(btnSendMail,Qt::AlignBaseline);
        vltMainLayout->setAlignment(lblCaption,Qt::AlignBaseline);
        vltMainLayout->setAlignment(lblStatusMessage,Qt::AlignBaseline);
        vltMainLayout->setAlignment(edtEmail,Qt::AlignBaseline);
        vltMainLayout->setAlignment(lblEmailError,Qt::AlignBaseline);

        vltMainLayout->insertWidget(5,lblEmailError);
        vltMainLayout->insertWidget(7,edtEmail);
        lblEmailError->setAlignment(Qt::AlignLeft);
        lblCaption->setText("Forget password");
        lblCaption->setAlignment(Qt::AlignJustify);
        lblStatusMessage->setAlignment(Qt::AlignJustify);
        edtEmail->setAlignment(Qt::AlignLeft);
        edtEmail->setPlaceholderText("e-mail");
        lblStatusMessage->setText("Please enter your email address.<br>We will send you an email to reset your password.");
#else


        edtEmail->setPlaceholderText("Your email");
        btnSendMail->setGraphicsEffect(new StyledDropShadowEffect(btnSendMail));
        lblStatusMessage->setText("Please enter your email address.<br>We will send you an email to<br>reset your password.");
#endif

        static int presBtn = 0;


        lblEmailError->setVisible(false);


        connect(btnSendMail,&QPushButton::clicked,[=]{
            switch (presBtn%3)
            {
            case 0:
                presBtn++;
                lblEmailError->setVisible(true);

#ifdef Q_OS_ANDROID
                Utils::setPropertyAndUpdateStyle(wgtMarginBottomMessage,Properties::STATE,"1");
#else
                Utils::setPropertyAndUpdateStyle(edtEmail,Properties::STATE,"1");
#endif
                break;
            case 1:
#ifdef Q_OS_ANDROID
                Utils::setPropertyAndUpdateStyle(wgtMarginBottomMessage,Properties::STATE,"2");
#else
                Utils::setPropertyAndUpdateStyle(lblStatusMessage,Properties::STATE,"1");
#endif
                lblStatusMessage->setText("Email sent. And other information in this paragraph.");
                lblCaption->setText("Check email");

                edtEmail->setVisible(false);
                lblEmailError->setVisible(false);
                btnSendMail->setText("Back");
                presBtn++;
                break;
            case 2:
                Utils::setPropertyAndUpdateStyle(lblStatusMessage,Properties::STATE,"0");
#ifdef Q_OS_ANDROID
                Utils::setPropertyAndUpdateStyle(wgtMarginBottomMessage,Properties::STATE,"0");
#else
                Utils::setPropertyAndUpdateStyle(edtEmail,Properties::STATE,"0");
#endif

                lblStatusMessage->setText("Please enter your email address.<br>We will send you an email to reset your password.");
                lblCaption->setText("Forget password");

                edtEmail->setVisible(true);
                btnSendMail->setText("Send email");
                presBtn++;
                break;
            }

        });
}
