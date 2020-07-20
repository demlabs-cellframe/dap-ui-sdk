#ifndef DAPCMDNEWS_H
#define DAPCMDNEWS_H

#include "DapCmdServiceAbstract.h"

class DapCmdNews: public DapCmdServiceAbstract
{
    Q_OBJECT
public:

    DapCmdNews(QObject *parent = nullptr);
    void handle(const QJsonObject *params) override;

signals:
    void sigReceivedNewsMessage(const QJsonDocument& news);
    void getNews();
};

#endif // DAPCMDNEWS_H
