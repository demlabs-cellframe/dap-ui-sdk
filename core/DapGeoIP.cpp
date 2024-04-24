#include "DapGeoIP.h"
#include <QFile>
#include <QDebug>

DapGeoIP::DapGeoIP(QObject *parent)
    : QObject(parent), dbPath(PATH_TO_DB), manager(new QNetworkAccessManager(this))
{
    if (QFile::exists(dbPath)) {

        connect(manager, &QNetworkAccessManager::finished, this, &DapGeoIP::onIPReceived);

        int status = MMDB_open(dbPath.toStdString().c_str(), MMDB_MODE_MMAP, &mmdb);
        if (status == MMDB_SUCCESS) {
            isDBOpen = true;
        } else {
            qDebug() << "Can't open " << dbPath.toStdString().c_str() << " - " << MMDB_strerror(status);
        }

        manager->get(QNetworkRequest(QUrl("http://api.ipify.org")));

    } else {
        qDebug() << "Database file does not exist: " << dbPath.toStdString().c_str();
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
        return;
    }
    QString ip = reply->readAll();
    qDebug() << "External IP:" << ip;

    countryCode = getCountryIsoCode(ip);
    qDebug() << "Country ISO Code:" << countryCode;
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