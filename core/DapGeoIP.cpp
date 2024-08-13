#include "DapGeoIP.h"
#include "DapDataLocal.h"

#include <QDebug>

DapGeoIP::DapGeoIP(QObject *parent)
    : QObject(parent), dbPath(PATH_TO_DB), manager(new QNetworkAccessManager(this))
{
    if (QFile::exists(dbPath)) {

        connect(manager, &QNetworkAccessManager::finished, this, &DapGeoIP::onIPReceived);

        QFile f(dbPath);
        f.open(QIODevice::ReadOnly);
        fmem = f.readAll();
        int status = MMDB_open_memory(fmem.constData(), fmem.size(), &mmdb);
        if (status == MMDB_SUCCESS) {
            isDBOpen = true;
        } else {
            qDebug() << "Can't open " << dbPath.toStdString().c_str() << " - " << MMDB_strerror(status);
        }

        //manager->get(QNetworkRequest(QUrl("http://api.ipify.org")));

        /* get cdb address */
        auto address  = DapDataLocal::instance()->m_cdbIter->address;
        auto url      = QUrl ("http://" + address + "/my_ip");

        /* send request */
        manager->get (QNetworkRequest (url));

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
        emit sigCountryIsoCodeExists();
        return;
    }
    QString ip = reply->readAll();
    qDebug() << "External IP:" << ip;

    countryCode = getCountryIsoCode(ip);
    qDebug() << "Country ISO Code:" << countryCode;
    DapDataLocal::instance()->setCountryISO(countryCode);
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