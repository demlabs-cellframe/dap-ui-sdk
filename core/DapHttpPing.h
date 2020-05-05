#ifndef DAPHTTPPING_H
#define DAPHTTPPING_H

#include <QNetworkReply>
#include <QElapsedTimer>

// Object of this class will automaticly deleted
// Don't use delete!
class DapHttpPingNetworkReply : public QObject
{
    Q_OBJECT
private:
    QNetworkReply* m_reply;
    QString m_host;
    quint16 m_port;
public:
    explicit DapHttpPingNetworkReply(QNetworkReply *networkReply, const QString& a_host,
                                     const quint16 a_port) :
        QObject(networkReply), m_host(a_host), m_port(a_port) {

        QElapsedTimer *timer = new QElapsedTimer;
        timer->start();

        connect(networkReply, &QNetworkReply::finished, this, [=] {
            if(networkReply->error() == QNetworkReply::NetworkError::NoError) {
                emit sigResponse(timer->elapsed());
            } else {
                emit sigNetworkError(networkReply->error());
            }
            delete timer;
        });
    }

    QString getHost() { return m_host; }
    quint16 getPort() { return m_port; }
signals:
    void sigResponse(qint64 requestTime);
    void sigNetworkError(QNetworkReply::NetworkError);
};

class DapHttpPing
{
private:
    explicit DapHttpPing() {}
public:
    static DapHttpPingNetworkReply* sendRequest(const QString& host, quint16 port);
};

#endif // DAPHTTPPING_H
