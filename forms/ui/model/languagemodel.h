#ifndef LANGUAGEMODEL_H
#define LANGUAGEMODEL_H

/* INCLUDES */
#include <QScrollArea>
#include <QBoxLayout>
#include <dapguiradio.h>
#include "modelbase.h"

/****************************************//**
 * @brief choose language model list widget
 * @ingroup groupUiModels
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class LanguageModel : public ModelBase
{
  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  QList<DapGuiRadio*> m_list;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit LanguageModel (QWidget *parent = nullptr);
  ~LanguageModel();
  /// @}

  /****************************************//**
   * @name PUBLIC SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetup();
  /// @}
};

/*-----------------------------------------*/
#endif // LANGUAGEMODEL_H
