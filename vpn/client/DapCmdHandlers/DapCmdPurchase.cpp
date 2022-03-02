#include "DapCmdPurchase.h"
#include "DapJsonParams.h"
#include <QJsonArray>

DapCmdPurchase::DapCmdPurchase(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::REQUEST_PURCHASE_VERIFY, parent)
{

}

void DapCmdPurchase::handle(const QJsonObject* params)
{
    connect(this, &DapCmdPurchase::purchaseVerified, [=](const QJsonDocument& result){
        QJsonObject obj = result.object();
        qDebug() << "Reply " << result.toJson();
        sendCmd(&obj);
    });

    emit requestPurchase(params);
}
