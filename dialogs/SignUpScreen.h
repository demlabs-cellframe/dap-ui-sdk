#ifndef SIGNUPSCREEN_H
#define SIGNUPSCREEN_H

#include <QCheckBox>
#include "DapUiScreen.h"
#include "StyledDropShadowEffect.h"

class SignUpScreen : public DapUiScreen
{
    Q_OBJECT
public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    SignUpScreen(QObject * a_parent, QStackedWidget * a_sw);

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    virtual void initUi(QWidget * a_w, ScreenRotation a_rotation) override;
};
#endif // SIGNUPSCREEN_H
