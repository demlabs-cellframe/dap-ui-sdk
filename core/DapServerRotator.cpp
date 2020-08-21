#include "DapServerRotator.h"

DapServerRotator::DapServerRotator(QObject *a_parent)
{
}

void DapServerRotator::fillingAlternativeServersContainer()
{
    m_alternativeServersList = QVector<DapServerInfo>::fromList(DapDataLocal::serversData()->servers());
}

void DapServerRotator::dropWasteServers()
{
    qDebug() << "Drop waste servers - begin size m_alternativeServersList: " << alternativeServersList().size();
    QMutableVectorIterator<DapServerInfo> i(alternativeServersList());
    while(i.hasNext()) {
        DapServerInfo server = i.next();
        if (server.address == DapDataLocal::serversData()->currentServer()->address || server.isAuto() || isDuplicate(server)){
            qDebug() << "drop: " << server.name << " " << server.address;
            i.remove();
        }
    }
}

bool DapServerRotator::isDuplicate(const DapServerInfo m_server)
{
    for (auto a_server : alternativeServersList()){
        if (m_server.address == a_server.address && m_server.name != a_server.name)
            return true;
    }
    return false;
}

bool DapServerRotator::selectingRandomServer()
{
    if (!alternativeServersList().isEmpty()){
        int indexRandomServer = qrand() % alternativeServersList().size();
        DapDataLocal::serversData()->setCurrentServer(alternativeServersList()[indexRandomServer].name);
        qDebug() << "DapServerRotator - set current server(random): " << alternativeServersList()[indexRandomServer].name;
        return true;
    }
    return false;
}

bool DapServerRotator::selectingSameLocationServer()
{
    if (!alternativeServersList().isEmpty()){
        for (DapServerInfo &server : alternativeServersList()){
            if (server.location == DapDataLocal::serversData()->currentServer()->location){
                qDebug() << "DapServerRotator - set current server(same location): " << server.name;
                DapDataLocal::serversData()->setCurrentServer(server.name);
                return true;
            }
        }
    }
    return false;
}
