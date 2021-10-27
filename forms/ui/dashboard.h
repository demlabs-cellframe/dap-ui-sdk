#ifndef DASHBOARD_H
#define DASHBOARD_H

/* INCLUDES */
#include <QWidget>
#include <QDateTime>
#include "baseform.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class Connection;}
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for connection status
 *
 * Dashboard screen show's information
 * about current connection
 *
 * Features:
 * - %Connection time
 * - Switch
 * - Download/Upload speed
 * - Choose other server
 *
 * @note status icon fires up if datetime
 * provided slotSetStartedTime is not null
 * @ingroup groupUiClasses
 * @date 26.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class Dashboard : public BaseForm
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::Connection *ui;
  QDateTime m_started;
  QTimer *m_updateTime;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit Dashboard (QWidget *parent = nullptr);
  ~Dashboard();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigSwitchToggle (bool checked);
  void sigServerClicked();
  void sigDisconnectionRequested();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  /// set both speeds
  void slotSetDownUp (quint64 down, quint64 up);
  /// set download speed
  void slotSetDown (quint64 down);
  /// set upload speed
  void slotSetUp (quint64 up);
  /// set server ip
  void slotSetSererIP (QString ip);
  /// setup connection time (or invalid for disconnected)
  void slotSetStartedTime (QDateTime dt);
  /// set status text
  void setStatusText (QString a_text);
  /// set connectet time text
  void setConnectedTime (QString a_text);
  /// activate/deactivate status indicator
  void setStatusIdicator(bool a_enabled = false);
  /// turn on/off switch
  void setBtnSwitchChecked(bool a_authorized = true);
  /// set download speed text
  void setDownloadSpeed(QString a_text);
  /// set upload speed text
  void setUploadSpeed(QString a_text);
  /// set server information into button
  void setServerInfo(QString a_name, QString a_ip);
  /// start disconnection
  void slotDisconnectionRequested();

private slots:
  void _slotTimeUpdate();
  /// @}
};

/*-----------------------------------------*/
#endif // DASHBOARD_H
