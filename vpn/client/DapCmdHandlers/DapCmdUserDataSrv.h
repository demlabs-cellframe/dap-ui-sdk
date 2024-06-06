/****************************************************************************
**
** This file is part of the DapVpn project.
** 
** The class implements the command handler for receiving user data from 
** the service.
**
****************************************************************************/

#ifndef DapCmdUserDataSrvHANDLER_H
#define DapCmdUserDataSrvHANDLER_H

#include <QObject>
#include <QJsonValue>
#include <QDateTime>

#include "DapCmdServiceAbstract.h"

class DapCmdUserDataSrv: public DapCmdServiceAbstract
{
    Q_OBJECT
    
     /// User login.
    QString     mUser;
    /// User password.
    QString     mPassword;
    /// Server address.
    QString     m_address;
    
    quint16 m_port;
public:
    const QString address() { return  m_address; }
    quint16 port() {return m_port; }
    /// Standart constructor.
    /// @param parent Parent.
    DapCmdUserDataSrv(QObject *parent = nullptr) :
        DapCmdServiceAbstract(DapJsonCmdType::GET_USER_DATA, parent) { }
    /// Virtual destructor.
    virtual ~DapCmdUserDataSrv() override { }
    /// Process command.
    /// @param params Command parameters.
    void handle(const QJsonObject* params) override;
    /// Set user data.
    /// @param user User login.
    /// @param password User password.
    /// @param address Server address.
    void setUserData(const QString &user, const QString &password, const QString &address, quint16 a_port);
    void sendLicenseTermTill(const QString &date);
signals:
    void usrDataChanged(const QString &address, quint16 a_port);
};

#endif // DapCmdUserDataSrvHANDLER_H
