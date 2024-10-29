#ifndef DAPCMDHANDLERINTERFACE_H
#define DAPCMDHANDLERINTERFACE_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QObject>
#include "DapJsonParams.h"

class DapCmdAbstract : public QObject {
    Q_OBJECT
protected:
    enum class Side {CLIENT, SERVICE}; // which side belong object
    static QMap<DapJsonCmdType, QString> cmdString;
private:
    Side m_side;
    DapJsonCmdType m_cmd;
    
public:
    explicit DapCmdAbstract(Side side, DapJsonCmdType cmd, QObject *parent = nullptr):
        QObject(parent), m_side(side), m_cmd(cmd) {}

    static DapJsonCmdType stringToCommand(const QString& cmd);
    static QString commandToString(DapJsonCmdType command);
    static DapJsonCmdType getCommand(const QJsonDocument& doc);

    virtual  DapJsonCmdType cmd() const final { return m_cmd; }

    virtual void handle(const QJsonObject* params) = 0;

    virtual ~DapCmdAbstract() {}

protected:
    // send command or result. Emits send signal
    virtual void sendCmd(const QJsonObject * obj = Q_NULLPTR);
signals:
    void send(const QByteArray& ba);
};

#endif // DAPCMDHANDLERINTERFACE_H
