#ifndef DAPCMDDATALOCAL_H
#define DAPCMDDATALOCAL_H


#include "DapCmdClientAbstract.h"
#include <QObject>

/****************************************//**
 * @brief Dap Local Data command
 * @date 30.09.2024
 * @author Mikhail Shilenko
 *******************************************/

class DapCmdDataLocal : public DapCmdClientAbstract
{
    Q_OBJECT

public:
    explicit DapCmdDataLocal (QObject *a_parent = nullptr);
    virtual ~DapCmdDataLocal() override;

    void handleResult (const QJsonObject &a_params) override;
    void handleError (int code, const QString &message) override;

    const QString &clientTypeName() const;
    void setClientTypeName (const QString &a_clientType);
    void sendUserConfigOldFilename (const QString &a_filename);
    /// request value from serive
    void requestValue (const QString &a_name, const int a_msgId);
    /// request all data from service
    void requestAllData (const int a_msgId = 0);
    /// send value to service
    void sendValue (const QString &a_name, const QVariant &a_value, const int a_msgId);
    /// send remove value to service
    void sendRemove (const QString &a_name, const int a_msgId);

signals:
    /// service sent value
    void sigGotValue (QString a_name, QVariant a_value, const int a_msgId);
    /// service sent all data
    void sigGotAllData (QJsonObject a_data, const int a_msgId, QString a_clientType);
    /// service sent remove value
    void sigGotRemove (QString a_name, const int a_msgId);

protected:
    QString m_clientType;
};

#endif
