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
    DapReplyTimeout(DapNetworkReply* reply, const int timeout){
        Q_ASSERT(reply);
        if (reply) {
            connect(reply, &DapNetworkReply::finished, this, [=] {
                qInfo() << "Request processed";
                QAbstractEventDispatcher::instance()->unregisterTimers(this);
                delete this;
            });
            QTimer::singleShot(timeout, Qt::PreciseTimer, this, [=]() {
                if (!reply->isFinished()) {
                    qWarning() << "Request timeout";
//                    reply->abort();
                }
            });
        } else {
            qCritical() << "Request is already closed";
            //DapConnectClient::instance()->_rebuildNetworkManager();
            delete this;
        }
    }

public:
    static void set(DapNetworkReply* reply, const int timeout) {
        new DapReplyTimeout(reply, timeout);
    }
};

#endif // DAPREPLYTIMEOUT_H
