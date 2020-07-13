#ifndef BUGREPORTWITHOUTEMAILSCREEN_H
#define BUGREPORTWITHOUTEMAILSCREEN_H

#include <QStateMachine>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QRegExpValidator>
#include <QMovie>

#include "AdaptiveScreen.h"
#include "vpnDefine.h"
#include "CustomLineHeightLabel.h"
#include "defines.h"
#include "StyledDropShadowEffect.h"
#include "CustomLineEdit.h"
#include "CustomTextEdit.h"

#include "BugReportScreenBase.h"

class BugReportWithoutEmailScreen : public BugReportScreenBase
{
    Q_OBJECT

public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    BugReportWithoutEmailScreen(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;
};



#endif // BUGREPORTWITHOUTEMAILSCREEN_H
