#ifndef LANGUAGE_H
#define LANGUAGE_H

/* INCLUDES */
#include <QWidget>
#include "baseform.h"
#include "model/languagemodel.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class Language; }
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for language list
 *
 * Choose different languages
 *
 * @ingroup groupUiClasses
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class Language : public BaseForm
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::Language *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit Language (QWidget *parent = nullptr);
  ~Language();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  LanguageModel *model();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigReturn();
  void sigSelect(int index, QString name);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotRetranslated();
  void slotSetCurrentLanguage (const QLocale::Language a_language, const QString a_languageName);
  /// @}
};

/*-----------------------------------------*/
#endif // LANGUAGE_H
