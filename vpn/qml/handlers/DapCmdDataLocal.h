#pragma once

#include "DapCmdClientAbstract.h"
#include <QObject>

class DapCmdDataLocal : public DapCmdClientAbstract
{
    Q_OBJECT

public:
    explicit DapCmdDataLocal (QObject *a_parent = nullptr);
    virtual ~DapCmdDataLocal() override;

    void handleResult (const QJsonObject &a_params) override;
    void handleError (int code, const QString &message) override;

    /// request value from serive
    void requestValue (const QString &a_name);
    /// request all data from service
    void requestAllData ();
    /// send value to service
    void sendValue (const QString &a_name, const QVariant &a_value);
    /// send remove value to service
    void sendRemove (const QString &a_name);

    void sendMigrate(const QJsonObject& object);
public slots:
    void dataLocalUpdated(const QJsonObject& object);

signals:
    void newDataSignal(const QJsonObject& object);

    /// service sent value
    void sigGotValue (QString a_name, QVariant a_value, const int a_msgId);
    /// service sent all data
    void sigGotAllData ();
    /// service sent remove value
    void sigGotRemove (QString a_name, const int a_msgId);

private:
    bool m_isAllData = false;
};
