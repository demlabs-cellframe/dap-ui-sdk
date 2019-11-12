/****************************************************************************
**
** This file is part of the DapVpn project.
** 
** The class implements allows to interact with the graphical interface of 
** the window forgot password.
**
****************************************************************************/

#ifndef SCREENFORGOTPASSWORD_H
#define SCREENFORGOTPASSWORD_H

#include "DapUiScreen.h"

#ifdef DAP_PLATFORM_DESKTOP
    #include "ui_forgot_password_desktop.h"
    #include "ui_forgot_password_desktop_big.h"
    #include "ui_forgot_password_desktop_small.h"
#else
    #include "ui_forgot_password_desktop.h"
    #include "ui_forgot_password_desktop_big.h"
    #include "ui_forgot_password_desktop_small.h"
    // TODO: include header files for mobile version forms.
#endif

class ScreenForgotPassword : public DapUiScreen
{
    Q_OBJECT
    
    /// Pointer to edit field "Login".
    QLineEdit       *edMail {nullptr};
    /// Pointer to the "Send reset instuctions" button.
    QPushButton     *btSend {nullptr};
    
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
    ScreenForgotPassword(QObject *a_parent, QStackedWidget *a_sw);
    
signals:
    /// The signal is emitted if the input fields are incorrect or empty.
    /// @param a_message Error message.
    void sigFieldError(const QString &a_message);
    /// The signal is emitted when all checks are passed and it is 
    /// possible to continue the password recovery procedure.
    ///  @param a_mail E-mail address.
    void sigSendResetInstructions(const QString &a_mail);
};

#endif // SCREENFORGOTPASSWORD_H
