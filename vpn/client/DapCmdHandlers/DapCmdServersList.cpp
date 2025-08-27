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

void DapCmdServersList::sendRequestToCDB(){

    DapNetworkReply* reply = new DapNetworkReply();

    connect(reply, &DapNetworkReply::finished, this, [=] {

        if (!reply) return;

        QJsonParseError jsonErr;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->getReplyData(), &jsonErr);

        if (jsonDoc.isNull() || !jsonDoc.isArray()) {

            qCritical() << "Can't parse server response to JSON: "<< jsonErr.errorString() << " on position "<< jsonErr.offset;
            //emit nextCdb();
            DapServiceDataLocal::instance()->nextCbdIterator();
            if (DapServiceDataLocal::instance()->getCdbIterator() != DapServiceDataLocal::instance()->cdbServersList().end())
            {
                DapServiceDataLocal::instance()->setNewCbdIterator(DapServiceDataLocal::instance()->cdbServersList().begin());

                if (loadServerList())
                    return;

            }
            sendSimpleError(-32001, "Bad response from server. Parse error");

        }
        else
        {
            auto arr = jsonDoc.array();
            if (arr.isEmpty())
            {
                //emit nextCdb();
                DapServiceDataLocal::instance()->nextCbdIterator();
                if (DapServiceDataLocal::instance()->getCdbIterator() != DapServiceDataLocal::instance()->cdbServersList().end())
                {
                    DapServiceDataLocal::instance()->setNewCbdIterator(DapServiceDataLocal::instance()->cdbServersList().begin());

                    if (loadServerList())
                        return;

                }
                sendSimpleError(-32003, "Empty nodelist, try another CDB...");
            } else {
                qDebug() << "Server list array:" << arr;

                updateServerList(arr);
            }
        }
    });

    connect(reply, &DapNetworkReply::sigError, this, [=] {

        if (!reply) return;

        qDebug() << "Network error occurred: " << reply->errorString();

        if (guiCall) {
            if (loadServerList()){
                guiCall = false;
            }
        }

        if (!emitTimer->isActive()) {
            emitTimer->start();
        }

        emit nextCdb();

        sendSimpleError(reply->error(), reply->errorString());
    });

    auto dataLocal = DapServiceDataLocal::instance();
      const auto &list = dataLocal->cdbServersList();
    auto it = dataLocal->getCdbIterator();

    if (list.isEmpty() || it == list.end()) {
        qWarning() << "[ServersList] CDB list empty or iterator invalid; using staged list";
        if (guiCall) {
            if (loadServerList()) {
                guiCall = false;
            }
        }
        if (!emitTimer->isActive()) {
            emitTimer->start();
        }
        sendSimpleError(-32004, "CDB list unavailable");
        return;
    }

    const QString address = it->address;
    const int port = it->port;
    if (address.isEmpty() || port <= 0) {
        qWarning() << "[ServersList] CDB address/port invalid; using staged list";
        if (guiCall) {
            if (loadServerList()) {
                guiCall = false;
            }
        }
        if (!emitTimer->isActive()) {
            emitTimer->start();
        }
        sendSimpleError(-32005, "CDB address/port invalid");
        return;
    }

    qDebug() << "Sending request to: " << address << ":" << port;
    DapConnectClient::instance()->request_GET(address, port, "nodelist", *reply);
}



void DapCmdServersList::updateServerList(const QJsonArray& arr)
{
    QString time = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());

    DapServiceDataLocal::instance()->saveToSettings(DapServiceDataLocal::LAST_NODE_LIST_UPDATE, arr);
    DapServiceDataLocal::instance()->saveToSettings(DapServiceDataLocal::LAST_NODE_LIST_UPDATE_TIME, time);

    qDebug() << "Saved updated server list at " << QDateTime::fromSecsSinceEpoch(time.toLongLong()).toString("yyyy-MM-dd HH:mm:ss");

    sendServerList(arr, time);

}

bool DapCmdServersList::loadServerList()
{
    QJsonArray arr;
    QString time;
    DapServiceDataLocal::instance()->loadFromSettings(DapServiceDataLocal::LAST_NODE_LIST_UPDATE, arr);
    DapServiceDataLocal::instance()->loadFromSettings(DapServiceDataLocal::LAST_NODE_LIST_UPDATE_TIME, time);

    qDebug() << "Loading server list from stage. " + QDateTime::fromSecsSinceEpoch(time.toLongLong()).toString("yyyy-MM-dd HH:mm:ss");

    if (arr.isEmpty()) {
        qDebug() << "Failed to load server list: no data found";
        return false;
    }

    sendSimpleError(-32008, "CDBs not available. Loaded local server list. Update " + QDateTime::fromSecsSinceEpoch(time.toLongLong()).toString("yyyy-MM-dd HH:mm:ss"));

    sendServerList(arr, time);
    return true;
}

void DapCmdServersList::sendServerList(const QJsonArray& arr, const QString &time)
{
    QJsonObject obj;
    obj["servers"] = arr;
    obj["update_time"] = time;

    qDebug() << "Sending server list to gui";

    sendCmd(&obj);
    DapServerInfoList l;
    if (DapServerInfo::fromJSON(obj.value("servers").toArray(), l) && !l.isEmpty()){
        emit updateNodesList(l);
        emit sendCurrentServer();
    }
}

