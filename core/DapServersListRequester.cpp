#include "DapServersListRequester.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "DapNetworkReply.h"

DapServersListNetworkReply::DapServersListNetworkReply(DapNetworkReply *networkReply)
{
    connect(networkReply, &DapNetworkReply::finished, this, [=] {
        if(networkReply->error() == DapNetworkReply::NetworkError::NoError) {
            QByteArray ba(networkReply->readAll());
            QJsonParseError jsonErr;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(ba, &jsonErr);

            if(!jsonDoc.isNull()) {
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
            if ((networkReply->error() == DapNetworkReply::NetworkError::NetworkSessionFailedError)
                    || (networkReply->error() == DapNetworkReply::NetworkError::UnknownNetworkError)) {
                //TODO
            }
        }
    });
}
