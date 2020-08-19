#include "DapSerialKeyData.h"

DapSerialKeyData::DapSerialKeyData(QObject *a_parent)
    :QObject(a_parent)
{
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

void DapSerialKeyData::setActivated(bool isActivated)
{
    m_isActivated = isActivated;
}
