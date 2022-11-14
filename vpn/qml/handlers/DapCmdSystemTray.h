/****************************************************************************
**
** This file is part of the DapVpn project.
**
** The class implements the command handler for receiving user data from
** the service.
**
****************************************************************************/

#ifndef DAPCMDSYSTEMTRAYHANDLER_H
#define DAPCMDSYSTEMTRAYHANDLER_H

/* INCLUDES */
#include "DapCmdServiceAbstract.h"

#include <QObject>
#include <QJsonValue>

/****************************************//**
 * @brief
 * @date
 * @author
 *******************************************/

class DapCmdSystemTray: public DapCmdServiceAbstract
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum class RequestType
  {
    setting,
    statistic,
    login,
    dashboard,
    client_started,
    client_exited,
    apllication_quit,
    tray_application_running,
    change_server,
  };
  Q_ENUM(RequestType)
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapCmdSystemTray (QObject *parent = nullptr);
  virtual ~DapCmdSystemTray() override;
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  /// Process command.
  /// @param params Command parameters.
  void handle (const QJsonObject *params) override;
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  /// @param address Server address.
  void sendShowSettingInterface();
  void sendShowStatisticInterface();
  void sendShowLoginInterface();
  void sendShowDashboardInterface();
  void sendChangeServer (const QString &a_serverName, const QString &a_serverAddress);
  void clientStarted();
  void trayApplicationStarted();
  void clientExited();
  void apllicationQuitRequest();
private:
  void sendShowInterface (const RequestType &a_type);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void showSettingInterface();
  void showStatisticInterface();
  void showLoginInterface();
  void showDashboardInterface();
  void changeServer (QString a_serverName, QString a_serverAddress);
  void trayApplicationFound();
  void clientFound();
  void clientClosed();
  void quitRequest();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPCMDSYSTEMTRAYHANDLER_H
