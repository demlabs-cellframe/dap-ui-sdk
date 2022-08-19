#ifndef COUNTRYMODEL_H
#define COUNTRYMODEL_H

/* INCLUDES */
#include <QStringList>
#include <dapguiradio.h>
#include "modelbase.h"

/* DEFS */
class Country;

/****************************************//**
 * @brief choose server model list widget
 * @ingroup groupUiModels
 * @date 08.2022
  *******************************************/

class CountryModel : public ModelBase
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  QStringList m_array;
  Country *m_cs;
  QList<DapGuiRadio*> m_list;
  bool _hook;
  int m_currentIndex;
  QString m_currentText;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit CountryModel (QWidget *parent = nullptr);
  ~CountryModel() override;
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:
  /// set model into this widget
  /// @note old model will not be deleted
  void setCountryList (const QStringList &a_array, Country *a_cs);
  void setCurrentCountry (const QString a_country);
  const QStringList &array() const;
  /// @}

  /****************************************//**
   * @name PUBLIC SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetup();
  void slotRetranslate();
  void viewFilter(QStringList showItems);
private slots:
  /// radio button toggle
  void slotToggled (bool checked);
  /// @}
signals:
  void filled();
};

/*-----------------------------------------*/
#endif // COUNTRYMODEL_H
