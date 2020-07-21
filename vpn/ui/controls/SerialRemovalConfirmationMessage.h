#ifndef SCREENMESSAGEWIDGET_H
#define SCREENMESSAGEWIDGET_H

#include <QWidget>
#include "CustomPopup.h"
#include "ui_ScreenMessage.h"

class SerialRemovalConfirmationMessage: public CustomPopup
{
    Q_OBJECT

    QLabel *m_lblMessage = nullptr;
public:
    SerialRemovalConfirmationMessage(QWidget *parent = nullptr);

    virtual void setTextMessage(const QString& a_message);

signals:
    void confirmed();
protected:

    virtual void initVariantUi(QWidget * a_widget) override;

    const QString LBL_MESSAGE   = "lblMessage";
    const QString BTN_CENCEL    = "btnCencel";
    const QString BTN_DELETE    = "btnDelete";
};

#endif // SCREENMESSAGEWIDGET_H
