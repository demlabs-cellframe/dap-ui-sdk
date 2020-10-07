#ifndef DAPBUGREPORT_H
#define DAPBUGREPORT_H

#include <QString>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QSysInfo>
#include "DapDataLocal.h"
#include "JlCompress.h"

class DapBugReport
{
public:
    DapBugReport();

    QString getSystemInfo();
    bool createZipDataBugReport(QString serial, QString message);
    QByteArray getDataZipFile(){return byteArrayZipFile;}

private:
    QByteArray byteArrayZipFile;

//    bool runScriptPackaging(QString path);
//    QString retLastModifyFile(QString path);


};

#endif // DAPBUGREPORT_H
