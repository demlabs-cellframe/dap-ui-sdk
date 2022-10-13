#ifndef DAPCMDNODEHANDLER_H
#define DAPCMDNODEHANDLER_H

#include <QObject>
#include <QJsonObject>
#include "DapCmdServiceAbstract.h"

class DapCmdNode : public DapCmdServiceAbstract
{
    Q_OBJECT
private:
    static const QString actionParam;
public:
    explicit DapCmdNode(QObject *parent = nullptr);
    virtual ~DapCmdNode() override;

    void handle(const QJsonObject* params) override;
public slots:
    void sendNodeInfo(QStringList walletsList);
    void sendError(int code, QString errorMessage);
signals:
    void nodeInfoRequest();
    void nodeInfoRequestStop();
};

#endif // DAPCMDNODEHANDLER_H
