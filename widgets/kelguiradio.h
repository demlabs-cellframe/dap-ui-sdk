#ifndef KELGUIRADIO_H
#define KELGUIRADIO_H

/* INCLUDES */
#include <QRadioButton>

/****************************************//**
 * @brief overlap for radio button style
 * @ingroup groupKelGuiWidgets
 * @date 23.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiRadio : public QRadioButton
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  KelGuiRadio(QWidget *parent = 0);
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUIRADIO_H
