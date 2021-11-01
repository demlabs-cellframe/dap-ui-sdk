#ifndef SETTINGS_H
#define SETTINGS_H

/* INCLUDES */
#include <QWidget>
#include "baseform.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class Settings; }
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for settings
 *
 * Uses model to display options.
 *
 * @note several options is temporeraly disabled
 *
 * @ingroup groupUiClasses
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class Settings : public BaseForm
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::Settings *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit Settings (QWidget *parent = nullptr);
  ~Settings();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetVersion (const QString &a_text);
  void slotRetranslated();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  /* settings */
  void sigLicenceGet();
  void sigLicenceReset();
  void sigLanguage();
  void sigSetDaysLeft (QString days);
  void sigResetDaysLeft();

  /* support */
  void sigBugSend();
  void sigTelegramBot();

  /* info */
  void sigBugReport();
  void sigLicenceHistory();
  void sigTermsOfUse();
  void sigPrivacyPolicy();
  void sigVersion();
  /// @}
};

/*-----------------------------------------*/
#endif // SETTINGS_H
