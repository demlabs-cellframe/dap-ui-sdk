#ifndef DAPGUIRADIO_H
#define DAPGUIRADIO_H

/* INCLUDES */
#include <QWidget>
#include "dapguiradiostylemanager.h"

/* DEFS */
namespace Ui { class DapGuiRadioButtonUI; };

/****************************************//**
 * @brief custom radio button
 *
 * Widget made using 3 DapGuiLabel 's
 * and DapGuiSeparator inside ui-form
 *
 * Widget will try to control other
 * instances of DapGuiRadio classes inside
 * parent's layout when clicked (checked/unchecked)
 *
 * @ingroup groupDapGuiWidgets
 * @date 23.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiRadio : public QWidget
{
  Q_OBJECT
  DAPGUI_ENABLECSS
  DAPGUI_ENABLEWIDGETSTYLE

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (QString text              READ text               WRITE setText)
  Q_PROPERTY (QString textCssClass      READ textCssClass       WRITE setTextCssClass)
  Q_PROPERTY (QString checkedCssClass   READ checkedCssClass    WRITE setCheckedCssClass)
  Q_PROPERTY (QString uncheckedCssClass READ uncheckedCssClass  WRITE setUncheckedCssClass)
  Q_PROPERTY (bool checked              READ checked            WRITE setChecked)
  Q_PROPERTY (bool separator            READ separator          WRITE setSeparator)
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::DapGuiRadioButtonUI *ui;
  bool m_checked;
  bool m_separator;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapGuiRadio (QWidget *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  QString text() const;
  void setText (const QString &a_text);

  QString textCssClass() const;
  void setTextCssClass (const QString &a_cssClass);

  QString checkedCssClass() const;
  void setCheckedCssClass (const QString &a_cssClass);

  QString uncheckedCssClass() const;
  void setUncheckedCssClass (const QString &a_cssClass);

  bool checked() const;
  void setChecked (bool a_newChecked);

  bool separator() const;
  void setSeparator (bool a_separator);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void clicked();
  void toggled (bool a_checked);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
private slots:
  void _slotClicked();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPGUIRADIO_H
