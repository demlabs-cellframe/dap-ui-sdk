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
  //KELGUI_ENABLECSS
  Q_PROPERTY (QString cssStyle READ cssStyle WRITE setCssStyle)
  KelGuiStyleManager *__kgsm = new KelGuiStyleManager (this);
  QString cssStyle() const { return __kgsm->cssStyle(); }
  void setCssStyle (const QString &cssStyle) { __kgsm->setCssStyle (cssStyle); }


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
