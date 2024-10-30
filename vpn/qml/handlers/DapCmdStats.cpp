#include "DapCmdStats.h"
#include <utility>

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

void DapCmdStats::trafficStatsHandler(const QJsonValue &readBytes, const QJsonValue &writeBytes,
        const QJsonValue&readPackage, const QJsonValue &writePackage)
{
    bool ok1, ok2, ok3, ok4;
    qint64 bRead    = readBytes     .toString().toLongLong(&ok1,10);
    qint64 bWrite   = writeBytes    .toString().toLongLong(&ok2,10);
    qint64 pRead    = readPackage   .toString().toLongLong(&ok3,10);
    qint64 pWrite   = writePackage  .toString().toLongLong(&ok4,10);
    
    if (!ok1 || !ok2 || !ok3 || !ok4) {
        qWarning() << "Something wrong can't parse traffic stats";
        return;
    }
    emit sigReadWriteBytesStat(bRead, bWrite, pRead, pWrite);
}

void DapCmdStats::handleResult(const QJsonObject& result)
{
    if (result.value(READ_BYTES_PARAM) != QJsonValue::Undefined &&
            result.value(WRITE_BYTES_PARAM) != QJsonValue::Undefined &&
            result.value(READ_PACKAGE_PARAM) != QJsonValue::Undefined &&
            result.value(WRITE_PACKAGE_PARAM) != QJsonValue::Undefined)
    {
        trafficStatsHandler(result.value(READ_BYTES_PARAM), result.value(WRITE_BYTES_PARAM),
                                                result.value(READ_PACKAGE_PARAM), result.value(WRITE_PACKAGE_PARAM));
    }
    else
    {
        qWarning() << "[DapCmdStats] Stats result" << result;
    }
}

void DapCmdStats::handleError(int code, const QString& message)
{
    Q_UNUSED(code); Q_UNUSED(message);
    qWarning() << *m_errorObject;
}
