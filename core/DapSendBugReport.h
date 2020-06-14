#ifndef DAPSENDBUGREPORT_H
#define DAPSENDBUGREPORT_H

#include <QString>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "DapSession.h"
#include "DapDataLocal.h"

class DapSendBugReport
{
public:
    DapSendBugReport();

    bool createBugReport();
    bool sendBugReport();
    bool runScriptPackaging(QString path);
    QString retLastModifyFile(QString path);
    DapSession *m_session;
    QNetworkReply * m_network_reply = Q_NULLPTR;

};

#endif // DAPSENDBUGREPORT_H
