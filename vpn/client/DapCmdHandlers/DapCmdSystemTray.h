#ifndef DAPCMDSYSTEMTRAYHANDLER_H
#define DAPCMDSYSTEMTRAYHANDLER_H

/* INCLUDES */
#include "DapCmdServiceAbstract.h"

#include <QObject>
#include <QJsonObject>

/****************************************//**
 * @brief
 * @date
 * @author
 *******************************************/

class DapCmdSystemTray : public DapCmdServiceAbstract
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapCmdSystemTray (QObject *parent = nullptr);
  virtual ~DapCmdSystemTray() override;
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  void handle (const QJsonObject* params) override;
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
//  void sendInterfaceRequest (QString interfaceName);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
//  void interfaceRequest (QString interfaceName);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPCMDSYSTEMTRAYHANDLER_H
