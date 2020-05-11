#ifndef ACCOUNTSCREEN_H
#define ACCOUNTSCREEN_H

#include <QLabel>
#include <QPushButton>
#include "StyledDropShadowEffect.h"

#include "DapUiScreen.h"

#include "ui_AccountScreen.h"
#include "controls/CustomPlacementButton.h"

class AccountScreen : public DapUiScreen
{
    Q_OBJECT

public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    AccountScreen(QObject * a_parent, QStackedWidget * a_sw);

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    virtual void initUi(QWidget * a_w, ScreenRotation a_rotation) override;
};

#endif // STARTSCREEN_H
