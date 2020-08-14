#ifndef BUGREPORTSCREENBASE_H
#define BUGREPORTSCREENBASE_H

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

#include "ui_BugReportScreen.h"

#ifndef BUG_REPORT_FORM
    #define BUG_REPORT_FORM Ui::BugReportScreen
#endif

#define MAX_SIZE_MESSAGE 200

class BugReportScreenBase : public AdaptiveScreen
{
    Q_OBJECT

public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    BugReportScreenBase(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override = 0;

    QString message() const;
public slots:
    void setMessage(const QString &a_message);
    void checkFieldsAndSendReport();

signals:

    void wrongMessage();
    void messageChanged(const QString &message);

    void sendRequested();

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;

protected:

    bool checkMessage();

    virtual bool checkFields();

    QScopedPointer<BUG_REPORT_FORM> m_ui;

private:
    bool validateText(QString &str);
};



#endif // BUGREPORTSCREENBASE_H
