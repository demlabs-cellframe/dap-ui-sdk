#ifndef DAPCMDSTATSHANDLER_H
#define DAPCMDSTATSHANDLER_H

#include <QObject>
#include "DapCmdClientAbstract.h"

class DapCmdStats: public DapCmdClientAbstract
{
    Q_OBJECT
public:    
    explicit DapCmdStats(QObject *parent = nullptr);
    // true - on, false - off
    void sendCmdStatsTrafficOn(bool);
protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;
signals:
    void sigReadWriteBytesStat(qint64 readBytes, qint64 writeBytes, qint64 readPackage, qint64 writePackage);
    void sigCurrentServerName(const QString& serverName);
private:
    void trafficStatsHandler(const QJsonValue &readBytes, const QJsonValue &writeBytes,
                             const QJsonValue &readPackage, const QJsonValue &writePackage);

    void readWriteBytesHandler();
private:
    const QString READ_BYTES_PARAM = "read_bytes";
    const QString WRITE_BYTES_PARAM = "write_bytes";
    const QString READ_PACKAGE_PARAM = "read_package";
    const QString WRITE_PACKAGE_PARAM = "write_package";
};

#endif // DAPCMDSTATSHANDLER_H
