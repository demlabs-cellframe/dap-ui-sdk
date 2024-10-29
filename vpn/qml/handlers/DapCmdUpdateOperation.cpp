#include "DapCmdUpdateOperation.h"
#include <QJsonArray>
#include <QList>
#include <QMap>


void DapCmdUpdateOperation::setDownloadUrl(QString url)
{
    m_dowmloadUrl = url;
}

void DapCmdUpdateOperation::setDownloadPack(QString pack)
{
    m_dowmloadPack = pack;
}

void DapCmdUpdateOperation::startDownload()
{
    startDownloadUrl(m_dowmloadUrl, m_dowmloadPack);
}

void DapCmdUpdateOperation::startDownloadUrl(QString url, QString pack)
{
    QJsonObject response;
    QJsonObject downloadInfo;
    downloadInfo["url"] = url;
    downloadInfo["pack"] = pack;
    response["start_download"] = downloadInfo;
    sendCmd(&response);
}

void DapCmdUpdateOperation::startUpadte()
{
    QJsonObject response;
    response["start_update"] = true;
    sendCmd(&response);
}


void DapCmdUpdateOperation::handleResult(const QJsonObject& result)
{
    if(result.value("load").isString() && result.value("total").isString()) {
        qint64 loadSize = 0;
        qint64 totalSize = 1;
        bool ok;
        loadSize = result.value("load").toString().toInt(&ok);
        if (!ok)
            qCritical() << "Bad response from service" << "load key";
        totalSize = result.value("total").toString().toInt(&ok);
        if (!ok)
            qCritical() << "Bad response from service" << "total key";
        if (totalSize != 0)
            emit downloadProgress(loadSize, totalSize);
    } else {
        qCritical() << "Bad response from service";
    }
}

void DapCmdUpdateOperation::handleError(int code, const QString& message)
{
    Q_UNUSED(code); Q_UNUSED(message);
    qWarning() << *m_errorObject;
    emit downloadError(message);
}
