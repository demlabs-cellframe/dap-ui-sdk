#include "DapCmdNewsSrv.h"
#include "DapJsonParams.h"
#include <QJsonArray>

DapCmdNewsSrv::DapCmdNewsSrv(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::GET_NEWS, parent)
{
    connect(this, &DapCmdNewsSrv::sigReceivedNewsMessage, [=](const QJsonDocument& news){
        auto arr = news.array();
        if (arr.isEmpty()) {
            sendSimpleError(-4563, "Empty news list");
        } else {
            QJsonObject obj;
            obj["news_list"] = arr;
            sendCmd(&obj);
        }
    });
}

void DapCmdNewsSrv::handle(const QJsonObject* params)
{
    Q_UNUSED(params);
    //if (params->value("message").toString() == "news")
    emit getNews();
}
