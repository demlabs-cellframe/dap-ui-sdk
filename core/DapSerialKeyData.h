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

    QDateTime activatedDate(){ return m_activatedDate;}

    void reset();

    const QDateTime & licenseTermTill() const;
    int daysLeft();
    QString daysLeftString();

signals:
    void serialKeyChanged(const QString& serialKey);
    void activationChanged(bool activation);
    void licenseTermTillChanged(const QString &days);
    void daysLeftStringChanged(const QString &days);

public slots:
    void setLicenseTermTill(const QString &a_date);

private:
    QString m_serialKey;
    bool m_isActivated;
    QDateTime m_licenseTermTill = QDateTime::currentDateTime();
    QDateTime m_activatedDate = QDateTime::currentDateTime();
};

QDataStream &operator<<(QDataStream &a_outStream, const DapSerialKeyData &a_serialKeyData);
QDataStream &operator>>(QDataStream &a_inStream, DapSerialKeyData &a_serialKeyData);

Q_DECLARE_METATYPE(DapSerialKeyData)







#endif // DAPSERIALKEYDATA_H
