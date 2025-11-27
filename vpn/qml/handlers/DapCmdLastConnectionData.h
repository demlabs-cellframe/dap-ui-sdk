/****************************************************************************
**
** This file is part of the DapVpn project.
** 
** The class implements a handler for receiving data about the last 
** connection received from the service.
**
****************************************************************************/

#ifndef DAPCMDLASTCONNECTIONDATAHANDLER_H
#define DAPCMDLASTCONNECTIONDATAHANDLER_H

#include <QObject>
#include <QDateTime>

#include "DapCmdClientAbstract.h"

class DapCmdLastConnectionData : public DapCmdClientAbstract
{
    Q_OBJECT
    
    /// Connection start time.
    static const QString startTime;
protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;
public:
    /// Standart consructor.
    /// @param parent Parent.
    explicit DapCmdLastConnectionData(QObject *parent = nullptr) :
        DapCmdClientAbstract(DapJsonCmdType::GET_LAST_CONNECTION_DATA, parent) { }

    /// Process and verify command parameters.
    /// @param startTime Connection start time.
    void lastConnectionDataHandler(const QJsonValue& startTime);
    
    void sendCmd();
signals:
    /// The signal emitted in case of successful reception of data on the last successful connection.
    /// @param startTime Connection start time.
    void sigLastConnectionData(QDateTime startTime);    
};

#endif // DAPCMDLASTCONNECTIONDATAHANDLER_H
