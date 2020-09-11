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

    QString serialKey() const;
    void setSerialKey(const QString &a_serialKey);

    bool isActivated() const;
    void setActivated(bool a_isActivated);

    void reset();

    const QDateTime & getLicenseTermTill();
    
signals:
    void serialKeyChanged(const QString& serialKey);
    void activationChanged(bool activation);

public slots:
    void setLicenseTermTill(const QString &a_date);

private:
    QString m_serialKey;
    bool m_isActivated;
    QDateTime m_licenseTermTill;
};

QDataStream &operator<<(QDataStream &a_outStream, const DapSerialKeyData &a_serialKeyData);
QDataStream &operator>>(QDataStream &a_inStream, DapSerialKeyData &a_serialKeyData);

Q_DECLARE_METATYPE(DapSerialKeyData)







#endif // DAPSERIALKEYDATA_H
