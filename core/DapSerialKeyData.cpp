#include "DapSerialKeyData.h"

#include "DapDataLocal.h"

DapSerialKeyData::DapSerialKeyData(QObject *a_parent)
    : QObject(a_parent)
    , m_isActivated(false)
{
}

DapSerialKeyData::DapSerialKeyData(const DapSerialKeyData &a_another):QObject()
{
    m_serialKey = a_another.serialKey();
    m_isActivated = a_another.isActivated();
}

QString DapSerialKeyData::serialKey() const
{
    return m_serialKey;
}

void DapSerialKeyData::setSerialKey(const QString &a_serialKey)
{
    if (m_serialKey == a_serialKey)
        return;
    m_serialKey = a_serialKey;

    emit this->serialKeyChanged(m_serialKey);
}

bool DapSerialKeyData::isActivated() const
{
    return m_isActivated;
}

void DapSerialKeyData::setActivated(bool a_isActivated)
{
    if (m_isActivated == a_isActivated)
        return;
    m_isActivated = a_isActivated;

    emit activationChanged(a_isActivated);
}

void DapSerialKeyData::reset()
{
    this->setSerialKey("");
    this->setActivated(false);
}

const void DapSerialKeyData::setLicenseTermTill(const QString &a_date)
{
    QDateTime tempDate = QDateTime::fromString(a_date);
    if (this->m_licenseTermTill == tempDate)
        return;
    this->m_licenseTermTill = tempDate;

//    emit this->licenseTermTillChanged(QString::number(tempDate.toTime_t()));
}

const QDateTime & DapSerialKeyData::getLicenseTermTill() {
//    QDateTime dsf = m_licenseTermTill;
    return m_licenseTermTill;
}

void DapSerialKeyData::operator=(const DapSerialKeyData &a_another)
{
    this->setSerialKey(a_another.serialKey());
    this->setActivated(a_another.isActivated());
}

QDataStream &operator<<(QDataStream &a_outStream, const DapSerialKeyData &a_serialKeyData)
{
    a_outStream << a_serialKeyData.serialKey() << DapUtils::toByteArray(a_serialKeyData.isActivated());
    return a_outStream;
}

QDataStream &operator>>(QDataStream &a_inStream, DapSerialKeyData &a_serialKeyData)
{
    QString serialKey;
    QByteArray isActivated;

    a_inStream >> serialKey >> isActivated;

    a_serialKeyData.setSerialKey(serialKey);
    a_serialKeyData.setActivated(DapUtils::fromByteArray<bool>(isActivated));

    return a_inStream;
}
