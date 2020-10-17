#ifndef DAPREPLYTIMEOUT_H
#define DAPREPLYTIMEOUT_H

#include <QNetworkReply>
#include <QTimer>
#include <QTimerEvent>
#include <QDebug>
#include <QtCore>
#include <QAbstractEventDispatcher>
#include "DapConnectClient.h"

class DapReplyTimeout : public QObject {
Q_OBJECT
private:
    DapReplyTimeout(QNetworkReply* reply, const int timeout) : QObject(reply) {
        Q_ASSERT(reply);
        if (reply->isOpen()) {
            connect(reply, &QNetworkReply::finished, this, [=] {
                if ((reply->error() == QNetworkReply::NetworkSessionFailedError)
                        || (reply->error() == QNetworkReply::UnknownNetworkError)) {
                    DapConnectClient::instance()->_rebuildNetworkManager();
                }
                qInfo() << "Request processed";
                QAbstractEventDispatcher::instance()->unregisterTimers(this);
                delete this;
            });
            QTimer::singleShot(timeout, Qt::PreciseTimer, this, [=]() {
                if (!reply->isFinished()) {
                    qWarning() << "Request timeout";
                    reply->abort();
                }
            });
        } else {
            qCritical() << "Request is already closed";
            DapConnectClient::instance()->_rebuildNetworkManager();
            delete this;
        }
    }

public:
    static void set(QNetworkReply* reply, const int timeout) {
        new DapReplyTimeout(reply, timeout);
    }
};

#endif // DAPREPLYTIMEOUT_H
