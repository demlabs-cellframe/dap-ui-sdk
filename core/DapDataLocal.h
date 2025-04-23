#pragma once

#include "DapBaseDataLocal.h"

class DapDataLocal : public DapBaseDataLocal
{
    Q_OBJECT
    DapDataLocal();
public:
    static DapDataLocal* instance();

    QVariant getValueSetting(const QString &a_setting) override;
    void saveValueSetting(const QString &setting, const QVariant &value) override;
    void removeValueSetting(const QString &setting) override;

    static QVariant getSetting (const QString& a_setting);
    static void     saveSetting(const QString& a_setting, const QVariant& a_value);
    static void     removeSetting(const QString& a_setting);

    void setCountryISO(const QString& iso_code) override;
    void saveSerialKeyData() override;
    void resetSerialKeyData() override;
    void saveKeysHistory() override;
    void saveBugReport() override;

    void savePendingSerialKey(QString a_serialkey) override;
    void setSettings(const QJsonObject &json) override;

    bool isNeedMigrate() const { return m_needMigration; }

    void saveEncryptedSetting(const QString &a_setting, const QVariant &a_value) override;
    void saveEncryptedSetting(const QString &a_setting, const QByteArray &a_value) override;
    bool loadEncryptedSettingString(const QString &a_setting, QByteArray& a_outString) override;
public slots:
    void setLogin(const QString &login) override;
    void setPassword(const QString &password) override;
    void saveAuthorizationData() override;

    void dataFromCommand(const QJsonObject& object);

signals:
    void valueDataLocalUpdated(const QJsonObject& object);
    void serialKeyDataUpdateFromService();

    void allDataReceived();
    void reMigrationSignal();
private:
    QVariantMap m_settingsMap;

    bool m_needMigration = false;
};
