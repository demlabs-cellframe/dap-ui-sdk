#ifndef DAPCMDPURCHASE_H
#define DAPCMDPURCHASE_H

#include "DapCmdServiceAbstract.h"

class DapCmdPurchaseSrv : public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    explicit DapCmdPurchaseSrv(QObject *parent = nullptr);
    void handle(const QJsonObject *params) override;
signals:
    void purchaseVerified(const QJsonDocument& result);
    void requestPurchase(const QJsonObject *params);
};

#endif // DAPCMDPURCHASE_H
