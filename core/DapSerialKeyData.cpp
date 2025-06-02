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

void DapSerialKeyData::setFromJson(const QJsonObject& jsonData)
{
    if (jsonData.contains(JSON_SERIAL_KEY_KEY) && jsonData[JSON_SERIAL_KEY_KEY].isString()) {
        setSerialKey(jsonData[JSON_SERIAL_KEY_KEY].toString());
    }

    if (jsonData.contains(JSON_IS_ACTIVATED_KEY) && jsonData[JSON_IS_ACTIVATED_KEY].isBool()) {
        setActivated(jsonData[JSON_IS_ACTIVATED_KEY].toBool());
    }

    if (jsonData.contains(JSON_LISENSE_TIME_KEY) && jsonData[JSON_LISENSE_TIME_KEY].isString()) {
        setLicenseTermTill(QString::number(jsonData[JSON_LISENSE_TIME_KEY].toString().toLongLong()));
    }

    emit sigSerialKeyIsSet();
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
    this->m_licenseTermTill = QDateTime::fromTime_t(0);
}

const QDateTime &DapSerialKeyData::licenseTermTill() const
{
    return m_licenseTermTill;
}

int DapSerialKeyData::daysLeft()
{
    return QDateTime::currentDateTime().daysTo(this->licenseTermTill());
}

QString DapSerialKeyData::daysLeftString()
{
    qDebug() << "[daysLeftString] Method called";

    if (m_licenseTermTill.toSecsSinceEpoch() == 0){
      qDebug() << "[daysLeftString] Expired license";
      return QObject::tr("");
    }else if (m_licenseTermTill.toSecsSinceEpoch() == -1){
      qDebug() << "[daysLeftString] Unlimited license";
      return QObject::tr("Unlimited");
    }

    int days = this->daysLeft();
    qDebug() << "[daysLeftString] Days left:" << days;

    if (days < 0){
      qDebug() << "[daysLeftString] Expired license";
      return QObject::tr("");
    } else if (days == 1){
      qDebug() << "[daysLeftString] 1 day left";
      return QObject::tr("%1 day left").arg(days);
    } else {
      qDebug() << "[daysLeftString] Multiple days left:" << days;
      return QObject::tr("%1 days left").arg(days);
    }
}

void DapSerialKeyData::setLicenseTermTill(const QString &a_date)
{
    qDebug() << "[setLicenseTermTill] Received a_date:" << a_date;

    QDateTime tempDate;
    int intDate = a_date.toInt();

    tempDate.setSecsSinceEpoch(intDate);

    qDebug() << "[setLicenseTermTill] Converted a_date to QDateTime:" << tempDate;

    if (tempDate == this->m_licenseTermTill){
        qDebug() << "[setLicenseTermTill] Converted a_date the same";
        return;
    }

    this->m_licenseTermTill = tempDate;
    qDebug() << "[setLicenseTermTill] Updated m_licenseTermTill to:" << this->m_licenseTermTill;

    QString daysLeft = this->daysLeftString();
    qDebug() << "[setLicenseTermTill] Emitting daysLeftStringChanged with value:" << daysLeft;
    emit this->daysLeftStringChanged(daysLeft);
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
