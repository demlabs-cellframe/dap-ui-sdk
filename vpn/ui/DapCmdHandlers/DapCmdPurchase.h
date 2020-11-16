#ifndef DAPCMDPURCHASE_H
#define DAPCMDPURCHASE_H

#include "DapCmdClientAbstract.h"

// Реализует отправку на CDB запроса на проверку покупки по Sku товара и purchase token
// и обработку ответа о результатах проверки
// Если покупка прошла проверку, мы получим ключ
class DapCmdPurchase : public DapCmdClientAbstract
{
    Q_OBJECT
public:
    explicit DapCmdPurchase(QObject *parent = nullptr);
    // запросить проверку покупки
    // productId - это Sku - у нас это: "android.demlabs.kelvpn_6", или "android.demlabs.kelvpn_12" или "android.demlabs.kelvpn_1"
    // token - это токен покупки от Google Play
    void requestVerify(const QString &packageName, const QString& productId, const QString& token);

protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;

signals:
    void purchaseVerified(const QString& key);//TODO
};

#endif // DAPCMDPURCHASE_H
