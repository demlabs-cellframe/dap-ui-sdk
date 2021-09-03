#ifndef CONNECTION_H
#define CONNECTION_H

/* INCLUDES */
#include <QWidget>

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class Connection;}
QT_END_NAMESPACE

/****************************************//**
 * @date 26.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class Connection : public QWidget
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
  explicit Connection(QWidget *parent = nullptr);
  ~Connection();
  /// @}
};

/*-----------------------------------------*/
#endif // CONNECTION_H
