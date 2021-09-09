#ifndef BASEFORM_H
#define BASEFORM_H

/* INCLUDES */
#include <QWidget>

#define BASEFORM_RESTARTUI_ROUTINE(t) \
  delete layout(); \
  ui->setupUi (this)

//  while (QWidget *w = findChild<QWidget *>()) \
//    delete w; \

//  delete ui; ui = nullptr

//  delete ui; \
//  ui = new t; \
//  ui->setupUi (this)

/****************************************//**
 * @brief base for ui/classes
 * @ingroup groupUiClasses
 * @date 09.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class BaseForm : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit BaseForm (QWidget *parent = nullptr);
  ~BaseForm() override;
  /// @}

  /****************************************//**
   * @name VIRTUAL
   *******************************************/
  /// @{
public:
  virtual void restartUi() = 0;
  /// @}
};

/*-----------------------------------------*/
#endif // BASEFORM_H
