#include "DapCmdNews.h"
#include "DapJsonParams.h"
#include <QJsonArray>

DapCmdNews::DapCmdNews(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::GET_NEWS, parent)
{
    connect(this, &DapCmdNews::sigReceivedNewsMessage, [=](const QJsonDocument& news){
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

void DapCmdNews::handle(const QJsonObject* params)
{
    DapCmdServiceAbstract::handle(params);
    //if (params->value("message").toString() == "news")
    emit getNews();
}
