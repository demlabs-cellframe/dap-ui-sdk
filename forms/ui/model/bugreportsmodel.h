#ifndef BUGREPORTSMODEL_H
#define BUGREPORTSMODEL_H

/* INCLUDES */
#include <kelguibutton.h>
#include "modelbase.h"

/****************************************//**
 * @brief watch bugreports history list widget
 * @ingroup groupUiModels
 * @date 21.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class BugReportsModel : public ModelBase
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  QList<KelGuiButton*> m_list;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit BugReportsModel (QWidget *parent = nullptr);
  ~BugReportsModel();
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
#endif // BUGREPORTSMODEL_H
