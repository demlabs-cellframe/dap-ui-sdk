#include "PasswordRecoveryScreen.h"


const QString PasswordRecoveryScreen::SCREEN_NAME = "PasswordRecoveryScreen";

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

        QLabel                  *lblLogo = a_widget->findChild<QLabel*>(LBL_LOGO);                                             Q_ASSERT(lblLogo);
        QPushButton             *btnSendMail = a_widget->findChild<QPushButton*>(BTN_SEND_MAIL);                               Q_ASSERT(btnSendMail);
        CustomLineHeightLabel   *lblStatusMessage = a_widget->findChild<CustomLineHeightLabel*>(LBL_STATUS_MESSAGE);           Q_ASSERT(lblStatusMessage);
        QLineEdit               *edtEmail = a_widget->findChild<QLineEdit*>(EDT_EMAIL);                                        Q_ASSERT(edtEmail);
        QLabel                  *lblEmailError = a_widget->findChild<QLabel*>(LBL_EMAIL_ERROR);                                Q_ASSERT(lblEmailError);
        QVBoxLayout             *vltMainLayout = a_widget->findChild<QVBoxLayout*>(VLT_MAIN_LAYOUT);                           Q_ASSERT(vltMainLayout);

#ifdef Q_OS_ANDROID
        QWidget                 *wgtMarginBottomMessage = a_widget->findChild<QWidget*>(WGT_MARGIN_BOTTOM_MESSAGE);            Q_ASSERT(wgtMarginBottomMessage);
        QLabel                  *lblCaption = a_widget->findChild<QLabel*>(LBL_CAPTION);                                       Q_ASSERT(lblCaption);
        vltMainLayout->insertWidget(5,lblEmailError);
        vltMainLayout->insertWidget(7,edtEmail);
        lblEmailError->setAlignment(Qt::AlignLeft);
        lblCaption->setText("Forget password");
        lblCaption->setAlignment(Qt::AlignJustify);
        lblStatusMessage->setAlignment(Qt::AlignJustify);
        edtEmail->setAlignment(Qt::AlignLeft);

#else

        vltMainLayout->setAlignment(lblLogo,Qt::AlignHCenter);
        vltMainLayout->setAlignment(btnSendMail,Qt::AlignHCenter);

        vltMainLayout->setAlignment(lblStatusMessage,Qt::AlignHCenter);
        vltMainLayout->setAlignment(edtEmail,Qt::AlignHCenter);
        vltMainLayout->setAlignment(lblEmailError,Qt::AlignHCenter);
#endif

        static int presBtn = 0;

        edtEmail->setPlaceholderText("Your email");
        btnSendMail->setGraphicsEffect(new StyledDropShadowEffect(btnSendMail));
        lblEmailError->setVisible(false);
#ifdef Q_OS_ANDROID
        lblStatusMessage->setText("Please enter your email address.<br>We will send you an email to reset your password.");
#else
        lblStatusMessage->setText("Please enter your email address.<br>We will send you an email to<br>reset your password.");
#endif

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

                edtEmail->setVisible(true);
                btnSendMail->setText("Send email");
                presBtn++;
                break;
            }

        });
}
