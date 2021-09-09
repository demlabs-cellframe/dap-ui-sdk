#ifndef CONNECTION_H
#define CONNECTION_H

/* INCLUDES */
#include <QWidget>
#include "baseform.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class Connection;}
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for connection status
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
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit Connection (QWidget *parent = nullptr);
  ~Connection() override;
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  void restartUi() override;
  /// @}
};

/*-----------------------------------------*/
#endif // CONNECTION_H
