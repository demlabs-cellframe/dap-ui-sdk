#ifndef DAPREPLYTIMEOUT_H
#define DAPREPLYTIMEOUT_H

#include <QNetworkReply>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QDebug>

/*
Source: https://stackoverflow.com/questions/37444539/how-to-set-qnetworkreply-timeout-without-external-timer
Use:
    QNetworkAccessManager networkAccessManger;
    QNetworkReply* reply =
        networkAccessManger.get(QNetworkRequest(QUrl("https://www.google.com")));
    ReplyTimeout::set(reply, 100);
*/

class DapReplyTimeout : public QObject {
    Q_OBJECT
private:
    QBasicTimer m_timer;

    DapReplyTimeout(QNetworkReply* reply, const int timeout)
        : QObject(reply) {
        Q_ASSERT(reply);
        if (reply && reply->isRunning()) {
            m_timer.start(timeout, this);
        } else {
            qWarning() << "QNetworkReply is not running. Can't set request timeout";
        }
    }
public:
    static void set(QNetworkReply* reply, const int timeout) {
        new DapReplyTimeout(reply, timeout);
    }
protected:
    void timerEvent(QTimerEvent * ev) {
        if (!m_timer.isActive() || ev->timerId() != m_timer.timerId())
            return;
        auto reply = static_cast<QNetworkReply*>(parent());
        if (reply->isRunning()) {
            qWarning() << "Close request by timeout";
            reply->abort();
        }
        m_timer.stop();
    }
};

#endif // DAPREPLYTIMEOUT_H
