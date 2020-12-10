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
            emit sigGotNews(listNews);
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

        if (jsonObj["target_os"].isArray()){
            QJsonArray target_os = jsonObj["target_os"].toArray();
            for (int i = 0; i < target_os.size(); i++){
                if (target_os[i].toString() == Utils::getOSName()){
                    fillingListNews(jsonObj, listNews);
                }
            }
        } else {
            if (jsonObj["target_os"].toString() == "any" || jsonObj["target_os"].toString() == Utils::getOSName()){
                fillingListNews(jsonObj, listNews);
            }
        }
    }
    return true;
}

void DapCmdNews::fillingListNews(const QJsonObject& jsonObj, QList<QMap<QString, QString>>* listNews)
{
    if (jsonObj["type"].toString() == "update_version"){

       listNews->append({
                          {"type",    jsonObj["type"].toString()},
                          {"message", jsonObj["message"].toString()},
                          {"version", jsonObj["version"].toString()},
                          {"url",     jsonObj["url"].toString()}
                        });

    } else if (jsonObj["type"].toString() == "news"){

       listNews->append({
                          {"type",    jsonObj["type"].toString()},
                          {"message", jsonObj["message"].toString()}
                        });

    } else {
        qInfo() << "News does not match known types";
    }
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
