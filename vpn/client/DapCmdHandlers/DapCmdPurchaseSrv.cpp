#include "DapCmdPurchaseSrv.h"
#include "DapJsonParams.h"
#include <QJsonArray>

DapCmdPurchaseSrv::DapCmdPurchaseSrv(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::REQUEST_PURCHASE_VERIFY, parent)
{
  connect(this, &DapCmdPurchaseSrv::purchaseVerified, [=](const QJsonDocument& result){
        QJsonObject obj = result.object();
        sendCmd(&obj);
    });
}

void DapCmdPurchaseSrv::handle(const QJsonObject* params)
{
    emit requestPurchase(params);
}
