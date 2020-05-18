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
            QJsonParseError jsonErr;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(ba, &jsonErr);

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
                qCritical() << "Can't parse server response to JSON: "<<jsonErr.errorString()<< " on position "<< jsonErr.offset ;
                emit sigParseResponseError();
                return;
            }

        } else {
            emit sigNetworkError(networkReply->error());
        }
    });
}
