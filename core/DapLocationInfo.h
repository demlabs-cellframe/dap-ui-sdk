#ifndef DAPLOCATIONINFO_H
#define DAPLOCATIONINFO_H
#include <QString>
#include <QMap>
#include <QDebug>

#define UNKNOWNLOCATION "UNKNOWN"
class DapLocationFiels
{
public:
    DapLocationFiels();
    DapLocationFiels(const DapLocationFiels& other);
    DapLocationFiels(DapLocationFiels&& other);
    DapLocationFiels &operator = (const DapLocationFiels &other);

    void setPicturePath(const QString& picturePath);
    QString picturePath();
protected:
    QString m_picturePath;
};


class DapLocationInfo
{
public:
    DapLocationInfo();
    bool addLocation(const QString& location, const QString& picturePath = "");
    QString picturePath(const QString& location);
    bool contain(const QString& location);
    QList<QString> locations();
    int count();

protected:
    void defaultFill();
    QMap<QString, DapLocationFiels> m_location;
};

#endif // DAPLOCATIONINFO_H

