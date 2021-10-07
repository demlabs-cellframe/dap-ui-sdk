#include "DapServersListRequester.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "DapNetworkReply.h"

DapServersListNetworkReply::DapServersListNetworkReply(DapNetworkReply *a_networkReply)
{
    m_networkReply = a_networkReply;
    connect(m_networkReply, &DapNetworkReply::finished, this, [=] {
        if(m_networkReply->error() == 0) {
            QJsonParseError jsonErr;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(m_networkReply->getReplyData(), &jsonErr);

            if(!jsonDoc.isNull()) {
                if(!jsonDoc.isArray()) {
                    qCritical() << "Error parse response. Must be array";
                    emit sigParseResponseError();
                    return;
                }
                emit sigResponse(jsonDoc);
            } else {
                qWarning() << "Server response:" << m_networkReply->getReplyData();
                qCritical() << "Can't parse server response to JSON: "<<jsonErr.errorString()<< " on position "<< jsonErr.offset ;
                emit sigParseResponseError();
                return;
            }
        } else {
            emit sigNetworkError(m_networkReply->error());
        }
    });

    connect(m_networkReply, &DapNetworkReply::sigError, this, [=] {
        qCritical() << "Couldn't fetch servers list";
        emit sigNetworkError(m_networkReply->error());
    });
}
