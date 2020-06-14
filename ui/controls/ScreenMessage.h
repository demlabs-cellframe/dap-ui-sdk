#ifndef SCREENMESSAGEWIDGET_H
#define SCREENMESSAGEWIDGET_H

#include <QWidget>
#include "CustomPopup.h"
#include "ui_ScreenMessage.h"

class ScreenMessage: public CustomPopup
{
    Q_OBJECT

    QLabel *m_lblMessage = nullptr;
public:
    ScreenMessage(QWidget *parent = nullptr);

    virtual void setTextMessage(const QString& a_message);

signals:
    void deleteSerialKey();
protected:

    virtual void initVariantUi(QWidget * a_widget) override;

    const QString LBL_MESSAGE       = "lblMessage";
    const QString BTN_CENCEL         = "btnCencel";
    const QString BTN_DELETE         = "btnDelete";
};

#endif // SCREENMESSAGEWIDGET_H
