#ifndef DAPSENDBUGREPORT_H
#define DAPSENDBUGREPORT_H

#include <QString>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "DapDataLocal.h"

class DapSendBugReport
{
public:
    DapSendBugReport();

    QString file_name;
    QString file_to_path;

    bool createBugReport();
    bool sendBugReport();
    bool runScriptPackaging(QString path);
    QString retLastModifyFile(QString path);

};

#endif // DAPSENDBUGREPORT_H
