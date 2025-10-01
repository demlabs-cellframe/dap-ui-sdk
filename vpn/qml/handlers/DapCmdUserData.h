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

#include "DapCmdClientAbstract.h"

class DapCmdUserData : public DapCmdClientAbstract
{
    Q_OBJECT
    
    /// User login.
    static const QString userParam;
    /// User password.
    static const QString passwordParam;
    /// Server address.
    static const QString addressParam;
protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;
public:
    /// Standart consructor.
    /// @param parent Parent.
    DapCmdUserData(QObject *parent = nullptr):
        DapCmdClientAbstract(DapJsonCmdType::GET_USER_DATA, parent) { }
    
    /// Process and verify command parameters.
    /// @param user User login.
    /// @param password User password.
    /// @param address Server address.
    void userDataHandler(const QJsonValue& user, const QJsonValue& password, const QJsonValue& address);
    
    void sendCmd();
signals:
    /// The signal emitted in case of successful reception and verification of user data from the service.
    /// @param user User login.
    /// @param password User password.
    /// @param address Server address.
    void sigUserData(const QString& user, const QString& password, const QString& address);
    void sigtLicenseTermTill(const QString &date);
    void sigCoutryISOReceived(const QString &date);
};

#endif // DAPCMDUSERDATAHANDLER_H
