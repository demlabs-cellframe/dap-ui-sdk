#ifndef DAPCMDPURCHASE_H
#define DAPCMDPURCHASE_H

#include "DapCmdClientAbstract.h"

class DapCmdPurchase : public DapCmdClientAbstract
{
    Q_OBJECT
public:
    explicit DapCmdPurchase(QObject *parent = nullptr);
    void requestVerify(const QString &packageName, const QString &productId, const QString &token);

protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;

signals:
    void purchaseVerified(const QString& key);//TODO
};

#endif // DAPCMDPURCHASE_H
