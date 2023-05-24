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

        QJsonArray jsonObj = result.value("news_list").toArray();

        foreach (const QJsonValue & val, jsonObj)
            if (val.toObject().value("type").toString() == "update"){

                if (fromJSON(val.toObject().value("update_arr").toArray(), &listNews))
                {
                    emit sigGotUpdateNews(listNews);
                } else {
                    qCritical() << "Error parse response from service";
                }
            } else if (val.toObject().value("type").toString() == "for_ticker")
            {
                emit sigGotTickerNews(val.toObject().value("message").toString(),
                                      val.toObject().value("url").toString());
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
        QJsonValue target_os = jsonObj.value("target_os");
            if (target_os == Utils::getOSName()){
                fillingListNews(jsonObj, listNews);
            }
    }

    return !listNews->isEmpty();
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

    }
}


void DapCmdNews::handleError(int code, const QString& message)
{
    Q_UNUSED(message)
    qDebug() << "Error code: " + QString::number(code) + "Massage: " + message;
    qWarning() << *m_errorObject;
}
