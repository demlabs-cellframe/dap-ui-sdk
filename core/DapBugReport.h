#ifndef DAPBUGREPORT_H
#define DAPBUGREPORT_H

#include <QString>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>

class DapBugReport
{
public:
    DapBugReport();

    QByteArray getDataBugReportByteArr(){return dataBugReportByteArray;}
    QString getDataBugReportStr(){return dataBugReportString;}
    QString getPuthToFile(){return pathToFile;}

private:
    QByteArray dataBugReportByteArray;
    QString dataBugReportString = "file_missing";
    QString pathToFile = "";


    bool runScriptPackaging(QString path);
    QString retLastModifyFile(QString path);

};

#endif // DAPBUGREPORT_H
