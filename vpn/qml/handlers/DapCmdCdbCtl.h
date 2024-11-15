#pragma once

#include <QObject>
#include "DapCmdClientAbstract.h"

class DapCmdCdbCtl : public DapCmdClientAbstract
{
  Q_OBJECT

protected:
  static const QString s_fieldCdb;
  static const QString s_fieldValue;

public:
  explicit DapCmdCdbCtl (QObject *parent = nullptr);

public:
  void sendCmdSetList (const QString &a_value);
  void sendCmdGetList();
protected:
  void _updateCmds (const QJsonValue &a_value);

protected:
  void handleResult (const QJsonObject &a_result) override;
  void handleError (int, const QString&) override;

signals:
  void sigCdbList();
};

