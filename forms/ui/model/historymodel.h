#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

/* INCLUDES */
#include <dapguibutton.h>
#include "modelbase.h"

/****************************************//**
 * @brief watch licence history list widget
 * @ingroup groupUiModels
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
  QList<DapGuiButton*> m_list;
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

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  bool eventFilter(QObject *o, QEvent *e) override;
  /// @}
};

/*-----------------------------------------*/
#endif // HISTORYMODEL_H
