#ifndef DAPCMDCLIENTABSTRACT_H
#define DAPCMDCLIENTABSTRACT_H

#include "DapCmdAbstract.h"

class DapCmdClientAbstract : public DapCmdAbstract
{
protected:
    // Child class can use full error object
    const QJsonObject *m_errorObject;

    virtual void handle(const QJsonObject* params) override;
public:
    DapCmdClientAbstract(DapJsonCmdType cmd, QObject* parent):
        DapCmdAbstract(Side::CLIENT, cmd, parent) {}

    void sendCmd(const QJsonObject * obj = Q_NULLPTR) override {

        if(obj == Q_NULLPTR) {
            qDebug() << "Send command" << commandToString(cmd());
        } else {
            qDebug() << "Send command" << commandToString(cmd())
                     << "params:"      << *obj;
        }
        DapCmdAbstract::sendCmd(obj);
    }

    virtual ~DapCmdClientAbstract() override;
    virtual void handleResult(const QJsonObject& result) = 0;
    virtual void handleError(int code, const QString& message) = 0;
};

#endif // DAPCMDCLIENTABSTRACT_H
