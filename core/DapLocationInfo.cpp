#include "DapLocationInfo.h"

DapLocationFiels::DapLocationFiels()
    : m_picturePath("")
{
}

DapLocationFiels::DapLocationFiels(const DapLocationFiels& other)
{
    this->m_picturePath = other.m_picturePath;
}

DapLocationFiels::DapLocationFiels(DapLocationFiels&& other)
{
    this->m_picturePath = other.m_picturePath;
    other.m_picturePath = "";
}

DapLocationFiels& DapLocationFiels::operator = (const DapLocationFiels &other)
{
    if (this == &other)
        return *this;
    this->m_picturePath = other.m_picturePath;
    return *this;
}

void DapLocationFiels::setPicturePath(const QString& picturePath)
{
    m_picturePath = picturePath;
}

QString DapLocationFiels::picturePath()
{
    return m_picturePath;
}




DapLocationInfo::DapLocationInfo()
{
}

void DapLocationInfo::addLocation(const QString& location, const QString& picturePath)
{
    QString a_location = location.toUpper();
    if (a_location == "")
    {
        qWarning() << "Unknown location" << location;
        a_location = UNKNOWNLOCATION;
    }
    if (!m_location.contains(a_location))
        m_location.insert(a_location, DapLocationFiels());
    m_location[a_location].setPicturePath(picturePath);
}

bool DapLocationInfo::contain(const QString &location)
{
    QString a_location = location.toUpper();
    return m_location.contains(a_location);
}

QString DapLocationInfo::picturePath(const QString& location)
{
    QString a_location = location.toUpper();
    if (m_location.contains(a_location))
    {
        QString path = m_location[a_location].picturePath();
        if (path == "")
            qWarning() << "Not found picture for current location. Return default!";
        return path;
    }
    else
    {
        qWarning() << "Not found location. Return default!";
        return "";
    }
}

QList<QString> DapLocationInfo::locations()
{
    return m_location.keys();
}

int DapLocationInfo::count()
{
    return m_location.count();
}

void DapLocationInfo::defaultFill()
{
//    addLocation(UNKNOWNLOCATION, ":/country/Flag_gf.png");
//    addLocation("ENGLAND", ":/country/GB.png");
//    addLocation("FRANCE", ":/country/FR.png");
//    addLocation("GERMANY", ":/country/DE.png");
//    addLocation("USA", ":/country/US.png");
//    addLocation("NETHERLANDS", ":/country/NL.png");
//    addLocation("RUSSIA", ":/country/RU.png");
//    addLocation("UKRAINE", ":/country/UA.png");
//    addLocation("Netherlands", ":/country/NL.png");
//    addLocation("Singapore", ":/country/SG.png");
}
