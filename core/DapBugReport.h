#ifndef DAPBUGREPORT_H
#define DAPBUGREPORT_H

#include <QString>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include "DapDataLocal.h"
#include "JlCompress.h"

class DapBugReport
{
public:
    DapBugReport();

    bool createZipDataBugReport(QString email, QString message);
    QByteArray getDataZipFile(){return byteArrayZipFile;}

private:
    QByteArray byteArrayZipFile;

//    bool runScriptPackaging(QString path);
//    QString retLastModifyFile(QString path);


};

#endif // DAPBUGREPORT_H
