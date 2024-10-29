#ifndef DAPGUICHECKBOX_H
#define DAPGUICHECKBOX_H

/* INCLUDES */
#include <QCheckBox>
#include "dapguicheckboxstylemanager.h"

/****************************************//**
 * @brief overlap for checkbox style
 *
 * Using inerhit from DapGuiStyleManager
 * class DapGuiCheckBoxStyleManager to show
 * styled Checkbox
 *
 * @ingroup groupDapGuiWidgets
 * @date 23.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiCheckBox : public QCheckBox
{
  Q_OBJECT

  DapGuiCheckBoxStyleManager __kgsm = DapGuiCheckBoxStyleManager (this);

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapGuiCheckBox (QWidget *parent = 0);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPGUICHECKBOX_H
