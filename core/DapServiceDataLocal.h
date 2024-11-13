#pragma once

#include "DapBaseDataLocal.h"

class DapServiceDataLocal : public DapBaseDataLocal
{
    Q_OBJECT

    DapServiceDataLocal();
public:
    static DapServiceDataLocal *instance();

    void saveValueSetting(const QString &setting, const QVariant &value) override;
    void removeValueSetting(const QString &setting) override;

    static QVariant getSetting (const QString& a_setting);
    static void     saveSetting(const QString& a_setting, const QVariant& a_value);
    static void     removeSetting(const QString& a_setting);

    void setCountryISO(const QString& iso_code) override;
    void saveSerialKeyData() override;
    void resetSerialKeyData() override;

    void savePendingSerialKey(QString a_serialkey) override;

    void setMigrationInfo(const QJsonObject& object);
public slots:
    void setLogin(const QString &login) override;
    void setPassword(const QString &password) override;

    void addNewSerialKey() override;

protected:
    void setBugReportHistory(const QJsonArray& list) override;
signals:
    void valueServiceDataLocalUpdated(const QJsonObject& object);
};

