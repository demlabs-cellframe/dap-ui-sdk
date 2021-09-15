#ifndef KELGUICHECKBOX_H
#define KELGUICHECKBOX_H

/* INCLUDES */
#include <QCheckBox>
#include "kelguicheckboxstylemanager.h"

/****************************************//**
 * @brief overlap for checkbox style
 * @ingroup groupKelGuiWidgets
 * @date 23.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiCheckBox : public QCheckBox
{
  Q_OBJECT

  KelGuiCheckBoxStyleManager __kgsm = KelGuiCheckBoxStyleManager (this);

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  KelGuiCheckBox (QWidget *parent = 0);
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUICHECKBOX_H
