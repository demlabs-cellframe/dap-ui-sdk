#include "DapDownload.h"
#include <QSaveFile>
#include <QFile>
#include <QDir>
#include <QDebug>

const quint16  DapDownload::DefaultPort     (80);
static DapDownload *__inst = nullptr;

DapDownload::DapDownload(QObject * obj):
    QObject(obj),
    m_networkReply(nullptr),
    m_downloading(false)
{
    m_httpClient = new QNetworkAccessManager;
}

DapDownload * DapDownload::instance()
{
    if (__inst == nullptr)
      __inst  = new DapDownload;
    return __inst;
}

void DapDownload::startDownload(const QString& a_url, const QString& a_pack)
{
    if (m_downloading)
        return;
    m_networkRequest = a_url;
    m_downloadFileName = a_pack;
    sendRequest();
}

void DapDownload::removeDownload(const QString& downloadFileName)
{
    QFile::remove(downloadFileName);
}

void DapDownload::sendRequest()
{
    QNetworkRequest request;
    if (m_downloadFileName.isEmpty()) {
//  todo set name for macos
        qWarning() << "Upload file name not set";
        return;
    }
    m_downloading = true;
    // remove file if exist
    if (QFile::exists(m_downloadFileName))
        QFile::remove(m_downloadFileName);
    // send request
    request.setUrl(QUrl(m_networkRequest));
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                         QNetworkRequest::NoLessSafeRedirectPolicy);
    m_networkReply = m_httpClient->get(request);
    connect(m_networkReply, &QNetworkReply::finished, this, [this]() {
        qWarning() << "Download finished" << m_networkReply->error();
        m_downloading = false;
        if(m_networkReply->error() != QNetworkReply::NetworkError::NoError)
            emit downloadError(m_networkReply->error(), tr("Download ended with an error"));
        m_networkReply->deleteLater();
        m_networkReply = nullptr;
    });
    connect(m_networkReply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError networkErr) {
        qWarning() << "Download error:" << networkErr;
        m_downloading = false;
        emit downloadError(static_cast<int>(networkErr), tr("Download ended with an error"));
    });
    connect(m_networkReply, &QNetworkReply::readyRead, this, [this]() {
        QFile file(m_downloadFileName);
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
        {
           if(m_networkReply->size())
           {
               QByteArray data = m_networkReply->readAll();
               file.write(data);
           }
           file.flush();
           file.close();
        }
    });
    connect(m_networkReply, &QNetworkReply::downloadProgress, this, [this](qint64 load, qint64 total) {
        emit downloadProgress(load, total);
    });
    qInfo() << "Download started" << m_networkRequest << "to" << m_downloadFileName;
}


QString& DapDownload::downloadFileName()
{
    return m_downloadFileName;
}
