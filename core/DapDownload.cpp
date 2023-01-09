#include "DapDownload.h"
#include <QSaveFile>
#include <QFile>
#include <QDir>
#include <QtAndroid>

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
    m_networkRequest = a_url;
    sendRequest();
}

void DapDownload::sendRequest()
{
    QNetworkRequest request;
    // example m_networkRequest = "https://pub.kelvpn.com/linux/latest/KelVPN-7.3-xxx-amd64.deb";
#ifdef Q_OS_ANDROID
    static QAndroidJniObject l_pathObj = QtAndroid::androidContext().callObjectMethod(
                "getExternalFilesDir"
                , "(Ljava/lang/String;)Ljava/io/File;"
                , QAndroidJniObject::fromString(QString("")).object());
    m_downloadFileName = QString("%1/%2").arg(l_pathObj.toString()).arg("KelVPN-update.apk");
#else
    m_downloadFileName = QDir::tempPath() + QDir::separator() +  "KelVPN-update.deb";
#endif


    // remove file if exist
    if (QFile::exists(m_downloadFileName))
        QFile::remove(m_downloadFileName);

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
    qInfo() << "Download started" << m_networkRequest;
}


QString& DapDownload::downloadFileName()
{
    return m_downloadFileName;
}
