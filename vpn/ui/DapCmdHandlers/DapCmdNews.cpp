#include "DapCmdNews.h"
#include <QJsonArray>
#include <QList>
#include <QMap>

DapCmdNews::DapCmdNews(QObject *parent)
    : DapCmdClientAbstract (DapJsonCmdType::GET_NEWS, parent)
{

}

void DapCmdNews::sendingGetNewsRequest()
{
    QJsonObject obj;
    sendCmd(&obj);
}

void DapCmdNews::handleResult(const QJsonObject& result)
{
    if(result.value("news_list").isArray()) {
        QList<QMap<QString, QString>> listNews;

        if (fromJSON(result.value("news_list").toArray(), &listNews))
        {
            emit sigGetedNews(listNews);
        } else {
            qCritical() << "Error parse response from service";
        }
    } else {
        qCritical() << "Bad response from service";
    }
}

bool DapCmdNews::fromJSON(const QJsonArray& jsonArr, QList<QMap<QString, QString>>* listNews)
{
    for (auto it = jsonArr.constBegin(); it != jsonArr.constEnd(); ++it) {
        const QJsonValue &val = *it;

        if(!val.isObject()) {
            qCritical() << "Error parse QJsonValue to QJsonObject";
            return false;
        }

        QJsonObject jsonObj = val.toObject();
        QMap<QString, QString> temp;
        if (jsonObj["type"].toString() == "update_version"){
           temp = { {"type",    jsonObj["type"].toString()},
                    {"message", jsonObj["message"].toString()},
                    {"version", jsonObj["version"].toString()},
                    {"url",     jsonObj["url"].toString()}};
        } else if (jsonObj["type"].toString() == "news"){
           temp = { {"type",    jsonObj["type"].toString()},
                    {"message", jsonObj["message"].toString()}};
        }

        listNews->append(temp);
    }
    return true;
}


void DapCmdNews::handleError(int code, const QString& message)
{/*
    Q_UNUSED(message)
    if (code == -666) {
        emit sigEmptyList(message);
    } else if (code == -32002) {
        emit sigErrorNetwork(message);
    }
    qWarning() << *m_errorObject;*/
}
