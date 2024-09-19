#ifndef DAPGEOIP_H
#define DAPGEOIP_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QCoreApplication>
#include <QFile>
#include <QByteArray>

#include "maxminddb/maxminddb.h"

#define PATH_TO_DB ":/GeoLite2-Country.mmdb"
// #define PATH_TO_DB "/home/danilmartynenko/untitled/GeoLite2-Country.mmdb"
// #define PATH_TO_DB QCoreApplication::applicationDirPath() + "/GeoLite2-Country.mmdb"

class DapGeoIP : public QObject
{
    Q_OBJECT
public:
    explicit DapGeoIP(QObject *parent = nullptr);
    ~DapGeoIP();

    static DapGeoIP* instance();

    QString getCountryIsoCode(const QString &ipAddress);
    QString getCountryIsoCode() {
        return countryCode;
    }

signals:
    void sigCountryIsoCodeExists();

private:
    MMDB_s mmdb;
    bool isDBOpen = false;
    QString dbPath;
    QString countryCode;
    QByteArray fmem;

    void onIPReceived(QNetworkReply *reply);

    QNetworkAccessManager *manager;
};

#endif // DAPGEOIP_H
