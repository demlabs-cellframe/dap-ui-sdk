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
//  todo set name for windows, macos
    m_downloadFileName = QDir::tempPath() + QDir::separator() +  "KelVPN-update.deb";
    // remove file if exist
    if (QFile::exists(m_downloadFileName))
        QFile::remove(m_downloadFileName);

    request.setUrl(QUrl(m_networkRequest));
    m_networkReply = m_httpClient->get(request);

    connect( m_networkReply, &QNetworkReply::finished, this, [=] {
//        QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
//        qDebug() << "Download finished" << reply->error();
//        qDebug() << "Download finished";
        m_downloading = false;
    });
    connect( m_networkReply, SIGNAL(error), this, SLOT([=](QNetworkReply::NetworkError networkErr) {
//        qDebug() << "Download error:" << networkErr;
//        qDebug() << "Download error:";
//        emit downloadError(m_networkReply->errorString());
        m_downloading = false;
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
    // send request
//    qDebug() << "Download started" << m_networkRequest << "to" << m_downloadFileName;
}


QString& DapDownload::downloadFileName()
{
    return m_downloadFileName;
}