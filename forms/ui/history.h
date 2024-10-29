#ifndef HISTORY_H
#define HISTORY_H

/* INCLUDES */
#include <QWidget>
#include "baseform.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class History; }
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for licence list
 *
 * Serial key's history list
 *
 * @ingroup groupUiClasses
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class History : public BaseForm
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::History *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit History (QWidget *parent = nullptr);
  ~History();
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
  void refreshHistoryList();
  void slotRetranslated();
  /// @}
};

/*-----------------------------------------*/
#endif // HISTORY_H
