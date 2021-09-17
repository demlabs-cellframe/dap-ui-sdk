#ifndef BASEFORM_H
#define BASEFORM_H

/* INCLUDES */
#include <QWidget>
#include "style/kelguistylemanager.h"

/****************************************//**
 * @brief base for ui/classes
 * @ingroup groupUiClasses
 * @date 09.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class BaseForm : public QWidget
{
  Q_OBJECT
  KELGUI_ENABLECSS
  KELGUI_ENABLEWIDGETSTYLE

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit BaseForm (QWidget *parent = nullptr);
  ~BaseForm() override;
  /// @}
};

/*-----------------------------------------*/
#endif // BASEFORM_H
