#ifndef KELGUIRADIO_H
#define KELGUIRADIO_H

/* INCLUDES */
#include <QWidget>
#include "kelguiradiostylemanager.h"

/* DEFS */
namespace Ui { class KelGuiRadioButtonUI; };

/****************************************//**
 * @brief overlap for radio button style
 * @ingroup groupKelGuiWidgets
 * @date 23.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiRadio : public QWidget
{
  Q_OBJECT
  KELGUI_ENABLECSS
  KELGUI_ENABLEWIDGETSTYLE

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
  Ui::KelGuiRadioButtonUI *ui;
  bool m_checked;
  bool m_separator;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit KelGuiRadio (QWidget *parent = nullptr);
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
#endif // KELGUIRADIO_H
