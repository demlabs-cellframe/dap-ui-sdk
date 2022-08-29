#ifndef DAPGUIPUSHBUTTON_H
#define DAPGUIPUSHBUTTON_H

/* INCLUDES */
#include <QPushButton>
#include <QLabel>
#include <QUrl>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <memory>
#include "dapguipushbuttonstylemanager.h"

/****************************************//**
 * @brief overlap for push button style
 * @note this button can be used as checkbox
 *
 * Features:
 * - Built-in styles (DapGuiPushButton::Style)
 * - Css Style
 * - Customizable button PNG's
 * - Choosable opacity (1 or 0.2)
 * - Several signals
 *
 * @ingroup groupDapGuiWidgets
 * @date 19.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiPushButton : public QPushButton
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  /// Button built-in styles
  enum Style
  {
    Basic,
    Main,
    Connection,
    Account,
    Settings,
    Check,
    //Radio,
    Switch,
    Close,
    Custom,
    FilterClear
  };
  Q_ENUM (Style)
  /// @}

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (Style style       READ style        WRITE setStyle NOTIFY styleChanged)
  Q_PROPERTY (QUrl custom       READ custom       WRITE setCustom NOTIFY customChanged DESIGNABLE customEnabled)
  Q_PROPERTY (QUrl customHover  READ customHover  WRITE setCustomHover NOTIFY customHoverChanged DESIGNABLE customEnabled)
  Q_PROPERTY (QUrl customPushed READ customPushed WRITE setCustomPushed NOTIFY customPushedChanged DESIGNABLE customEnabled)
  Q_PROPERTY (QString cssStyle  READ customCss    WRITE setCustomCss)
  Q_PROPERTY (bool opacity      READ opacity      WRITE setOpacity)
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Style m_style;
  QUrl m_custom, m_customHover, m_customPushed;
  QString m_customCss;
  QGraphicsOpacityEffect *m_opacityEffect;
  DapGuiPushButtonStyleManager __kgsm = DapGuiPushButtonStyleManager (this);
  bool _styleHook;
  struct
  {
      QString filename;
      QPixmap pixmap;
      QSize size;
  } _cache;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapGuiPushButton (QWidget *parent = 0);
  ~DapGuiPushButton() override;
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:

  Style style() const;
  void setStyle (const Style &style);

  bool customEnabled() const;
  void setEnabledCustom(bool value);

  bool opacity() const;
  void setOpacity(bool value);

  QUrl custom() const;
  void setCustom (const QUrl &custom);

  QUrl customHover() const;
  void setCustomHover (const QUrl &customHover);

  QUrl customPushed() const;
  void setCustomPushed (const QUrl &customPushed);

  QString customCss() const;
  void setCustomCss (const QString &customCss);


  /// @}

  /****************************************//**
   * @name PRIVATE METHODS
   *******************************************/
  /// @{
private:
  void updateStyle();
  /// @}

  void paintEvent(QPaintEvent *) override;

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void styleChanged();        ///< button changed style
  void textChanged();         ///< button changed text
  void customChanged();       ///< button changed custom
  void customHoverChanged();  ///< button changed custom:hover
  void customPushedChanged(); ///< button changed custom:pressed

  void clicked();             ///< button is clicked
  /// @}
};

/*-----------------------------------------*/
#endif // DAPGUIPUSHBUTTON_H
