#pragma once

#include "DapCmdServiceAbstract.h"
#include <QObject>

class DapCmdDataLocalSrv : public DapCmdServiceAbstract
{
    Q_OBJECT

public:
    explicit DapCmdDataLocalSrv (QObject *a_parent = nullptr);
    virtual ~DapCmdDataLocalSrv() override;

    /// Process command.
    /// @param params Command parameters.
    void handle (const QJsonObject *a_params) override;

    /// send copy of value to client
    void sendValue (const QString &a_name, const QVariant &a_value);
    /// send copy of all data to client
    void sendAllData (const QJsonObject &a_data);
    /// send remove value to client
    void sendRemove (const QString &a_name);

public slots:
    void serviceDataLocalUpdated(const QJsonObject& object);
signals:

    /// client requested all data
    void sigGetAllData (QString a_clientType);

    /// client wants to remove a value
    void sigRemoveValue (QString a_name);


private:
    void getValue(const QString& nameValue);
    void setValue(const QString& nameValue, const QVariant& value);
    void remove(const QString& nameValue);
    void getAllData();
};

