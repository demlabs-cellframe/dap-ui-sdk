#ifndef DAPCMDSTATSHANDLER_H
#define DAPCMDSTATSHANDLER_H

#include <QObject>
#include "DapCmdClientAbstract.h"

class DapCmdStats: public DapCmdClientAbstract
{
    Q_OBJECT
protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;
public:    
    explicit DapCmdStats(QObject *parent = nullptr);
    // true - on, false - off
    void sendCmdStatsTrafficOn(bool);
private:
    static const QString readBytesParam;
    static const QString writeBytesParam;
    static const QString readPackageParam;
    static const QString writePackageParam;
    void trafficStatsHandler(const QJsonValue &readBytes, const QJsonValue &writeBytes,
                             const QJsonValue &readPackage, const QJsonValue &writePackage);
    void readWriteBytesHandler();
signals:
    void sigReadWriteBytesStat(qint64 readBytes, qint64 writeBytes, qint64 readPackage, qint64 writePackage);
public slots:
};

#endif // DAPCMDSTATSHANDLER_H
