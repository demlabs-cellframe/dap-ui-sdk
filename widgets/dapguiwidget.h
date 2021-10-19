#ifndef DAPGUIWIDGET_H
#define DAPGUIWIDGET_H

/* INCLUDES */
#include <QWidget>
#include "style/dapguistylemanager.h"

/****************************************//**
 * @brief widget with custom style
 * @ingroup groupDapGuiWidgets
 * @date 30.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiWidget : public QWidget
{
  Q_OBJECT
  DAPGUI_ENABLECSS
  DAPGUI_ENABLEWIDGETSTYLE

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapGuiWidget (QWidget *parent = nullptr);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPGUIWIDGET_H
