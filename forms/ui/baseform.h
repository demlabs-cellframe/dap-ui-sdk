#ifndef BASEFORM_H
#define BASEFORM_H

/* INCLUDES */
#include <QWidget>
#include "style/dapguistylemanager.h"

/****************************************//**
 * @brief base for ui/classes
 *
 * Features:
 * - Css Style
 * - Css Class "window" as default
 *
 * @ingroup groupUiClasses
 * @date 09.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class BaseForm : public QWidget
{
  Q_OBJECT
  DAPGUI_ENABLECSS
  DAPGUI_ENABLEWIDGETSTYLE

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
