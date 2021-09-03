#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

/* INCLUDES */
#include <kelguibutton.h>
#include "modelbase.h"

/****************************************//**
 * @brief choose server model list widget
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class HistoryModel : public ModelBase
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
  explicit HistoryModel (QWidget *parent = nullptr);
  ~HistoryModel() override;
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
#endif // HISTORYMODEL_H
