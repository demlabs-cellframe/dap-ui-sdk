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
    show_window,
    setting,
    statistic,
    login,
    dashboard,
    client_started,
    client_exited,
    apllication_quit,
    tray_application_running,
    change_server,
    select_cdb_mode,
    activation_changed
  };
  Q_ENUM(RequestType)

  enum class CdbMode
  {
    cdb,
    no_cdb
  };
  Q_ENUM(CdbMode)
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
  void sendShowGui();
  void sendShowSettingInterface();
  void sendShowStatisticInterface();
  void sendShowLoginInterface();
  void sendShowDashboardInterface();
  void sendChangeServer (const QString &a_serverName, const QString &a_serverAddress);
  void sendSelectCdbMode (CdbMode a_value);
  void sendActiovationChange (bool a_activated, const QString &a_serialKey);
  void clientStarted();
  void trayApplicationStarted();
  void clientExited();
  void apllicationQuitRequest();
private:
  void sendGuiCmd (const RequestType &a_type);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void showGui();
  void showSettingInterface();
  void showStatisticInterface();
  void showLoginInterface();
  void showDashboardInterface();
  void changeServer (QString a_serverName, QString a_serverAddress);
  void guiSelectCdbMode (CdbMode a_value);
  void activationChanged (bool a_activated, QString a_serialKey);
  void trayApplicationFound();
  void clientFound();
  void clientClosed();
  void quitRequest();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPCMDSYSTEMTRAYHANDLER_H
