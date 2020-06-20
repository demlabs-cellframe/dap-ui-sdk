#include "DapBugReport.h"

DapBugReport::DapBugReport()
{
    /*
#ifdef Q_OS_LINUX
    if (runScriptPackaging(QString(":/linux/report.sh")));
        pathToFile = "/opt/%1/bug_reports/" + retLastModifyFile(QString("/opt/%1/bug_reports/").arg(DapDataLocal::me()->getBrandName()));

#elif defined (Q_OS_WIN)
    pathToFile = DapDataLocal::me()->getLogFilePath();//= "C:/Users/Public/Documents/" DAP_BRAND "/log/" DAP_BRAND "Service.log";
        //run script for windows
#endif
*/
    logService = DapDataLocal::me()->getLogFilePath();
    logGui = QString(DapDataLocal::me()->getLogFilePath()).replace("Service", "GUI");

    QFile bugReportFileService(logService);
    if(bugReportFileService.open(QIODevice::ReadOnly)){
        QTextStream in(&bugReportFileService);
        logService = in.readAll();
    } else {logService = "file_missing";}

    QFile bugReportFileGui(logGui);
    if(bugReportFileGui.open(QIODevice::ReadOnly)){
        QTextStream in(&bugReportFileGui);
        logGui = in.readAll();
    } else {logGui = "file_missing";}
}

bool DapBugReport::runScriptPackaging(QString path){
    QProcess *proc = new QProcess;
    QStringList arg;
    arg << "-c" ;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return false;

    arg << file.readAll();
    proc->start("/bin/bash", arg);
    return true;
}


QString DapBugReport::retLastModifyFile(QString path){

    QString fileName;
    QDir dir(path);

    if (!dir.exists()) {
        qWarning("The directory does not exist");
        return "";
    }

    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Time);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); i++){
        if (list[i].fileName().contains("bug-report_", Qt::CaseInsensitive)){
            return list[i].fileName();
        }
    }
    return "";
}
