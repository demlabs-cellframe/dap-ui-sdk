#ifndef DAPBUGREPORT_H
#define DAPBUGREPORT_H

#include <QString>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include "DapDataLocal.h"

class DapBugReport
{
public:
    DapBugReport();

    QString getDataLogService(){return logService;}
    QString getDataLogGui(){return logGui;}

private:
    QString logService = "";
    QString logGui = "";

    bool runScriptPackaging(QString path);
    QString retLastModifyFile(QString path);

};

#endif // DAPBUGREPORT_H
