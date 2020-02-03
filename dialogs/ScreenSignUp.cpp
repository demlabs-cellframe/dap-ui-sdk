#include "ScreenSignUp.h"

/// Overloaded constructor.
/// @param a_parent Parent.
/// @param a_sw Application window stack.
ScreenSignUp::ScreenSignUp(QObject * a_parent, QStackedWidget * a_sw)
    :DapUiScreen(a_parent,a_sw)
{
    
#ifdef DAP_PLATFORM_MOBILE
    // TODO: creating mobile version forms.
#else
    // Creating a window.
    create<Ui::SignUp>();
#endif
    
}

/// Form initialization.
/// @param a_w Window GUI widget.
/// @param a_rotation Device display orientation.
void ScreenSignUp::initUi(QWidget * a_w,ScreenRotation a_rotation)
{
    Q_UNUSED(a_rotation);
    
    // Initializing pointers to window widgets
    btBack = a_w->findChild<QPushButton*>("btBack");
    edMail = a_w->findChild<QLineEdit*>("edMail");
    edPassword = a_w->findChild<QLineEdit*>("edPassword");
    btShow = a_w->findChild<QPushButton*>("btShow");
    edConfPassword = a_w->findChild<QLineEdit*>("edConfPassword");
    btConfShow = a_w->findChild<QPushButton*>("btConfShow");
    btCreate = a_w->findChild<QPushButton*>("btCreate");
    
    Q_ASSERT(btBack);
    Q_ASSERT(edMail);
    Q_ASSERT(edPassword);
    Q_ASSERT(btShow);
    Q_ASSERT(edConfPassword);
    Q_ASSERT(btConfShow);
    Q_ASSERT(btCreate);
    
    // Signal-slot connection that changes the display of the password depending on the button state.
    connect(btShow, &QPushButton::clicked, [=] 
    {
        edPassword->setEchoMode(btShow->isChecked() ? QLineEdit::Normal : QLineEdit::Password);
    });
    // Signal-slot connection that changes the display of the password depending on the button state.
    connect(btConfShow, &QPushButton::clicked, [=] 
    {
        edConfPassword->setEchoMode(btConfShow->isChecked() ? QLineEdit::Normal : QLineEdit::Password);
    });
    
    // Signal-slot connection performing field filling check: if the fields are not filled in or the 
    // passwords entered by the user do not match, then emit an error signal, otherwise emit a signal 
    // to continue performing the required action.
    connect(btCreate, &QPushButton::clicked, [=] 
    {
        if(!checkField())
            emit sigFieldError("Fields are not filled or have the wrong value"); 
        else if(edPassword->text() != edConfPassword->text())
            emit sigPasswordMismatch("Passwords must match"); 
        else
            emit sigCreateAccount(edMail->text(), edPassword->text());
    });

}

/// Check field filling.
/// @return Returns true - if the fields are correct, 
/// false - if the fields are incorrect.
bool ScreenSignUp::checkField() const
{
    return edMail->text().isEmpty() || edMail->text().isNull() || 
            edPassword->text().isEmpty() || edPassword->text().isNull() ||
            edConfPassword->text().isEmpty() || edConfPassword->text().isNull() 
             ? false : true;
}
