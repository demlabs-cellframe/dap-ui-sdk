#ifndef DAPSERIALKEYDATA_H
#define DAPSERIALKEYDATA_H

#include <QObject>
#include <QDateTime>

#include "DapServerInfo.h"

class DapSerialKeyData: public QObject
{
    Q_OBJECT
public:
    DapSerialKeyData(QObject* a_parent = nullptr);

    DapSerialKeyData(const DapSerialKeyData& a_another);
    void operator=(const DapSerialKeyData& a_another);
    bool operator==(const DapSerialKeyData& a_another){
        return m_serialKey == a_another.m_serialKey;
    }

    QString serialKey() const;
    void setSerialKey(const QString &a_serialKey);

    bool isActivated() const;
    void setActivated(bool a_isActivated);

    void setDateActivate(const QDateTime& time) { m_activatedDate = time; }
    QDateTime activatedDate() const { return m_activatedDate;}

    void reset();

    const QDateTime & licenseTermTill() const;
    int daysLeft();
    QString daysLeftString();

    void setFromJson(const QJsonObject& jsonData);

public slots:
    void userSerialKeyEntered(const QString &a_serialKey);

signals:
    void serialKeyChanged(const QString& serialKey);
    void serialKeyToSave();
    void activationChanged(bool activation);
    void licenseTermTillChanged(const QString &days);
    void daysLeftStringChanged(const QString &days);
    void sigSerialKeyIsSet();

public slots:
    void setLicenseTermTill(const QString &a_date);

private:
    QString m_serialKey;
    bool m_isActivated;
    QDateTime m_licenseTermTill = QDateTime::fromTime_t(0);
    QDateTime m_activatedDate = QDateTime::currentDateTime();

protected:
    const QString JSON_SERIAL_KEY_KEY = "serialKey";
    const QString JSON_IS_ACTIVATED_KEY = "isActivated";
    const QString JSON_LISENSE_TIME_KEY = "lisenseTime";
    const QString JSON_DAYS_LEFT_STRING_KEY = "daysLeftString";
    const QString JSON_SERIAL_KEY_DATA_KEY = "serialKeyData";
    const QString JSON_SERIAL_KEY_DATA_LIST_KEY = "serialKeyDataList";

};

QDataStream &operator<<(QDataStream &a_outStream, const DapSerialKeyData &a_serialKeyData);
QDataStream &operator>>(QDataStream &a_inStream, DapSerialKeyData &a_serialKeyData);

Q_DECLARE_METATYPE(DapSerialKeyData)

#endif // DAPSERIALKEYDATA_H
