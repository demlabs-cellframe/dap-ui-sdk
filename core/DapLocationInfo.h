#ifndef DAPLOCATIONINFO_H
#define DAPLOCATIONINFO_H
#include <QString>
#include <QMap>
#include <QDebug>

#define UNKNOWNLOCATION "UNKNOWN"
class DapLocationFields
{
public:
    DapLocationFields();
    DapLocationFields(const DapLocationFields& other);
    DapLocationFields(DapLocationFields&& other);
    DapLocationFields &operator = (const DapLocationFields &other);

    void setPicturePath(const QString& picturePath);
    QString picturePath();
protected:
    QString m_picturePath;
};


class DapLocationInfo
{
public:
    DapLocationInfo();
    void addLocation(const QString& location, const QString& picturePath = "");
    QString picturePath(const QString& location);
    bool contain(const QString& location);
    QList<QString> locations();
    int count();

protected:
    void defaultFill();
    QMap<QString, DapLocationFields> m_location;
};

#endif // DAPLOCATIONINFO_H

