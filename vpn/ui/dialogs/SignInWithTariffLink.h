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

<<<<<<< HEAD
    void appendTariff(const QList<TariffItem> &a_tariffList, std::function<void (int)> func);
    void appendTariff(const QList<TariffItem> &a_tariffList);
signals:
    void tarriffScreenPopupOpened();

=======
    // установит тарифы и соединит сигнал выбора тарифа так, чтобы открывать URL
    void appendTariff(const QList<TariffItem> &a_tariffList);
    // установит тарифы но сигнал выбора тарифа перенаправил в func (для in-app purchasing)
    void appendTariff(const QList<TariffItem> &a_tariffList, std::function<void (int)> func);
>>>>>>> 0846bd932df68fbe370b84fc36075a8d1a344c44
protected:
    virtual void initVariantUi(QWidget *a_widget) override;

    virtual QList<CustomPopup *> customPopups() override;

};

#endif // SIGNINWITHTARIFFLINK_H
