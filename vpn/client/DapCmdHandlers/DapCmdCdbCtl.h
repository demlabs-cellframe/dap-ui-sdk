#pragma once
#include <QObject>
#include "DapCmdServiceAbstract.h"

class DapCmdCdbCtl : public DapCmdServiceAbstract
{
  Q_OBJECT

protected:
  static const QString s_fieldCdb;
  static const QString s_fieldValue;

public:
  explicit DapCmdCdbCtl(QObject *parent = nullptr);

public:
  void sendCmdGetList (const QString &a_value);

public:
  void handle (const QJsonObject* a_params) override;

signals:
  void sigCdbList (const QString a_value);
};

