#ifndef KELGUITEXTEDIT_H
#define KELGUITEXTEDIT_H

/* INCLUDES */
#include <QTextEdit>
#include "style/kelguistylemanager.h"

/****************************************//**
 * @brief overlap for text edit style
 * @ingroup groupKelGuiWidgets
 * @date 15.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiTextEdit : public QTextEdit
{
  Q_OBJECT
  KELGUI_ENABLECSS

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  KelGuiTextEdit (QWidget *parent = 0);
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUITEXTEDIT_H
