#ifndef SCREENLOGIN_H
#define SCREENLOGIN_H

#include <QHash>
#include <QLineEdit>
#include <QtWidgets>

#include "DapUiScreen.h"
#include "DapDataLocal.h"
#include "ScreenSignUp.h"
#include "ScreenForgotPassword.h"
#include "controls/ComboBoxDelegate.h"

#ifdef DAP_PLATFORM_DESKTOP
    #include "ui_login_desktop.h"
    #include "ui_login_desktop_big.h"
    #include "ui_login_desktop_small.h"
#else
#include "ui_login_mobile_hor.h"
#include "ui_login_mobile_hor_big.h"
#include "ui_login_mobile_hor_small.h"
#include "ui_login_mobile_ver.h"
#include "ui_login_mobile_ver_big.h"
#include "ui_login_mobile_ver_small.h"
#endif

class ScreenLogin : public DapUiScreen
{
    Q_OBJECT
    
    enum Pages {
        Login,
        SignUpForgot
    };

    /// Pointer to edit field "Login".
    QLineEdit       *edMail {nullptr};
    /// Pointer to the edit field "Password".
    QLineEdit       *edPassword {nullptr};
    /// Pointer to the "Display password" button.
    QPushButton     *btShow {nullptr};
    /// Pointer to the drop-down list of available servers "Servers".
    QComboBox       *cbUpstream {nullptr};
    /// Pointer to the "Connect" button.
    QPushButton     *btLogin {nullptr};

    QStackedWidget  *m_swScreen {nullptr};
    
    QStackedWidget  *m_swSignUpForgot {nullptr};

    DapUiScreen     *m_screenSignUpForgot {nullptr};

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    void initUi(QWidget * a_w,ScreenRotation a_rotation);
    /// Check field filling.
    /// @return Returns true - if the fields are correct, 
    /// false - if the fields are incorrect.
    inline bool checkField() const;
    /// Check login and password for correct input.
    /// @return Returns true if the login or password is entered correctly, otherwise - false.
    inline bool checkUserData() const;

public slots:
    void reloadServers(void);
public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    ScreenLogin(QObject * a_parent, QStackedWidget * a_sw);

signals:
    /// The signal is emitted if the input fields are incorrect or empty.
    /// @param a_message Error message.
    void sigFieldError(const QString &a_message);
    /// The signal is emitted when all checks are passed and it is 
    /// possible to continue the registration procedure.
    /// @param a_mail E-mail address.
    /// @param a_password User password.
    void reqConnect(DapServerInfo&, const QString&, const QString&);
    void currentUpstreamNameChanged(QString);


};


//Q_DECLARE_METATYPE(ScreenLogin);

#endif // SCREENLOGIN_H
