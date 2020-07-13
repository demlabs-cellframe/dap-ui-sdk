#ifndef DAPLOGHANDLER_H
#define DAPLOGHANDLER_H

#include <QObject>
#include <QFileSystemWatcher>

class DapLogHandler : public QObject
{
    Q_OBJECT
public:

    explicit DapLogHandler(const QString& logPath, QObject *parent = nullptr);


signals:
    void logChanged();                        ///< The signal is emitted when system logs file was changed

public slots:
    QStringList request();                      ///< get

private:

    int moveCaretToEnd();
    QString m_logFilePath;
    QFileSystemWatcher  m_fileSystemWatcher;    ///<  Log file change watcher.
    qint64 m_currentCaretPosition{0};           ///<  Current caret position in log file

};

#endif // DAPCHAINLOGHANDLER_H
