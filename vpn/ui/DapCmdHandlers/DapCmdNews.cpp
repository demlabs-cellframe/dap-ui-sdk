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
        for (auto key: jsonObj.keys()) {
            if (jsonObj[key].type() == QJsonValue::Double) {
                temp[key] = QString::number(qint64(jsonObj[key].toDouble()));
            }
            else {
                temp[key] = jsonObj[key].toString();
            }
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
