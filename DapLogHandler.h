#ifndef DAPCHAINLOGHANDLER_H
#define DAPCHAINLOGHANDLER_H

#include <QObject>
#include <QFileSystemWatcher>

class DapChainLogHandler : public QObject
{
    Q_OBJECT
public:

    explicit DapChainLogHandler(QString logPath, QObject *parent = nullptr);


signals:
    void onChangedLog();                        ///< The signal is emitted when system logs file was changed

public slots:
    QStringList request();                      ///< get

private:

    int moveCaretToEnd();
    QString m_logFilePath;
    QFileSystemWatcher  m_fileSystemWatcher;    ///<  Log file change watcher.
    qint64 m_currentCaretPosition{0};           ///<  Current caret position in log file

};

#endif // DAPCHAINLOGHANDLER_H
