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
    defaultFill();
}

bool DapLocationInfo::addLocation(const QString& location, const QString& picturePath)
{
//    https://stackoverflow.com/questions/35885760/is-there-any-way-to-set-a-class-in-the-qmap-value
    QString a_location = location.toUpper();
    if (a_location == "")
        a_location = UNKNOWNLOCATION;
    m_location.insert(a_location, DapLocationFiels());
    if (picturePath != "")
        m_location[a_location].setPicturePath(picturePath);
    qDebug() << "------------------ " << a_location << " : " << m_location.count();
    //DapServerLocation DapServerInfo::stringToLocation(const QString& location) {
    //    DapServerLocation v = m_countries.value(location.toUpper());
    //    if (int(v) == 0) {
    //        qWarning() << "Unknown location" << location;
    //        return DapServerLocation::UNKNOWN;
    //    }
    //    return v;
    //}
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
        return m_location[a_location].picturePath();
    else
        return "";
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

QList<QString> DapLocationInfo::locations()
{
    return m_location.keys();
}

int DapLocationInfo::count()
{
    return m_location.count();
}
