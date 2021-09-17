#ifndef BUGREPORTS_H
#define BUGREPORTS_H

/* INCLUDES */
#include <QWidget>
#include <QMap>
#include <QMovie>
#include "baseform.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class BugReports; }
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for bug reports
 * @ingroup groupUiClasses
 * @date 27.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class BugReports : public BaseForm
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum Mode
  {
    List,
    Write,
    Loading,
  };
  Q_ENUM (Mode);
  /// @}

  static const QString SCREEN_NAME;

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  /// gui
  Ui::BugReports *ui;
  /// animation
  QMovie *movLoading;
  /// current mode
  Mode m_mode;
  /// showing/hiding map
  QMultiMap<Mode, QWidget *> m_map;
  /// testing radio map @see slotRadioTest
  QMap<QObject *, Mode> p_radioTestToMode;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit BugReports (QWidget *parent = nullptr);
  ~BugReports();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigReturn();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetMode (Mode mode);
private slots:
  void slotRadioTest();
  /// @}
};

/*-----------------------------------------*/
#endif // BUGREPORTS_H
