#include "DapCmdPurchase.h"
#include "DapJsonParams.h"
#include <QJsonArray>

DapCmdPurchase::DapCmdPurchase(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::REQUEST_PURCHASE_VERIFY, parent)
{
    connect(this, &DapCmdPurchase::purchaseVerified, [=](const QJsonDocument& result){
        QJsonObject obj = result.object();
        sendCmd(&obj);
    });
}

void DapCmdPurchase::handle(const QJsonObject* params)
{
    DapCmdServiceAbstract::handle(params);
    emit requestPurchase(params);
}
