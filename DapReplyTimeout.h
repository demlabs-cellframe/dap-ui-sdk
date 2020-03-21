#ifndef DAPREPLYTIMEOUT_H
#define DAPREPLYTIMEOUT_H

#include <QNetworkReply>
#include <QTimer>
#include <QTimerEvent>
#include <QDebug>
#include <QtCore>
#include <QAbstractEventDispatcher>

class DapReplyTimeout : public QObject {
    Q_OBJECT
private:
    QNetworkReply *repl;
    DapReplyTimeout(QNetworkReply* reply, const int timeout)
        : QObject(reply) {
        Q_ASSERT(reply);
        if (reply) {
            repl = reply;
        }
        if (repl->isOpen()) {
            QTimer::singleShot(timeout, this, SLOT(timeout()));
            connect(repl, SIGNAL(finished()), this, SLOT(interrupt()));
        } else {
            qWarning() << "This network request is not running. Can't set request timeout";
        }
    }
public:
    static void set(QNetworkReply* reply, const int timeout) {
        new DapReplyTimeout(reply, timeout);
    }
protected slots:
    void timeout() {
        if (!repl) {
            return;
        }
        disconnect(repl, SIGNAL(finished()), this, SLOT(interrupt()));
        if (!repl->isFinished()) {
            qWarning() << "Request timeout, aborting";
            repl->abort();
        }
    }
    void interrupt() {
        qInfo() << "Request replied";
        QAbstractEventDispatcher::instance()->unregisterTimers(this);
    }
};

#endif // DAPREPLYTIMEOUT_H
