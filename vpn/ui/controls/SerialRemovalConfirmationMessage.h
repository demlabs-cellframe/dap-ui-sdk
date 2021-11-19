#ifndef SCREENMESSAGEWIDGET_H
#define SCREENMESSAGEWIDGET_H

#include "CustomMessageBox.h"

class SerialRemovalConfirmationMessage: public CustomMessageBox
{
    Q_OBJECT

public:
    SerialRemovalConfirmationMessage(QWidget *a_parent = nullptr);

    virtual void doEscapeAction() override;
    virtual void doDefaultAction() override;

protected:
    virtual void closeEvent(QCloseEvent *) override;
};

#endif // SCREENMESSAGEWIDGET_H
