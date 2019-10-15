/****************************************************************************
**
** This file is part of the DapVpn project.
** 
** The class implements the command handler for receiving user data from 
** the service.
**
****************************************************************************/

#ifndef DAPCMDUSERDATAHANDLER_H
#define DAPCMDUSERDATAHANDLER_H

#include <QObject>
#include <QJsonValue>

#include "DapCmdServiceAbstract.h"

class DapCmdUserData: public DapCmdServiceAbstract
{
    Q_OBJECT
    
     /// User login.
    QString     mUser;
    /// User password.
    QString     mPassword;
    /// Server address.
    QString     mAddress;
    
public:
    /// Standart constructor.
    /// @param parent Parent.
    DapCmdUserData(QObject *parent = nullptr) :
        DapCmdServiceAbstract(DapJsonCmdType::GET_USER_DATA, parent) { }
    /// Virtual destructor.
    virtual ~DapCmdUserData() override { }
    /// Process command.
    /// @param params Command parameters.
    void handle(const QJsonObject* params) override;
    /// Set user data.
    /// @param user User login.
    /// @param password User password.
    /// @param address Server address.
    void setUserData(const QString &user, const QString &password, const QString &address);
};

#endif // DAPCMDUSERDATAHANDLER_H
