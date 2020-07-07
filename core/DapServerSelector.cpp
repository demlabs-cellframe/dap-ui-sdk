#include "DapServerSelector.h"

DapServerSelector::DapServerSelector()
{

}

bool DapServerSelector::question(QString title, QString message)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, title, message, QMessageBox::Yes|QMessageBox::No);
    return reply == QMessageBox::Yes;
}

bool DapServerSelector::newServerAfterNetworkError()
{
    if (DapDataLocal::me()->m_serversForCheck.isEmpty())
        for (auto server : DapDataLocal::me()->servers())
            DapDataLocal::me()->m_serversForCheck.push_back(server);

    DapServerInfo *currentServer = DapDataLocal::me()->currentServer();

    QMutableVectorIterator<DapServerInfo> i(DapDataLocal::me()->m_serversForCheck);
    while(i.hasNext()) {
        DapServerInfo temp = i.next();
        if (temp.address == currentServer->address || temp.location == DapServerLocation::UNKNOWN)
            i.remove();
    }

    if (DapDataLocal::me()->m_serversForCheck.isEmpty()){
        QMessageBox::information(this, "Network Error", "No servers available. Contact support or send bug-report.");
        return false;
    }

    if (DapDataLocal::me()->bSelectedAutoServer) {
        for (DapServerInfo& server : DapDataLocal::me()->m_serversForCheck){
            if (currentServer->address != server.address){
                DapDataLocal::me()->setCurrentServer(&server);
                return true;
            }
        }
    }
    else {
        for (DapServerInfo& server : DapDataLocal::me()->m_serversForCheck){
            if (server.location == currentServer->location && currentServer->address != server.address){
                DapDataLocal::me()->setCurrentServer(&server);
                return true;
            }
        }
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Network Error", "Failed to pick up a server from the same region. Extend your search?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
            for (DapServerInfo& server : DapDataLocal::me()->m_serversForCheck){
                if (currentServer->address != server.address){
                    DapDataLocal::me()->setCurrentServer(&server);
                    return true;
                }
            }
    }
    QMessageBox::information(this, "Network Error", "No servers available. Contact support or send bug-report.");
    return false;
}
