#include "DapServerRotator.h"

DapServerRotator::DapServerRotator(QWidget *a_parent)
{
    qDebug() << "DapServerRotator";
    fillingServersContainer();
}

void DapServerRotator::fillingServersContainer()
{
    qDebug() << "fillingServersContainer";
    if (DapDataLocal::serversData()->serversForCheck().isEmpty())
        for (auto server : DapDataLocal::serversData()->servers())
            DapDataLocal::serversData()->serversForCheck().push_back(server);
    dropWasteServers();
}

void DapServerRotator::dropWasteServers()
{
    qDebug() << "dropWasteServers - begin size m_serversForCheck: " << DapDataLocal::serversData()->serversForCheck().size();
    QMutableVectorIterator<DapServerInfo> i(DapDataLocal::serversData()->serversForCheck());
    while(i.hasNext()) {
        DapServerInfo server = i.next();
        //drop current server and auto
        if (server.address == DapDataLocal::serversData()->currentServer()->address || server.isAuto() || isDuplicate(server)){
            qDebug() << "dropWasteServers - drop: " << server.name << " " << server.address;
            i.remove();
        }
    }
    qDebug() << "dropWasteServers - end size m_serversForCheck: " << DapDataLocal::serversData()->serversForCheck().size();
}

bool DapServerRotator::isDuplicate(const DapServerInfo m_server)
{
    for (auto a_server : DapDataLocal::serversData()->serversForCheck()){
        if (m_server.address == a_server.address && m_server.name != a_server.name)
            return true;
    }
    return false;
}

bool DapServerRotator::selectingRandomServer()
{
    qDebug() << "selectingRandomServer";

    if (!DapDataLocal::serversData()->serversForCheck().isEmpty()){
        int indexRandomServer = qrand() % ((DapDataLocal::serversData()->serversForCheck().size() + 1) - 0) + 0;
        DapDataLocal::serversData()->setCurrentServer(DapDataLocal::serversData()->serversForCheck()[indexRandomServer].name);
        qDebug() << "indexRandomServer " << DapDataLocal::serversData()->serversForCheck()[indexRandomServer].name;
        return true;
    }
    qDebug() << " | selectingRandomServer - return false";
    return false;
}

bool DapServerRotator::selectingSameLocationServer()
{   qDebug() << "selectingSameLocationServer";

    if (!DapDataLocal::serversData()->serversForCheck().isEmpty()){
        for (DapServerInfo &server : DapDataLocal::serversData()->serversForCheck()){
            qDebug() << "for server: " << server.name;
            if (server.location == DapDataLocal::serversData()->currentServer()->location){
                qDebug() << "serversData()->setCurrentServer: " << server.name;
                DapDataLocal::serversData()->setCurrentServer(server.name);
                return true;
            }
        }
    }
    else
        qDebug() << "serversForCheck isEmpty";
    qDebug() << " | selectingSameLocationServer - return false";
    return false;
}
