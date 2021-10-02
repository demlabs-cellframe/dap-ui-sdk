#ifndef KELGUILINEEDIT_H
#define KELGUILINEEDIT_H

/* INCLUDES */
#include <QLineEdit>
#include "style/kelguistylemanager.h"

/****************************************//**
 * @brief overlap for line edit style
 * @ingroup groupKelGuiWidgets
 * @date 18.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiLineEdit : public QLineEdit
{
  Q_OBJECT
  KELGUI_ENABLECSS

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  KelGuiLineEdit (QWidget *parent = nullptr);
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUILINEEDIT_H
