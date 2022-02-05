#ifndef BUGREPORTSMODEL_H
#define BUGREPORTSMODEL_H

/* INCLUDES */
#include <dapguibutton.h>
#include "modelbase.h"
#include "DapDataLocal.h"

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
  QList<DapGuiButton*> m_list;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit BugReportsModel (QWidget *parent = nullptr);
  ~BugReportsModel();
  bool eventFilter(QObject *o, QEvent *e) override;
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
