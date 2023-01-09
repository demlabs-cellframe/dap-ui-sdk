#ifndef DAPCMDCDBCTL_H
#define DAPCMDCDBCTL_H

/* INCLUDES */
#include <QObject>
#include "DapCmdClientAbstract.h"

/****************************************//**
 * @brief Get/Set cdb servers list
 * @date 26.10.2022
 * @author Mikhail Shilenko
 *******************************************/

class DapCmdCdbCtl : public DapCmdClientAbstract
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  static const QString s_fieldCdb;
  static const QString s_fieldValue;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapCmdCdbCtl (QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  void sendCmdSetList (const QString &a_value);
  void sendCmdGetList();
protected:
  void _updateCmds (const QJsonValue &a_value);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
protected:
  void handleResult (const QJsonObject &a_result) override;
  void handleError (int, const QString&) override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigCdbList();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPCMDCDBCTL_H
