#ifndef DAPCMDDATALOCALHANDLER_H
#define DAPCMDDATALOCALHANDLER_H

#include <QObject>
#include <QJsonObject>
#include <QSettings>
#include "DapDataLocal.h"
#include "DapCmdServiceAbstract.h"

class DapCmdDataLocal : public DapCmdServiceAbstract
{
    Q_OBJECT
private:
    static const QString actionParam;
    DapDataSettingsLocal* dapDataLocalSettings;
public:
    explicit DapCmdDataLocal(QObject *parent = nullptr);
    virtual ~DapCmdDataLocal() override;

    void handle(const QJsonObject* params) override;
public slots:
    void sendSettings(QJsonObject& metadata);
signals:
    void sendSettingSignal();
    void saveSatus(bool status);
};

#endif // DAPCMDDATALOCALHANDLER_H
