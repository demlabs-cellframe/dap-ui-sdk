#ifndef DAPCMDNEWS_H
#define DAPCMDNEWS_H

#include "DapCmdClientAbstract.h"
#include "Utilz.h"

class DapCmdNews: public DapCmdClientAbstract
{
    Q_OBJECT

public:
    explicit DapCmdNews(QObject *parent = nullptr);
    void sendingGetNewsRequest();

protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;
    bool fromJSON(const QJsonArray& jsonArr, QList<QMap<QString, QString>> *listNews);
    void fillingListNews(const QJsonObject& jsonObj, QList<QMap<QString, QString>>* listNews);
signals:
    void sigGotUpdateNews(QList<QMap<QString, QString>>);
    void sigGotTickerNews(const QString& message, const QString& url);

};

#endif // DAPCMDNEWS_H
