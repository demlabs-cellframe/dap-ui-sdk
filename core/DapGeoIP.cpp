#include "DapGeoIP.h"
#include "DapCdbManager.h"
#include "DapServiceDataLocal.h"

#include <QDebug>

DapGeoIP::DapGeoIP(QObject *parent)
    : QObject(parent), dbPath(PATH_TO_DB), manager(new QNetworkAccessManager(this))
{
    if (!QFile::exists(dbPath)) {
        qDebug() << "Database file does not exist:" << dbPath.toStdString().c_str();
        return;
    }

    connect(manager, &QNetworkAccessManager::finished, this, &DapGeoIP::onIPReceived);

    QFile f(dbPath);
    f.open(QIODevice::ReadOnly);
    fmem = f.readAll();
    int status = MMDB_open_memory(fmem.constData(), fmem.size(), &mmdb);
    if (status == MMDB_SUCCESS) {
        isDBOpen = true;
    } else {
        qDebug() << "Can't open" << dbPath.toStdString().c_str() << "-" << MMDB_strerror(status);
    }

    DapCdbServer* server = DapCdbManager::instance().currentServer();
    if (server) {
        QUrl url("http://" + server->address + "/my_ip");
        manager->get(QNetworkRequest(url));
    } else {
        qWarning() << "[DapGeoIP] No available CDB servers, skipping IP request.";
    }
}

DapGeoIP::~DapGeoIP() {
    if (isDBOpen) {
        MMDB_close(&mmdb);
    }
}

DapGeoIP *DapGeoIP::instance()
{
    static DapGeoIP s_instance;
    return &s_instance;
}

void DapGeoIP::onIPReceived(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << "Network error: " << reply->errorString();
        emit sigCountryIsoCodeExists();
        return;
    }
    QString ip = reply->readAll();
    qDebug() << "External IP:" << ip;

    countryCode = getCountryIsoCode(ip);
    qDebug() << "Country ISO Code:" << countryCode;
    DapServiceDataLocal::instance()->setCountryISO(countryCode);
    emit sigCountryIsoCodeExists();
}

QString DapGeoIP::getCountryIsoCode(const QString &ipAddress) {

    if (!isDBOpen) {
        qDebug() << "Database is not open.";
        return "";
    }

    int gai_error, mmdb_error;
    MMDB_lookup_result_s result = MMDB_lookup_string(&mmdb, ipAddress.toStdString().c_str(), &gai_error, &mmdb_error);
    if (mmdb_error == MMDB_SUCCESS) {
        if (result.found_entry) {
            MMDB_entry_data_s entry_data;
            int status = MMDB_get_value(&result.entry, &entry_data, "country", "iso_code", NULL);
            if (status == MMDB_SUCCESS && entry_data.has_data) {
                return QString::fromUtf8(entry_data.utf8_string, entry_data.data_size);
            } else {
                qDebug() << "ISO code not found.";
            }
        } else {
            qDebug() << "No entry found for this IP address.";
        }
    } else {
        qDebug() << "Error occurred while looking up the IP address: " << MMDB_strerror(mmdb_error);
    }
    return "";
}

QString DapGeoIP::getLocationString(const QString &ipAddress) {
    if (!isDBOpen) {
        qDebug() << "Database is not open.";
        return "";
    }

    int gai_error, mmdb_error;
    MMDB_lookup_result_s result = MMDB_lookup_string(&mmdb, ipAddress.toStdString().c_str(), &gai_error, &mmdb_error);
    if (mmdb_error == MMDB_SUCCESS) {
        if (result.found_entry) {
            MMDB_entry_data_s continent_data;
            MMDB_entry_data_s country_data;
            MMDB_entry_data_s city_data;

            QString continent = "Unknown";
            QString country = "Unknown";
            QString city = "Unknown";

            int status_continent = MMDB_get_value(&result.entry, &continent_data, "continent", "names", "en", NULL);
            if (status_continent == MMDB_SUCCESS && continent_data.has_data) {
                continent = QString::fromUtf8(continent_data.utf8_string, continent_data.data_size);
            }

            int status_country = MMDB_get_value(&result.entry, &country_data, "country", "names", "en", NULL);
            if (status_country == MMDB_SUCCESS && country_data.has_data) {
                country = QString::fromUtf8(country_data.utf8_string, country_data.data_size);
            }

            int status_city = MMDB_get_value(&result.entry, &city_data, "city", "names", "en", NULL);
            if (status_city == MMDB_SUCCESS && city_data.has_data) {
                city = QString::fromUtf8(city_data.utf8_string, city_data.data_size);
            }

            return (city != "Unknown") ? QString("%1.%2.%3").arg(continent, country, city)
                                       : QString("%1.%2").arg(continent, country);
        } else {
            qDebug() << "No entry found for this IP address.";
        }
    } else {
        qDebug() << "Error occurred while looking up the IP address: " << MMDB_strerror(mmdb_error);
    }
    return "";
}
