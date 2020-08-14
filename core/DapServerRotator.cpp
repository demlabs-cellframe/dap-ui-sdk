#include "DapServerRotator.h"

DapServerRotator::DapServerRotator(QWidget *a_parent)
{
    qDebug() << "DapServerRotator";
    fillingServersContainer();
}

void DapServerRotator::fillingServersContainer()
{
    qDebug() << "fillingServersContainer";
    if (DapDataLocal::instance()->m_serversForCheck.isEmpty())
        for (auto server : DapDataLocal::serversData()->servers())
            DapDataLocal::instance()->m_serversForCheck.push_back(server);
    dropWasteServers();
}

void DapServerRotator::dropWasteServers()
{
    qDebug() << "dropWasteServers - begin size m_serversForCheck: " << DapDataLocal::instance()->m_serversForCheck.size();
    QMutableVectorIterator<DapServerInfo> i(DapDataLocal::instance()->m_serversForCheck);
    while(i.hasNext()) {
        DapServerInfo tempServer = i.next();
        //drop current server and auto
        if (tempServer.address == DapDataLocal::serversData()->currentServer()->address || tempServer.isAuto() || isDuplicate(tempServer)){
            qDebug() << "dropWasteServers - drop: " << tempServer.name << " " << tempServer.address;
            i.remove();
        }
    }
    qDebug() << "dropWasteServers - end size m_serversForCheck: " << DapDataLocal::instance()->m_serversForCheck.size();
}

bool DapServerRotator::isDuplicate(const DapServerInfo m_server)
{
    for (auto a_server : DapDataLocal::instance()->m_serversForCheck){
        if (m_server.address == a_server.address && m_server.name != a_server.name)
            return true;
    }
    return false;
}

bool DapServerRotator::selectingRandomServer()
{
    qDebug() << "selectingRandomServer";
    dropWasteServers();

    if (!DapDataLocal::instance()->m_serversForCheck.isEmpty()){
        int indexRandomServer = qrand() % ((DapDataLocal::instance()->m_serversForCheck.size() + 1) - 0) + 0;
        DapDataLocal::serversData()->setCurrentServer(DapDataLocal::instance()->m_serversForCheck[indexRandomServer].name);
        qDebug() << "indexRandomServer " << indexRandomServer;
        return true;
    }
    return false;
}

bool DapServerRotator::selectingSameLocationServer()
{
    dropWasteServers();

    if (!DapDataLocal::instance()->m_serversForCheck.isEmpty()){
        for (DapServerInfo& server : DapDataLocal::instance()->m_serversForCheck){
            if (server.location == DapDataLocal::serversData()->currentServer()->location){
                DapDataLocal::serversData()->setCurrentServer(server.name);
                return true;
            }
        }
    }
    return false;
}
