#include "DapChainLogHandler.h"

#include <QtGlobal>
#include <QFile>
#include <QDebug>
#include <QRegularExpression>


/// Standard constructor
/// Add path to system logs file
DapChainLogHandler::DapChainLogHandler(QString logPath, QObject *parent)
    : QObject(parent)
    , m_logFilePath(logPath)
{
    qDebug()<< "DapChainLogHandler constructor: " << logPath;

    m_fileSystemWatcher.addPath(logPath);

    moveCaretToEnd();

    connect(&m_fileSystemWatcher, &QFileSystemWatcher::fileChanged, this, [this, logPath] (const QString& asFile) {
        Q_UNUSED(asFile)
        m_fileSystemWatcher.addPath(logPath);

        emit onChangedLog();
    });
}

/// Request new logs from system logs file
/// @return list of new logs
QStringList DapChainLogHandler::request()
{
    QFile file(m_logFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open file " << file.fileName();
        return QStringList();
    }
    else
    {
        QTextStream in(&file);
        in.seek(m_currentCaretPosition);
        const QRegularExpression re("(\\[\\d\\d\\/\\d\\d\\/\\d\\d\\-\\d\\d\\:\\d\\d\\:\\d\\d])\\s(\\[\\w+\\])\\s(\\[\\w+\\])(.+)");

        QStringList listLogs;
        while (!in.atEnd()) {
            const QString line = in.readLine();
            const auto match = re.match(line);
            if(!match.hasMatch())
                continue;

            const QString matchedLog = match.captured();
            listLogs.append(matchedLog);
            m_currentCaretPosition += matchedLog.length();
        }

        return listLogs;
    }
}

int DapChainLogHandler::moveCaretToEnd()
{
    QFile file(m_logFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open file " << file.fileName();
        return -1;
    }

    return m_currentCaretPosition = file.readAll().length();
}

