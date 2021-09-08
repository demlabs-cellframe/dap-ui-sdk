#ifndef KELGUILABEL_H
#define KELGUILABEL_H

/* INCLUDES */
#include <QLabel>
#include "style/kelguistylemanager.h"

/****************************************//**
 * @brief widget with custom properties
 * @ingroup groupKelGuiWidgets
 * @date 30.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiLabel : public QLabel
{
  Q_PROPERTY (QString cssStyle READ cssStyle WRITE setCssStyle)
  KelGuiStyleManager __kgsm = KelGuiStyleManager (this);
public:
  QString cssStyle() const { return __kgsm.cssStyle(); }
  void setCssStyle (const QString &cssStyle) { __kgsm.setCssStyle (cssStyle); }
private:

  //KELGUI_ENABLECSSSTYLE

//  /****************************************//**
//   * @name PROPERTIES
//   *******************************************/
//  /// @{
//  Q_PROPERTY (QString cssStyle READ cssStyle WRITE setCssStyle)
//  /// @}

//  /****************************************//**
//   * @name VARS
//   *******************************************/
//  /// @{
//private:
//  QString m_cssStyle;
//  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit KelGuiLabel (QWidget *parent = nullptr);
  /// @}

//  /****************************************//**
//   * @name PUBLIC METHODS
//   *******************************************/
//  /// @{
//public:
//  QString cssStyle() const;
//  void setCssStyle(const QString &cssStyleText);
//  /// @}
};

/*-----------------------------------------*/
#endif // KELGUILABEL_H
