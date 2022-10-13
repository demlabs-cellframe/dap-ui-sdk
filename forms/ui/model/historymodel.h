#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

/* INCLUDES */
//#include <dapguibutton.h>
#include "modelbase.h"
#include <QTimer>

/* DEFS */
class DapGuiButton;

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

signals:
  void filled(int rowCount);
};

/// button animation
class _ButtonClickAnimation : public QObject
{
  Q_OBJECT

  QScopedPointer<QTimer> m_timer;
  DapGuiButton *m_button;

public:
  _ButtonClickAnimation (DapGuiButton *a_button);

public slots:
  void slotStart();
  void slotFinish();
};

/*-----------------------------------------*/
#endif // HISTORYMODEL_H
