#ifndef BUGREPORTWITHEMAILSCREEN_H
#define BUGREPORTWITHEMAILSCREEN_H

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


class BugReportWithEmailScreen : public BugReportScreenBase
{
    Q_OBJECT

public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    BugReportWithEmailScreen(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

public slots:
    void setEmail   (const QString &a_email);

signals:

    void wrongEmail();
    void emailChanged(const QString &email);

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;

protected:

    bool checkEmail();

    virtual bool checkFields();
};



#endif // BUGREPORTWITHEMAILSCREEN_H
