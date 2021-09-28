#ifndef CONNECTION_H
#define CONNECTION_H

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
 * @note status icon fires up if datetime
 * provided slotSetStartedTime is not null
 * @ingroup groupUiClasses
 * @date 26.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class Connection : public BaseForm
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
  explicit Connection (QWidget *parent = nullptr);
  ~Connection();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigSwitchToggle (bool checked);
  void sigServerClicked();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSwitchSetState (bool checked);
  void slotSetDownUp (quint64 down, quint64 up);
  void slotSetDown (quint64 down);
  void slotSetUp (quint64 up);
  void slotSetSererIP (QString ip);
  void slotSetStartedTime (QDateTime dt);
  void slotSetStatusState (bool connected);
private slots:
  void _slotTimeUpdate();
  /// @}
};

/*-----------------------------------------*/
#endif // CONNECTION_H
