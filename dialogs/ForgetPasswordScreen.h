#ifndef FORGETPASSWORDSCREEN_H
#define FORGETPASSWORDSCREEN_H

#include "DapUiScreen.h"

class ForgetPasswordScreen : public DapUiScreen
{
    Q_OBJECT
public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    ForgetPasswordScreen(QObject * a_parent, QStackedWidget * a_sw);

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    virtual void initUi(QWidget * a_w, ScreenRotation a_rotation) override;
};
#endif // FORGETPASSWORDSCREEN_H
