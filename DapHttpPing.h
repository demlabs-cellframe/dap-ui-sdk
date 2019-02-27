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
public:
    explicit DapHttpPingNetworkReply(QNetworkReply *networkReply) : QObject(networkReply) {
        QElapsedTimer *timer = new QElapsedTimer;
        timer->start();

        connect(networkReply, &QNetworkReply::finished, this, [=] {
            if(networkReply->error() == QNetworkReply::NetworkError::NoError) {
                qDebug() << timer->elapsed() << networkReply->readAll();
                emit sigResponse(timer->elapsed());
            } else {
                emit sigNetworkError(networkReply->error());
            }
            delete timer;
        });
    }
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
