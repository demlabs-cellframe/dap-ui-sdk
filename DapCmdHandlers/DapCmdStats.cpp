#include "DapCmdStats.h"

DapCmdStats::DapCmdStats(QObject *parent) :
    DapCmdServiceAbstract(DapJsonCmdType::STATS, parent)
{

}

void DapCmdStats::handle(const QJsonObject* params)
{
    Q_UNUSED(params);

    QJsonValue val = params->value("stream");

    if(!val.isBool()) {
        qWarning() << "Bad value" << *params;
        return;
    }

    val.toBool() == true ? emit sigStreamOn() : emit sigStreamOff();
}

void DapCmdStats::sendCmdStats(qint64 readBytes, qint64 writeBytes,
                               qint64 readPackages, qint64 writePackages)
{
    static QJsonObject result;
    result["read_kbytes"] = readBytes / 1024;
    result["write_kbytes"] = writeBytes / 1024;
    result["read_package"] = readPackages;
    result["write_package"] = writePackages;
    sendCmd(&result);
}
