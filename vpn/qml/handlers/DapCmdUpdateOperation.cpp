#include "DapCmdUpdateOperation.h"
#include <QJsonArray>
#include <QList>
#include <QMap>
#include <QFile>
#include "DapDataLocal.h"
#include "Utilz.h"


void DapCmdUpdateOperation::setDownloadUrl(QString url)
{
    m_dowmloadUrl = url;
}

void DapCmdUpdateOperation::setAvailableVersion(QString avVersion)
{
    if (avVersion.startsWith("-"))
        avVersion.remove(0, 1);
    m_availableVersion = avVersion.replace(avVersion.indexOf("_"), 1, ".");
}

void DapCmdUpdateOperation::setDownloadPack(QString pack)
{
    m_dowmloadPack = pack;
}

void DapCmdUpdateOperation::startDownload()
{
    qInfo() << "startDownload: url =" << m_dowmloadUrl
            << "pack =" << m_dowmloadPack
            << "availableVersion =" << m_availableVersion;

    auto existingUpdate = DapDataLocal::instance()->getValueSetting(DapBaseDataLocal::UPDATE_FILE_PATH).toString();
    qInfo() << "startDownload: existingUpdate =" << existingUpdate;

    if(existingUpdate.isEmpty())
    {
        startDownloadUrl(m_dowmloadUrl, m_dowmloadPack);
        return;
    }

    if(!m_dowmloadPack.isEmpty() && !QFile::exists(m_dowmloadPack))
    {
        qWarning() << "startDownload: cached file not found on disk:" << m_dowmloadPack << "- restarting download";
        DapDataLocal::instance()->saveValueSetting(DapBaseDataLocal::UPDATE_FILE_PATH, "");
        startDownloadUrl(m_dowmloadUrl, m_dowmloadPack);
        return;
    }

    auto parts = existingUpdate.split("-");
    if(parts.size() < 3)
    {
        qWarning() << "startDownload: malformed updatefilepath, restarting download";
        DapDataLocal::instance()->saveValueSetting(DapBaseDataLocal::UPDATE_FILE_PATH, "");
        startDownloadUrl(m_dowmloadUrl, m_dowmloadPack);
        return;
    }

    auto existingUpdateVer = parts[1] + "-" + parts[2];
    if(Utils::isNewerVersion(m_availableVersion, existingUpdateVer))
    {
        qInfo() << "startDownload: newer version available, restarting download";
        DapDataLocal::instance()->saveValueSetting(DapBaseDataLocal::UPDATE_FILE_PATH, "");
        startDownloadUrl(m_dowmloadUrl, m_dowmloadPack);
    }
    else
    {
        QChar separator = existingUpdate.contains('|') ? '|' : '%';
        const qint64 totalSize = existingUpdate.split(separator).last().toULongLong();
        if(totalSize <= 0)
        {
            qWarning() << "startDownload: invalid cached size, restarting download";
            DapDataLocal::instance()->saveValueSetting(DapBaseDataLocal::UPDATE_FILE_PATH, "");
            startDownloadUrl(m_dowmloadUrl, m_dowmloadPack);
            return;
        }
        qInfo() << "startDownload: using cached download, totalSize =" << totalSize;
        emit downloadProgress(totalSize, totalSize);
    }
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

void DapCmdUpdateOperation::sendDownloadRemove(const QString& downloadPack)
{
    QJsonObject response;
    QJsonObject downloadRemoveInfo;
    downloadRemoveInfo["pack"] = downloadPack;
    response["download_remove"] = downloadRemoveInfo;
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

        if (loadSize == totalSize)
        {
            DapDataLocal::instance()->saveValueSetting(DapBaseDataLocal::UPDATE_FILE_PATH, m_dowmloadUrl.split("/").last() + "|" + QString::number(totalSize));
        }
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
