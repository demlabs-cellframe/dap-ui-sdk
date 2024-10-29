#ifndef COUNTRY_H
#define COUNTRY_H

/* INCLUDES */
#include <QWidget>
#include <QLabel>
#include "baseform.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class Country; }
QT_END_NAMESPACE

class CountryModel;

/****************************************//**
 * @brief ui/class for country list
 *
 * Container for country list.
 *
 * Used CountryModel to control contents
 *
 * Sends sigSelect signal on server selection
 *
 * @ingroup groupUiClasses
 * @date 08.2022
 *******************************************/

class Country : public BaseForm
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::Country *ui;
  QWidget *m_overlay;
  QLabel *m_spinner;
  QMovie *m_movie;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit Country (QWidget *parent = nullptr);
  ~Country();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  CountryModel *model();
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
  void slotSetCountry (const QString a_country);
  /// @}
protected:
    void showEvent(QShowEvent *event) override;
};

/*-----------------------------------------*/
#endif // COUNTRY_H
