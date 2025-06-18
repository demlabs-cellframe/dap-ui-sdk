#ifndef DAPBUGREPORT_H
#define DAPBUGREPORT_H

#include <QString>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QDir>
#include <QSysInfo>
#include <QStandardPaths>
#include "DapDataLocal.h"
#include "packzip.h"

class DapBugReport
{
public:
    DapBugReport();

    QString getSystemInfo();
    bool createZipDataBugReport(const QString &serial, const QString &message, const QString &contactAddress, const QString &attachFile, const QString &pkeyHash);
    QByteArray getDataZipFile(){return byteArrayZipFile;}

private:
    QByteArray byteArrayZipFile;

//    bool runScriptPackaging(QString path);
//    QString retLastModifyFile(QString path);


};

#endif // DAPBUGREPORT_H
