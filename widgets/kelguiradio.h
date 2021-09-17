#ifndef KELGUIRADIO_H
#define KELGUIRADIO_H

/* INCLUDES */
#include <QRadioButton>
#include "kelguiradiostylemanager.h"

/****************************************//**
 * @brief overlap for radio button style
 * @ingroup groupKelGuiWidgets
 * @date 23.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiRadio : public QRadioButton
{
  Q_OBJECT

  KelGuiRadioStyleManager __kgsm = KelGuiRadioStyleManager (this);

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  KelGuiRadio (QWidget *parent = 0);
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUIRADIO_H
