#ifndef SCREENMESSAGEWIDGET_H
#define SCREENMESSAGEWIDGET_H

#include <QWidget>
#include "CustomPopup.h"
#include "ui_ScreenMessage.h"

class ScreenMessage: public CustomPopup
{
    Q_OBJECT
public:
    ScreenMessage(QWidget *parent = nullptr);

    void showPopup();

    virtual void setCaption(const QString& a_caption);

signals:
    void deleteSerialKey();
protected:
    CustomPopup *m_popup = nullptr;
    virtual void initVariantUi(QWidget * a_widget) override;
};

#endif // SCREENMESSAGEWIDGET_H
