#include "DapCmdStats.h"
#include <utility>

const QString DapCmdStats::readBytesParam =
        DapJsonParams::toString(DapJsonParams::READ_BYTES);

const QString DapCmdStats::writeBytesParam =
        DapJsonParams::toString(DapJsonParams::WRITE_BYTES);

const QString DapCmdStats::readPackageParam =
        DapJsonParams::toString(DapJsonParams::READ_PACKAGE);

const QString DapCmdStats::writePackageParam =
        DapJsonParams::toString(DapJsonParams::WRITE_PACKAGE);

DapCmdStats::DapCmdStats(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::STATS, parent)
{
    
}

void DapCmdStats::sendCmdStatsTrafficOn(bool val)
{
    QJsonObject obj;
    obj["stream"] = val;
    sendCmd(&obj);
}

void DapCmdStats::trafficStatsHandler(const QJsonValue& readBytes, const QJsonValue& writeBytes,
        const QJsonValue& readPackage, const QJsonValue& writePackage)
{
    int bRead = readBytes.toInt(-1),
        bWrite = writeBytes.toInt(-1),
        pRead = readPackage.toInt(-1),
        pWrite = writePackage.toInt(-1);
    
    if (bRead == -1 || bWrite == -1 || pRead == -1 || pWrite == -1) {
        qWarning() << "Something wrong can't parse traffic stats";
        return;
    }
    emit sigReadWriteBytesStat(bRead, bWrite, pRead, pWrite);
}

void DapCmdStats::handleResult(const QJsonObject& result)
{
    if (result.value(readBytesParam) != QJsonValue::Undefined &&
            result.value(writeBytesParam) != QJsonValue::Undefined &&
            result.value(readPackageParam) != QJsonValue::Undefined &&
            result.value(writePackageParam) != QJsonValue::Undefined) {
        DapCmdStats::trafficStatsHandler(result.value(readBytesParam), result.value(writeBytesParam),
                                                result.value(readPackageParam), result.value(writePackageParam));
    } else {
        qWarning() << result;
    }
}

void DapCmdStats::handleError(int code, const QString& message)
{
    Q_UNUSED(code); Q_UNUSED(message);
    qWarning() << *m_errorObject;
}
