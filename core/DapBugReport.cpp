#include "DapBugReport.h"

#ifndef DAP_VERSION
#define DAP_VERSION ""
#endif

DapBugReport::DapBugReport()
{

}

bool DapBugReport::createZipDataBugReport(QString serial, QString message)
{
    qDebug() << "DapBugReport::createZip";

    QFile file("data.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream writeStream(&file);
        writeStream << serial << endl << " App: " DAP_BRAND" " << DAP_VERSION << endl << getSystemInfo() << endl << message.toUtf8();
        file.close();
    }

    QFileInfo infoFileData(file);
    QStringList fileList;
    for (int i = 0; i < 3; i++){
        QString path;
        switch (i) {
            case 0:
                path = DapDataLocal::instance()->getLogFilePath(); break;
            case 1:
                path = QString(DapDataLocal::instance()->getLogFilePath()).replace("Service", "GUI"); break;
            case 2:
                path = infoFileData.path() + "/" + infoFileData.fileName(); break;
        }
        QFileInfo check_file(path);
        if (check_file.exists() && check_file.isFile()) {
            fileList << path;
        }
    }

    if (!JlCompress::compressFiles("temp_bugReportZip.zip", fileList)){
        qDebug() << "Bug-report file not compress";
        return false;
    }

    QFile zipFile("temp_bugReportZip.zip");
    if (zipFile.open(QIODevice::ReadOnly)){
        byteArrayZipFile = zipFile.readAll();
        qDebug() << "Bug-report byte array size: " << byteArrayZipFile.size();
        zipFile.remove();
    }
    return true;
}

QString DapBugReport::getSystemInfo()
{
    return " Kernel: " + QSysInfo::kernelType()
           + " " + QSysInfo::kernelVersion()
           + " Product: " +   QSysInfo::productType()
           + " Version: " +   QSysInfo::productVersion()
           + " (" + QSysInfo::prettyProductName() + ") "
           + " Cpu architecture build: " +   QSysInfo::buildCpuArchitecture()
           + " Current: " +  QSysInfo::currentCpuArchitecture()
           + " Machine host name: " +  QSysInfo::machineHostName();
}

//bool DapBugReport::runScriptPackaging(QString path){
//    QProcess *proc = new QProcess;
//    QStringList arg;
//    arg << "-c" ;

//    QFile file(path);
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//            return false;

//    arg << file.readAll();
//    proc->start("/bin/bash", arg);
//    return true;
//}


//QString DapBugReport::retLastModifyFile(QString path){

//    QString fileName;
//    QDir dir(path);

//    if (!dir.exists()) {
//        qWarning("The directory does not exist");
//        return "";
//    }

//    dir.setFilter(QDir::Files);
//    dir.setSorting(QDir::Time);

//    QFileInfoList list = dir.entryInfoList();
//    for (int i = 0; i < list.size(); i++){
//        if (list[i].fileName().contains("bug-report_", Qt::CaseInsensitive)){
//            return list[i].fileName();
//        }
//    }
//    return "";
//}
