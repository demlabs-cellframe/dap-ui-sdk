#include <QDebug>
#include <QDateTime>
#include "DapCmdServersList.h"
#include "DapJsonParams.h"
#include "DapConnectClient.h"
#include "DapServiceDataLocal.h"
#include <QMetaEnum>

DapCmdServersList::DapCmdServersList(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::GET_SERVERS_LIST, parent)
{
    emitTimer = new QTimer(this);
    emitTimer->setInterval(5000);
    emitTimer->setSingleShot(true);
    guiCall = true;

    connect(emitTimer, &QTimer::timeout, this, [=]() {
        sendRequestToCDB();
    });
}

void DapCmdServersList::handle(const QJsonObject* params)
{
    DapCmdServiceAbstract::handle(params);
    sendRequestToCDB();
    guiCall = true;

}

void DapCmdServersList::sendRequestToCDB() {
    DapNetworkReply* reply = new DapNetworkReply(this);

    connect(reply, &DapNetworkReply::finished, this, [this, reply] {
        handleReplyFinished(reply);
    });

    connect(reply, &DapNetworkReply::sigError, this, [this, reply] {
        handleReplyError(reply);
    });

    sendRequestToCurrentCDB(reply);
}

void DapCmdServersList::handleReplyFinished(DapNetworkReply* reply) {
    if (!reply) return;

    QJsonParseError jsonErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->getReplyData(), &jsonErr);

    if (jsonDoc.isNull() || !jsonDoc.isArray()) {
        qCritical() << "Can't parse server response to JSON:" << jsonErr.errorString() << "on position" << jsonErr.offset;
        processNextCDB(-32001, "Bad response from server. Parse error");
        return;
    }

    auto arr = jsonDoc.array();
    QJsonArray filteredArray = filterUnavailableServers(arr);

    if (filteredArray.isEmpty()) {
        processNextCDB(-32003, "Empty nodelist after filtering, try another CDB...");
    } else {
        qDebug() << "Filtered server list array:" << filteredArray;
        updateServerList(filteredArray);
    }
}

QJsonArray DapCmdServersList::filterUnavailableServers(const QJsonArray& arr) {
    QJsonArray filteredArray;
    for (const auto& item : arr) {
        if (!item.isObject()) continue;
        QJsonObject obj = item.toObject();
        if (!obj.contains("Status") || obj.value("Status").toString() != "unavailable") {
            filteredArray.append(obj);
        }
    }
    return filteredArray;
}

void DapCmdServersList::handleReplyError(DapNetworkReply* reply) {
    if (!reply) return;

    qDebug() << "Network error occurred:" << reply->errorString();

    if (guiCall) {
        if (loadServerList()) {
            guiCall = false;
        }
    }

    if (!emitTimer->isActive()) {
        emitTimer->start();
    }

    emit nextCdb();
    sendSimpleError(reply->error(), reply->errorString());
}

void DapCmdServersList::processNextCDB(int errorCode, const QString& errorMessage) {
    DapServiceDataLocal::instance()->nextCbdIterator();

    auto cdbIterator = DapServiceDataLocal::instance()->getCdbIterator();
    if (cdbIterator == DapServiceDataLocal::instance()->cdbServersList().end()) {
        DapServiceDataLocal::instance()->setNewCbdIterator(DapServiceDataLocal::instance()->cdbServersList().begin());

        if (loadServerList()) {
            return;
        }
    }

    sendSimpleError(errorCode, errorMessage);
}

void DapCmdServersList::sendRequestToCurrentCDB(DapNetworkReply* reply) {
    auto it = DapServiceDataLocal::instance()->getCdbIterator();
    qDebug() << "Sending request to:" << it->address << ":" << it->port;
    DapConnectClient::instance()->request_GET(it->address, it->port, "nodelist", *reply);
}

void DapCmdServersList::updateServerList(const QJsonArray& arr) {
    QString time = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());

    saveServerListToSettings(arr, time);
    emitServerListUpdate(arr, time);
}

bool DapCmdServersList::loadServerList() {
    QJsonArray arr;
    QString time;
    DapServiceDataLocal::instance()->loadFromSettings(DapServiceDataLocal::LAST_NODE_LIST_UPDATE, arr);
    DapServiceDataLocal::instance()->loadFromSettings(DapServiceDataLocal::LAST_NODE_LIST_UPDATE_TIME, time);

    qDebug() << "Loading server list from stage. " << QDateTime::fromSecsSinceEpoch(time.toLongLong()).toString("yyyy-MM-dd HH:mm:ss");

    if (arr.isEmpty()) {
        qDebug() << "Failed to load server list: no data found";
        return false;
    }

    sendSimpleError(-32008, "CDBs not available. Loaded local server list. Update " + QDateTime::fromSecsSinceEpoch(time.toLongLong()).toString("yyyy-MM-dd HH:mm:ss"));

    emitServerListUpdate(arr, time);

    return true;
}

void DapCmdServersList::sendServerList(DapServerInfoList m_nodelist, QString time) {
    QJsonObject obj;
    QJsonArray serverArray;
    for (const auto& server : m_nodelist) {
        serverArray.append(DapServerInfo::toJSON(server));
    }
    obj["servers"] = serverArray;
    obj["update_time"] = time;

    qDebug() << "Sending server list to gui";

    sendCmd(&obj);
}

void DapCmdServersList::saveServerListToSettings(const QJsonArray& arr, const QString& time) {
    DapServiceDataLocal::instance()->saveToSettings(DapServiceDataLocal::LAST_NODE_LIST_UPDATE, arr);
    DapServiceDataLocal::instance()->saveToSettings(DapServiceDataLocal::LAST_NODE_LIST_UPDATE_TIME, time);

    qDebug() << "Saved updated server list at " << QDateTime::fromSecsSinceEpoch(time.toLongLong()).toString("yyyy-MM-dd HH:mm:ss");
}

void DapCmdServersList::emitServerListUpdate(const QJsonArray& arr, const QString& time) {
    QJsonObject obj;
    obj["servers"] = arr;

    DapServerInfoList l;
    if (DapServerInfo::fromJSON(obj.value("servers").toArray(), l) && !l.isEmpty()) {
        emit updateNodesList(l, time);
        emit sendCurrentServer();
    } else {
        qDebug() << "Error parsing nodelist from JSON";
    }
}
