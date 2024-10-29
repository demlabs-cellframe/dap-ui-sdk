#ifndef CHOOSETHEME_H
#define CHOOSETHEME_H

/* INCLUDES */
#include <QWidget>
#include <QLabel>
#include "baseform.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class ChooseTheme; }
QT_END_NAMESPACE

class ChooseThemeModel;

/****************************************//**
 * @brief ui/class for server list
 *
 * Container for server list.
 *
 * Used ChooseServerModel to control contents
 *
 * Sends sigSelect signal on server selection
 *
 * @ingroup groupUiClasses
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class ChooseTheme : public BaseForm
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::ChooseTheme *ui;
  QWidget *m_overlay;
  QLabel *m_spinner;
  QMovie *m_movie;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit ChooseTheme (QWidget *parent = nullptr);
  ~ChooseTheme();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  ChooseThemeModel *model();
  void showOverlay();
  void hideOverlay();
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
  void slotSetColorTheme (const QString a_colorTheme);
  /// @}
};

/*-----------------------------------------*/
#endif // CHOOSESERVER_H
