#ifndef BUGREPORTSCREEN_H
#define BUGREPORTSCREEN_H

#include <QStateMachine>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

#include "AdaptiveScreen.h"
#include "vpnDefine.h"
#include "ui_BugReportScreen.h"
#include "CustomLineHeightLabel.h"
#include "CustomLineHeightTextEdit.h"
#include "defines.h"

#ifdef Q_OS_ANDROID

#else
#include "StyledDropShadowEffect.h"
#endif
class BugReportScreen : public AdaptiveScreen
{
    Q_OBJECT

public:

    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    BugReportScreen(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

public:

    void setState(ConnectionStates a_state);

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    virtual void initVariantUi(QWidget *a_widget) override;

    const QString LBL_STATUS_MESSAGE = "lblStatusMessage";
    const QString BTN_SEND = "btnSend";
    const QString EDT_EMAIL = "edtEmail";
    const QString EDT_MESSAGE = "edtMessage";
    const QString LBL_CAPTURE = "lblCapture";
    const QString VLT_BUG_REPORT = "vltBugReport";
};

#endif // BUGREPORTSCREEN_H
