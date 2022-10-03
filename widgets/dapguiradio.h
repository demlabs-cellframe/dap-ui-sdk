#ifndef DAPGUIRADIO_H
#define DAPGUIRADIO_H

/* INCLUDES */
#include <QWidget>
#include "dapguiradiostylemanager.h"
#include "DapServersData.h"

#include "QToolTip"
#include "QMouseEvent"

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

class DapGuiRadioBase : public QWidget
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
  DapServerInfo::connectionQuality m_connetion_quality = DapServerInfo::connectionQuality::NO_CONNECTION;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapGuiRadioBase (QWidget *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  QString text() const;
  void setText (const QString &a_text);

  DapServerInfo::connectionQuality qulityIcon() const;
  void setQulityIcon(const DapServerInfo::connectionQuality connetion_quality);

  void setPingToolTip(const qint16 a_ping);

  QString textCssClass() const;
  void setTextCssClass (const QString &a_cssClass);

  QString checkedCssClass() const;
  void setCheckedCssClass (const QString &a_cssClass);

  QString uncheckedCssClass() const;
  void setUncheckedCssClass (const QString &a_cssClass);

  QString qualityCssClass() const;
  void setQualityCssClass(const QString &a_cssClass);

  bool checked() const;
  void setIndicatorChecked(bool a_newChecked);
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
public slots:
  void _slotClicked();
  /// @}
};



class DapGuiRadio : public DapGuiRadioBase
{
  Q_OBJECT
public:
  explicit DapGuiRadio (QWidget *parent = nullptr)
        : DapGuiRadioBase(parent)
    {
        connect (this, &DapGuiRadio::clicked,
                 this, &DapGuiRadioBase::_slotClicked);
    }
};

/*-----------------------------------------*/
#endif // DAPGUIRADIO_H
