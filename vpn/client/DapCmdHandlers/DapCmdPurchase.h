#ifndef DAPCMDPURCHASE_H
#define DAPCMDPURCHASE_H

#include "DapCmdServiceAbstract.h"

class DapCmdPurchase : public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    explicit DapCmdPurchase(QObject *parent = nullptr);
    void handle(const QJsonObject *params) override;
signals:
    void purchaseVerified(const QJsonDocument& result);
    void requestPurchase(const QJsonObject *params);
};

#endif // DAPCMDPURCHASE_H
