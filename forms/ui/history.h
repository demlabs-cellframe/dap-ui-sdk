#ifndef HISTORY_H
#define HISTORY_H

/* INCLUDES */
#include <QWidget>

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class History; }
QT_END_NAMESPACE

/****************************************//**
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class History : public QWidget
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
  explicit History(QWidget *parent = nullptr);
  ~History();
  /// @}
};

/*-----------------------------------------*/
#endif // HISTORY_H
