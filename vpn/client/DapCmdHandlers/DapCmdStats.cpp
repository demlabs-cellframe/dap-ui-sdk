#include "DapCmdStats.h"

DapCmdStats::DapCmdStats(QObject *parent) :
    DapCmdServiceAbstract(DapJsonCmdType::STATS, parent)
{

}

void DapCmdStats::handle(const QJsonObject* params)
{
    DapCmdServiceAbstract::handle(params);

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
    result["read_bytes"]    = QString::number(readBytes);
    result["write_bytes"]   = QString::number(writeBytes);
    result["read_package"]  = QString::number(readPackages);
    result["write_package"] = QString::number(writePackages);
    sendCmd(&result);
}
