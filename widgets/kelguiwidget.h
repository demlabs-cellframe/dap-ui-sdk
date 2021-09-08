#ifndef KELGUIWIDGET_H
#define KELGUIWIDGET_H

/* INCLUDES */
#include <QWidget>
#include "style/kelguistylemanager.h"

/****************************************//**
 * @brief widget with custom style
 * @ingroup groupKelGuiWidgets
 * @date 30.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiWidget : public QWidget
{
  Q_OBJECT
  KELGUI_ENABLECSS
  KELGUI_ENABLEWIDGETSTYLE

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit KelGuiWidget (QWidget *parent = nullptr);
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUIWIDGET_H
