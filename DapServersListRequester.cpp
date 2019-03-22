#include "DapServersListRequester.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

DapServersListNetworkReply::DapServersListNetworkReply(QNetworkReply *networkReply) :
    QObject(networkReply)
{
    connect(networkReply, &QNetworkReply::finished, this, [=] {

        if(networkReply->error() == QNetworkReply::NetworkError::NoError) {

            QByteArray ba(networkReply->readAll());
            QJsonDocument jsonDoc = QJsonDocument::fromJson(ba);

            if(!jsonDoc.isNull()) {
                qDebug() << "Servers list response" << jsonDoc;

                if(!jsonDoc.isArray()) {
                    qCritical() << "Error parse response. Must be array";
                    emit sigParseResponseError();
                    return;
                }
                emit sigResponse(jsonDoc);
            } else {
                qWarning() << "Server response:" << ba;
                qCritical() << "Can't parse server response to JSON";
                emit sigParseResponseError();
                return;
            }

        } else {
            emit sigNetworkError(networkReply->error());
        }
    });
}
