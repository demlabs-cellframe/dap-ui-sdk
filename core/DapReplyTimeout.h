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
        QMetaObject::Connection c;
        DapReplyTimeout(QNetworkReply* reply, const int timeout)
            : QObject(reply) {
            Q_ASSERT(reply);
            if (reply->isOpen()) {
                c = connect(reply, SIGNAL(finished()), this, SLOT(interrupt()));
                QTimer::singleShot(timeout, Qt::PreciseTimer, this, [=]() {
                    if (!reply->isFinished()) {
                        qWarning() << "Request timeout";
                        reply->abort();
                    }
                });
            } else {
                qCritical() << "Request is already closed";
            }
        }

    public:
        static void set(QNetworkReply* reply, const int timeout) {
            new DapReplyTimeout(reply, timeout);
        }
    protected slots:
        void interrupt() {
            qInfo() << "Request processed";
            QAbstractEventDispatcher::instance()->unregisterTimers(this);
            delete this;
        }

};

#endif // DAPREPLYTIMEOUT_H
