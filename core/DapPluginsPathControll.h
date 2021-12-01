#ifndef DAPPLUGINSPATHCONTROLL_H
#define DAPPLUGINSPATHCONTROLL_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QDir>
#include <QTimer>
#include <QFileSystemWatcher>

#include "dap_common.h"
#include "dap_file_utils.h"

class DapPluginsPathControll : public QObject
{
    Q_OBJECT
public:
    explicit DapPluginsPathControll(QObject *parent = nullptr);

public:
    static QString defaultPluginPath(const QString a_brand);
    void setPathToPlugin(QString path){ m_pathToPlugin = path; }
    QString getPathToPlugin(){ return m_pathToPlugin; }

private:
    QString m_pathToPlugin;


};

#endif // DAPPLUGINSPATHCONTROLL_H
