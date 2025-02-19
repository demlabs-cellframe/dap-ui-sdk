#include "DapSerialKeyData.h"

#include "DapBaseDataLocal.h"

DapSerialKeyData::DapSerialKeyData(QObject *a_parent)
    : QObject(a_parent)
    , m_isActivated(false)
{
}

DapSerialKeyData::DapSerialKeyData(const DapSerialKeyData &a_another):QObject()
{
    m_serialKey = a_another.serialKey();
    m_isActivated = a_another.isActivated();
    m_licenseTermTill = a_another.licenseTermTill();
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

    emit serialKeyChanged(m_serialKey);
}

void DapSerialKeyData::userSerialKeyEntered(const QString &a_serialKey)
{
    if(m_serialKey != a_serialKey)
    {
        setSerialKey(a_serialKey);
        emit serialKeyToSave();
    }
}

bool DapSerialKeyData::isActivated() const
{
    return m_isActivated;
}

void DapSerialKeyData::setActivated(bool a_isActivated)
{
    if (m_isActivated == a_isActivated)
        return;
    if (m_serialKey.isEmpty() && a_isActivated)
        return;
    m_isActivated = a_isActivated;

    emit activationChanged(a_isActivated);
}

void DapSerialKeyData::reset()
{
    this->setActivated(false);
    this->setSerialKey("");
}

const QDateTime &DapSerialKeyData::licenseTermTill() const
{
    return m_licenseTermTill;
}

int DapSerialKeyData::daysLeft()
{
    if (this->licenseTermTill().toTime_t() == 0)
        return -1;

    return QDateTime::currentDateTime().daysTo(this->licenseTermTill());
}

QString DapSerialKeyData::daysLeftString()
{
    QString text;
    if (m_isActivated)
    {
        int days = this->daysLeft();
        switch (days) {
            case -1:
                return QObject::tr("Unlimited");
            case 1:
                return QObject::tr("%1 day left").arg(days);
            default:
                return QObject::tr("%1 days left").arg(days);
        }
    }
    else
    {
        return QString("");
    }
}

void DapSerialKeyData::setLicenseTermTill(const QString &a_date)
{
    QDateTime tempDate = QDateTime::fromTime_t(a_date.toUInt());

    this->m_licenseTermTill = tempDate;

    emit this->daysLeftStringChanged(this->daysLeftString());
}

void DapSerialKeyData::operator=(const DapSerialKeyData &a_another)
{
    this->setSerialKey(a_another.serialKey());
    this->setActivated(a_another.isActivated());
    this->setLicenseTermTill(QString::number(a_another.licenseTermTill().toTime_t()));
}

QDataStream &operator<<(QDataStream &a_outStream, const DapSerialKeyData &a_serialKeyData)
{
    a_outStream << a_serialKeyData.serialKey() << DapUtils::toByteArray(a_serialKeyData.isActivated())
                << QString::number(a_serialKeyData.licenseTermTill().toTime_t());
    return a_outStream;
}

QDataStream &operator>>(QDataStream &a_inStream, DapSerialKeyData &a_serialKeyData)
{
    QString serialKey;
    QByteArray isActivated;
    QString licenseTermTill;

    a_inStream >> serialKey >> isActivated >> licenseTermTill;

    a_serialKeyData.setSerialKey(serialKey);
    a_serialKeyData.setActivated(DapUtils::fromByteArray<bool>(isActivated));
    a_serialKeyData.setLicenseTermTill(licenseTermTill);

    return a_inStream;
}
