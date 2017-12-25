#ifndef DAPCONNECTBASE_H
#define DAPCONNECTBASE_H

#include <QObject>
#include <QNetworkReply>
#include <QAbstractSocket>
#include <QString>
#include <QList>
#include <QBuffer>

class DapConnectBase : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void slotNetworkError(QNetworkReply::NetworkError);
    void slotReadPacketFinished();

public:
    DapConnectBase(QObject *parent = NULL);
    virtual ~DapConnectBase();

    QNetworkReply * request(const QString & url, QByteArray * rData = Q_NULLPTR);
    QNetworkReply * request(const QString & url, QString &rData);

protected:
    QNetworkAccessManager * http_client;
    QNetworkReply * network_reply;

    QByteArray byte_buffer;

    static QString httpAddress();

Q_SIGNALS:
    void finished();
    void errorText(const QString &e);
    void errorNetwork(int);
    void errorAuth(int);
    void notify(const QString&);


};

#endif // DAPCONNECTBASE_H
