#ifndef KELGUIPUSHBUTTON_H
#define KELGUIPUSHBUTTON_H

/* INCLUDES */
#include <QPushButton>
#include <QLabel>
#include <QUrl>
#include <QGraphicsDropShadowEffect>
#include <memory>

/* DEFS */
//#define ENABLEPURPLE

/****************************************//**
 * @brief overlap for button style
 * @note this button can be used as checkbox
 * @ingroup groupKelGuiWidgets
 * @date 19.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiPushButton : public QPushButton
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum Style
  {
    Basic,
#ifdef ENABLEPURPLE
    Purple,
#endif // ENABLEPURPLE
    Main,
    Connection,
    Account,
    Settings,
    Check,
    //Radio,
    Switch,
    Custom,
  };
  Q_ENUM(Style)
  /// @}

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (Style style READ style WRITE setStyle NOTIFY styleChanged)
#ifdef ENABLEPURPLE
  Q_PROPERTY (QString text READ text WRITE setText NOTIFY textChanged)
#endif // ENABLEPURPLE
  Q_PROPERTY (QUrl custom READ custom WRITE setCustom NOTIFY customChanged DESIGNABLE customEnabled)
  Q_PROPERTY (QUrl customHover READ customHover WRITE setCustomHover NOTIFY customHoverChanged DESIGNABLE customEnabled)
  Q_PROPERTY (QUrl customPushed READ customPushed WRITE setCustomPushed NOTIFY customPushedChanged DESIGNABLE customEnabled)
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Style m_style;
  QUrl m_custom, m_customHover, m_customPushed;

#ifdef ENABLEPURPLE
  QString m_text;
  std::unique_ptr<QLabel> m_label;
  QGraphicsDropShadowEffect *m_effect;
#endif // ENABLEPURPLE
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  KelGuiPushButton (QWidget *parent = 0);
  ~KelGuiPushButton() override;
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:

  Style style() const;
  void setStyle(const Style &style);

#ifdef ENABLEPURPLE
  QString text() const;
  void setText(const QString &text);
#endif // ENABLEPURPLE

  QUrl custom() const;
  void setCustom(const QUrl &custom);

  QUrl customHover() const;
  void setCustomHover(const QUrl &customHover);

  QUrl customPushed() const;
  void setCustomPushed(const QUrl &customPushed);

  bool customEnabled() const;

  /// @}

  /****************************************//**
   * @name PRIVATE METHODS
   *******************************************/
  /// @{
private:
  void updateStyle();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  //void paintEvent (QPaintEvent *) override;
  //void mousePressEvent (QMouseEvent *) override;
  //bool event(QEvent *) override;
  /// @}

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
#endif // KELGUIPUSHBUTTON_H
