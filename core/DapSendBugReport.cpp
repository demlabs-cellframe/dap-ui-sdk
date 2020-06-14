#include "DapSendBugReport.h"

DapSendBugReport::DapSendBugReport()
{

}

bool DapSendBugReport::sendBugReport(){

    QString pathToFile = QString("/opt/%1/bug_reports/").arg(DapDataLocal::me()->getBrandName());

#ifdef Q_OS_LINUX
    if (runScriptPackaging(QString(":/linux/report.sh")));
        pathToFile += retLastModifyFile(pathToFile);

#elif defined (Q_OS_WIN)
        //run script for windows
#endif

    m_network_reply = m_session->sendBugReport(pathToFile);
    QString reply = m_network_reply->readAll();

    return false;
}


bool DapSendBugReport::runScriptPackaging(QString path){
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


QString DapSendBugReport::retLastModifyFile(QString path){

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
