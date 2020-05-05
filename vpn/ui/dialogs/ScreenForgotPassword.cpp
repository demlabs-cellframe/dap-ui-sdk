#include "ScreenForgotPassword.h"

#include "controls/StyledDropShadowEffect.h"

/// Overloaded constructor.
/// @param a_parent Parent.
/// @param a_sw Application window stack.
ScreenForgotPassword::ScreenForgotPassword(QObject * a_parent, QStackedWidget * a_sw)
    :DapUiScreen(a_parent,a_sw)
{
    
#ifdef DAP_PLATFORM_MOBILE
    // TODO: creating mobile version forms.
#else
    // Creating a window.
    create<Ui::ForgotPassword>();
#endif
    
}

/// Form initialization.
/// @param a_w Window GUI widget.
/// @param a_rotation Device display orientation.
void ScreenForgotPassword::initUi(QWidget * a_w,ScreenRotation a_rotation)
{
    Q_UNUSED(a_rotation);
    
    // Initializing pointers to window widgets
    edMail = a_w->findChild<QLineEdit*>("edMail");
    btSend = a_w->findChild<QPushButton*>("btSend");
    
    Q_ASSERT(edMail);
    Q_ASSERT(btSend);
    
    btSend->setGraphicsEffect(new StyledDropShadowEffect(btSend));

    // Signal-slot connection performing field filling check: if the fields are not filled,
    // then emit an error signal, otherwise emit a signal to continue performing the required action.
    connect(btSend, &QPushButton::clicked, [=]
    {
        if(!checkField())
            emit sigFieldError("Fields are not filled or have the wrong value");
        else
            emit sigSendResetInstructions(edMail->text());
    });
}

/// Check field filling.
/// @return Returns true - if the fields are correct, 
/// false - if the fields are incorrect.
bool ScreenForgotPassword::checkField() const
{
    return edMail->text().isEmpty() || edMail->text().isNull() ? false : true;
}
