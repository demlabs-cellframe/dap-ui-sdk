#ifndef KELGUILABEL_H
#define KELGUILABEL_H

/* INCLUDES */
#include <QLabel>

/****************************************//**
 * @brief widget with custom properties
 * @ingroup groupKelGuiWidgets
 * @date 30.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiLabel : public QLabel
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (QString cssStyle READ cssStyle WRITE setCssStyle)
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  QString m_cssStyle;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit KelGuiLabel(QWidget *parent = nullptr);
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:
  QString cssStyle() const;
  void setCssStyle(const QString &cssStyleText);
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUILABEL_H
