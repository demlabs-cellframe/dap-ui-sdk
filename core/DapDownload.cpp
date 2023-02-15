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

void DapDownload::startDownload(const QString& a_url)
{
    if (m_downloading)
        return;
    m_networkRequest = a_url;
    sendRequest();
}

void DapDownload::sendRequest()
{
    m_downloading = true;
    QNetworkRequest request;
//  todo set name for macos
#ifdef Q_OS_LINUX
    m_downloadFileName = QDir::tempPath() + QDir::separator() + QString("%1%2").arg(DAP_BRAND).arg("-update.deb"); // example KelVPN-installer.deb
#endif
#ifdef Q_OS_WIN
    m_downloadFileName = QDir::tempPath() + QDir::separator() + QString("%1%2").arg(DAP_BRAND).arg("-update.exe") // example "KelVPN-installer.exe";
#endif
#ifdef Q_OS_MACOS
    m_downloadFileName = QDir::tempPath() + QDir::separator() + QString("%1%2").arg(DAP_BRAND).arg("-update.pkg") // example "KelVPN-installer.pkg";
#endif

    // remove file if exist
    if (QFile::exists(m_downloadFileName))
        QFile::remove(m_downloadFileName);
    // send request
    request.setUrl(QUrl(m_networkRequest));
    m_networkReply = m_httpClient->get(request);
    connect( m_networkReply, &QNetworkReply::finished, this, [=] {
        QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
        qDebug() << "Download finished" << reply->error();
        m_downloading = false;
        if (reply->error() != QNetworkReply::NetworkError::NoError)
            emit downloadError(reply->error(), tr("Download ended with an error"));
    });
    connect( m_networkReply, SIGNAL(error), this, SLOT([=](QNetworkReply::NetworkError networkErr) {
        qDebug() << "Download error:" << networkErr;
        m_downloading = false;
        if (reply->error() != QNetworkReply::NetworkError::NoError)
            emit downloadError(reply->error(), "Download ended with an error");
    }));
    connect( m_networkReply, &QNetworkReply::readyRead, this, [=]() {
        QFile* m_file = new QFile(m_downloadFileName);
        if(m_file->open(QIODevice::WriteOnly | QIODevice::Append))
        {
           if(m_networkReply->size())
           {
               QByteArray data = m_networkReply->readAll();
               m_file->write(data);
           }
           m_file->flush();
           m_file->close();
        }
    });
    connect( m_networkReply, &QNetworkReply::downloadProgress, this, [=](quint64 load, quint64 total) {
        emit downloadProgress(load, total);
    });
    qDebug() << "Download started" << m_networkRequest << "to" << m_downloadFileName;
}


QString& DapDownload::downloadFileName()
{
    return m_downloadFileName;
}
