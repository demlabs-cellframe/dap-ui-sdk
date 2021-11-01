#ifndef SETTINGSMODEL_H
#define SETTINGSMODEL_H

/* INCLUDES */
#include "modelbase.h"
#include "style/dapguistylemanager.h"

class Settings;

/****************************************//**
 * @brief settings model list widget
 * @ingroup groupUiModels
 * @date 31.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class SettingsModel : public ModelBase
{
  DAPGUI_ENABLECSS
  //DAPGUI_ENABLEWIDGETSTYLE

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
private:
  typedef QString TextStyle;
  typedef void (*ItemCB) ();
  typedef QString (*TranslatingString) ();

  enum StyleId
  {
    SI_TITLE,
    SI_TITLETOP,
    SI_BUTTON,
    SI_BUTTONRED,
    SI_BUTTONGRAY,
    SI_LINK,
    SI_SPACER
  };

  struct Info
  {
    TextStyle style[2];
  };

  struct _SItem
  {
    StyleId sid;
    TranslatingString text[2];
    QString iconCss;
    ItemCB cb;
  };
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  static QMap<StyleId, Info> s_presets;
  static QList<_SItem> s_items;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit SettingsModel (QWidget *parent = nullptr);
  ~SettingsModel() override;
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:
  void setInterface (Settings *s);
  void setVersionText (const QString &a_text);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetup();
  void slotSetDaysLeft (QString days);
  void slotResetDaysLeft ();
  void slotClicked();
  void slotRetranslate();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  bool eventFilter(QObject *o, QEvent *e) override;
  /// @}
};

/*-----------------------------------------*/
#endif // SETTINGSMODEL_H
