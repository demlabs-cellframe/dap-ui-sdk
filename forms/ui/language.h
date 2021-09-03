#ifndef LANGUAGE_H
#define LANGUAGE_H

/* INCLUDES */
#include <QWidget>

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class Language; }
QT_END_NAMESPACE

/****************************************//**
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class Language : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::Language *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit Language(QWidget *parent = nullptr);
  ~Language();
  /// @}
};

/*-----------------------------------------*/
#endif // LANGUAGE_H
