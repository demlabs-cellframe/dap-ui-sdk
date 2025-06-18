#include "DapBugReport.h"

#ifndef DAP_VERSION
#define DAP_VERSION ""
#endif

bool CheckFile(const QString path)
{
    QFileInfo check_file(path);
    return check_file.exists() && check_file.isFile();
}

DapBugReport::DapBugReport()
{

}

bool DapBugReport::createZipDataBugReport(const QString &serial, const QString &message, const QString &contactAddress, const QString &attachFile, const QString &pkeyHash)
{
    qDebug() << "DapBugReport::createZip";
    qDebug() << "DapBugReport attachFile" << attachFile << CheckFile(attachFile);

    QString dataJsonFilePath = QDir::tempPath() + QDir::separator() + "data.json";
    QFile fileJsonData(dataJsonFilePath);
    if(fileJsonData.open(QIODevice::WriteOnly | QIODevice::Text)){
        QJsonObject obj;
        obj["sn"] = serial;
        obj["version_app"] = DAP_BRAND " " DAP_VERSION;
        obj["os"] = getSystemInfo();
        obj["message"] = message;
        obj["contact_address"] = contactAddress;
        obj["pKeyHash"] = pkeyHash;
        obj["file_attached"] = (CheckFile(attachFile)) ? "YES" : "NO";
        QJsonDocument saveDoc(obj);
        fileJsonData.write(saveDoc.toJson());
        fileJsonData.close();
    }
    else
    {
        qWarning() << "Can not create the data.json file for bug-report";
    }

    QFileInfo infoFileData(fileJsonData);
    QStringList fileList;
    for (int i = 0; i < 4; i++){
        QString path;
        switch (i) {
            case 0:
                path = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/service.log"; break;
            case 1:
                path = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/gui.log"; break;
            case 2:
                path = infoFileData.path() + "/" + infoFileData.fileName(); break;
            case 3:
                path = attachFile; break;
        }
        if (CheckFile(path)) {
            fileList << path;
        }
    }

    QString bugReportPath = QDir::tempPath() + QDir::separator() + "temp_bugReportZip.zip";
    if (!DapZip::compressFiles(bugReportPath, fileList)){
        qWarning() << "Bug-report file not compress";
        return false;
    }

    QFile zipFile(bugReportPath);
    if (zipFile.open(QIODevice::ReadOnly)){
        byteArrayZipFile = zipFile.readAll();
        qDebug() << "Bug-report byte array size: " << byteArrayZipFile.size();
        zipFile.remove();
        fileJsonData.remove();
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
