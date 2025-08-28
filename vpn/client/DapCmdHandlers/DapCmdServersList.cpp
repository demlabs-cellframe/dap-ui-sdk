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
    qDebug() << "[SVC][ServersList] handle() GET_SERVERS_LIST received";
    m_errorSentOnce = false; // reset once-per-session per command invocation
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

            qCritical() << "[SVC][ServersList] Can't parse server response to JSON:" << jsonErr.errorString() << " pos=" << jsonErr.offset;
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
                qDebug() << "[SVC][ServersList] Server list array size=" << arr.size();

                updateServerList(arr);
            }
        }
    });

    connect(reply, &DapNetworkReply::sigError, this, [=] {

        if (!reply) return;

        qDebug() << "[SVC][ServersList] Network error occurred: code=" << reply->error() << ", str=" << reply->errorString() << ", guiCall=" << guiCall;

        if (guiCall) {
            if (loadServerList()){
                guiCall = false;
                qDebug() << "[SVC][ServersList] Loaded local server list, guiCall=false";
            }
        }

        if (!emitTimer->isActive()) {
            qDebug() << "[SVC][ServersList] Starting emitTimer(5000ms) for next retry";
            emitTimer->start();
        }

        emit nextCdb();

        if (!m_errorSentOnce) {
            qDebug() << "[SVC][ServersList] sendSimpleError(" << reply->error() << ", '" << reply->errorString() << "') [first only]";
            sendSimpleError(reply->error(), reply->errorString());
            m_errorSentOnce = true;
        } else {
            qDebug() << "[SVC][ServersList] suppressing repeated error emission (already sent once)";
        }
    });

    auto it = DapServiceDataLocal::instance()->getCdbIterator();
    qDebug() << "[SVC][ServersList] sendRequestToCDB(guiCall=" << guiCall << ") target=" << it->address << ":" << it->port;
    DapConnectClient::instance()->request_GET (it->address, it->port, "nodelist", *reply);
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

