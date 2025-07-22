#ifndef DAPDOWNLOAD_H
#define DAPDOWNLOAD_H
#include <QCoreApplication>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>


class DapDownload: public QObject
{
    Q_OBJECT

private:
    // http access manager
    QNetworkAccessManager * m_httpClient;
    // reply data
    QNetworkReply *m_networkReply;
    // network request string
    QString m_networkRequest;
    // file name
    QString m_downloadFileName;
    // host
    QString m_host;
    // port
    quint16 m_port;

public:
    static const int DEFAULT_REQUEST_TIMEOUT = 10000; // 10 sec
    static const quint16 DefaultPort;

public:
    DapDownload(QObject * obj = Q_NULLPTR);
    static DapDownload* instance();
    QString& downloadFileName();

public slots:
    void startDownload(const QString& url, const QString& pack);
    void removeDownload(const QString& downloadFileName);

private:
//    void request_GET(const QString& host,  quint16 port,
//                     const QString & urlPath, DapNetworkReply &a_netReply,
//                     const QString &headers = "", bool ssl = false);
//    void parseUrl(const QString& url);
    void sendRequest();
    bool m_downloading;

signals:
    void fileSaved(QString path);
    void downloadProgress(qint64 load, qint64 total);
    void downloadError(int code, QString error);
};

#endif // DAPDOWNLOAD_H
