#ifndef SIGNINWITHTARIFFLINK_H
#define SIGNINWITHTARIFFLINK_H

#include "SignInScreenSerialNumberBase.h"
#include "TariffDelegate.h"
#include "TariffItem.h"
#include <QDesktopServices>

class SignInWithTariffLink: public SignInScreenSerialNumberBase
{
    Q_OBJECT
public:
    SignInWithTariffLink(QWidget *a_parent = nullptr);
    void appendTariff(const QList<TariffItem> &a_tariffList);
#ifdef BUILD_VAR_GOOGLE
    void appendTariff(const QList<TariffItem> &a_tariffList, std::function<void (int)> func);
#endif
signals:
    void tarriffScreenPopupOpened();

protected:
    virtual void initVariantUi(QWidget *a_widget) override;

    virtual QList<CustomPopup *> customPopups() override;

};

#endif // SIGNINWITHTARIFFLINK_H
