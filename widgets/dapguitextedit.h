#ifndef DAPGUITEXTEDIT_H
#define DAPGUITEXTEDIT_H

/* INCLUDES */
#include <QTextEdit>
#include "style/dapguistylemanager.h"

/****************************************//**
 * @brief overlap for text edit style
 *
 * Features:
 * - Css Style
 * - QTextEdit
 *
 * @ingroup groupDapGuiWidgets
 * @date 15.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiTextEdit : public QTextEdit
{
  Q_OBJECT
  DAPGUI_ENABLECSS

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapGuiTextEdit (QWidget *parent = 0);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPGUITEXTEDIT_H
