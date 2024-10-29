#ifndef DAPCMDPARSER_H
#define DAPCMDPARSER_H

#include <QObject>
#include <QTcpSocket>

class DapCmdParser : public QObject
{
    Q_OBJECT
public:
    explicit DapCmdParser(QTcpSocket* sockCtl,
                          QObject *parent = nullptr);
private:
    const static uint16_t MAX_BUFFER_SIZE = 10000;
    QTcpSocket * m_sockCtl;
    QByteArray readBuffer;
signals:
    void cmdReady(QByteArray ba);
public slots:
    void onCtlReadReady();
};

#endif // DAPCMDPARSER_H
