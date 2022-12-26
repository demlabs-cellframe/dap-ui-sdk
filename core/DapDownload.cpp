#include "DapDownload.h"
#include <QSaveFile>
#include <QFile>

const quint16  DapDownload::DefaultPort     (80);
static DapDownload *__inst = nullptr;

DapDownload::DapDownload(QObject * obj):
    QObject(obj),
    m_networkReply(nullptr)
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
    QString url = "https://pub.kelvpn.com/linux/master/KelVPN-7.3-22-amd64.deb";
    sendRequest();
}

void DapDownload::sendRequest()
{
    QNetworkRequest request;
    m_networkRequest = "https://pub.kelvpn.com/linux/master/KelVPN-7.3-22-amd64.deb";
    m_downloadFileName = "/tmp/KelVPN-7.3-22-amd64.deb";
    request.setUrl(QUrl(m_networkRequest));
    m_networkReply = m_httpClient->get(request);

    connect( m_networkReply, &QNetworkReply::finished, this, [=] {
        QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
        qInfo() << "Download finished" << reply->error();
    });
    connect( m_networkReply, SIGNAL(error), this, SLOT([=](QNetworkReply::NetworkError networkErr) {
        qInfo() << "Download error:" << networkErr;
//        emit downloadError(m_networkReply->errorString());
    }));
    connect( m_networkReply, &QNetworkReply::readyRead, this, [=]() {
        qInfo() << "Data ready" << m_networkReply->size();
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
        qInfo() << "Download progress" << load/total;
        emit downloadProgress(load/total);
    });
    // send request
    qInfo() << "Download started" << m_networkRequest;
}
