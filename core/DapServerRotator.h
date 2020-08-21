#ifndef DAPSERVERROTATOR_H
#define DAPSERVERROTATOR_H

#include "DapDataLocal.h"
//#include <QWidget>
//#include <QMessageBox>
#include <QtWidgets/QMessageBox>

class DapServerRotator : public QObject
{
public:

    DapServerRotator(QObject* a_parent = nullptr);

    bool selectingRandomServer();
    bool selectingSameLocationServer();
    void dropWasteServers();
    void fillingAlternativeServersContainer();

    void setSameLocationSearchAccepted(bool value){ m_sameLocationSearchAccepted = value; };
    void setAnotherLocationSearchAccepted(bool value){ m_anotherLocationSearchAccepted = value; };
    bool sameLocationSearchAccepted(){return m_sameLocationSearchAccepted;};
    bool anotherLocationSearchAccepted(){return m_anotherLocationSearchAccepted;};

    QVector<DapServerInfo>& alternativeServersList(){ return m_alternativeServersList; }

private:

    bool isDuplicate(const DapServerInfo m_server);

    QVector<DapServerInfo> m_alternativeServersList;

    bool m_sameLocationSearchAccepted = false;
    bool m_anotherLocationSearchAccepted = false;

};

#endif // DAPSERVERROTATOR_H
