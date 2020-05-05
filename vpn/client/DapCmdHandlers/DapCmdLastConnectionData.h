/****************************************************************************
**
** This file is part of the DapVpn project.
** 
** The class implements a handler for receiving data about the last 
** connection received from the service.
**
****************************************************************************/

#ifndef DapCmdLastConnectionDataHandler_H
#define DapCmdLastConnectionDataHandler_H

#include <QObject>
#include <QJsonValue>
#include "DapCmdServiceAbstract.h"

class DapCmdLastConnectionData: public DapCmdServiceAbstract
{
    Q_OBJECT

    /// Connection start time.
    QString mStartTime; 
    bool mNoReset = false;
public:
    /// Standart constructor.
    /// @param parent Parent.    
    explicit DapCmdLastConnectionData(QObject *parent = nullptr):
        DapCmdServiceAbstract(DapJsonCmdType::GET_LAST_CONNECTION_DATA, parent) { }
    /// Virtual destructor.    
    virtual ~DapCmdLastConnectionData() override { }
    /// Process command.
    /// @param params Command parameters.
    void handle(const QJsonObject* params) override;
    void dontReset(bool);
public slots:
    void saveConnectionStartTime();
};

#endif // DapCmdLastConnectionDataHandler_H
