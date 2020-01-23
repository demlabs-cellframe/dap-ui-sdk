/****************************************************************************
**
** This file is part of the DapVpn project.
** 
** The class implements allows to interact with the graphical interface of 
** the window Sign UP.
**
****************************************************************************/

#ifndef SCREENSIGNUP_H
#define SCREENSIGNUP_H

#include "DapUiScreen.h"

#ifdef DAP_PLATFORM_DESKTOP
    #include "ui_sign_up_desktop.h"
    #include "ui_sign_up_desktop_big.h"
    #include "ui_sign_up_desktop_small.h"
#else
    #include "ui_sign_up_desktop.h"
#endif

#include <QGraphicsDropShadowEffect>

class ScreenSignUp : public DapUiScreen
{
    Q_OBJECT
    
    /// Pointer to the back button.
    QPushButton     *btBack {nullptr};
    /// Pointer to edit field "Login".
    QLineEdit       *edMail {nullptr};
    /// Pointer to the edit field "Password".
    QLineEdit       *edPassword {nullptr};
    /// Pointer to the "Display password" button.
    QPushButton     *btShow {nullptr};
    /// Pointer to the "Confirm Password" edit box.
    QLineEdit       *edConfPassword {nullptr};
    /// Pointer to the "Display password" button.
    QPushButton     *btConfShow {nullptr};
    /// Pointer to the "Create account" button.
    QPushButton     *btCreate {nullptr};
    
protected:    
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    void initUi(QWidget * a_w,ScreenRotation a_rotation);
    /// Check field filling.
    /// @return Returns true - if the fields are correct, 
    /// false - if the fields are incorrect.
    inline bool checkField() const;
    
public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    ScreenSignUp(QObject *a_parent, QStackedWidget *a_sw);
    
signals:
    /// The signal is emitted if the input fields are incorrect or empty.
    /// @param a_message Error message.
    void sigFieldError(const QString &a_message);
    /// A signal is emitted if the entered passwords do not match.
    /// @param a_message Error message.
    void sigPasswordMismatch(const QString &a_message);
    /// The signal is emitted when all checks are passed and it is 
    /// possible to continue the registration procedure.
    /// @param a_mail E-mail address.
    /// @param a_password User password.
    void sigCreateAccount(const QString &a_mail, const QString &a_password);
};

//Q_DECLARE_METATYPE(ScreenSignUp);

#endif // SCREENSIGNUP_H
