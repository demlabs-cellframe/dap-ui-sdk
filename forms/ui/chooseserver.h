#ifndef CHOOSESERVER_H
#define CHOOSESERVER_H

/* INCLUDES */
#include <QWidget>

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class ChooseServer; }
QT_END_NAMESPACE

/****************************************//**
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class ChooseServer : public QWidget
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
  explicit ChooseServer(QWidget *parent = nullptr);
  ~ChooseServer();
  /// @}
};

/*-----------------------------------------*/
#endif // CHOOSESERVER_H
