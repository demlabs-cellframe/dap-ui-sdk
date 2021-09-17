#ifndef CHOOSESERVER_H
#define CHOOSESERVER_H

/* INCLUDES */
#include <QWidget>
#include "baseform.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class ChooseServer; }
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for server list
 * @ingroup groupUiClasses
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class ChooseServer : public BaseForm
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::ChooseServer *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit ChooseServer (QWidget *parent = nullptr);
  ~ChooseServer();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigReturn();
  /// @}
};

/*-----------------------------------------*/
#endif // CHOOSESERVER_H
