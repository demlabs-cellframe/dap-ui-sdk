#include <QDebug>
#include <QDateTime>


#include "DapCmdServersList.h"
#include "DapJsonParams.h"
#include "DapConnectClient.h"
#include "DapDataLocal.h"
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
            if (++DapDataLocal::instance()->m_cdbIter == DapDataLocal::instance()->cdbServersList().end()) {
                DapDataLocal::instance()->m_cdbIter = DapDataLocal::instance()->cdbServersList().begin();

                if (loadServerList())
                    return;

            }
            sendSimpleError(-32001, "Bad response from server. Parse error");

        } else {
            auto arr = jsonDoc.array();
            if (arr.isEmpty()) {
                //emit nextCdb();
                if (++DapDataLocal::instance()->m_cdbIter == DapDataLocal::instance()->cdbServersList().end()) {
                    DapDataLocal::instance()->m_cdbIter = DapDataLocal::instance()->cdbServersList().begin();

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

    auto it = DapDataLocal::instance()->m_cdbIter;
    qDebug() << "Sending request to: " << it->address << ":" << it->port;
    DapConnectClient::instance()->request_GET (it->address, it->port, "nodelist", *reply);
}



void DapCmdServersList::updateServerList(const QJsonArray& arr)
{
    QString time = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());

    DapDataLocal::instance()->saveToSettings("last_nodelist_update", arr);
    DapDataLocal::instance()->saveToSettings("last_nodelist_update_time", time);

    qDebug() << "Saved updated server list at " << QDateTime::fromSecsSinceEpoch(time.toLongLong()).toString("yyyy-MM-dd HH:mm:ss");

    sendServerList(arr, time);

}

bool DapCmdServersList::loadServerList()
{
    QJsonArray arr;
    QString time;
    DapDataLocal::instance()->loadFromSettings("last_nodelist_update", arr);
    DapDataLocal::instance()->loadFromSettings("last_nodelist_update_time", time);

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

