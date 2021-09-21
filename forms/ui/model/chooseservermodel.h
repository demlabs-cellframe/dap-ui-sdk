#ifndef CHOOSESERVERMODEL_H
#define CHOOSESERVERMODEL_H

/* INCLUDES */
#include <QAbstractListModel>
#include <kelguiradio.h>
#include "modelbase.h"

/* DEFS */
class ChooseServer;

/****************************************//**
 * @brief choose server model list widget
 * @ingroup groupUiModels
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class ChooseServerModel : public ModelBase
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  QAbstractListModel *m_model;
  ChooseServer *m_cs;
  QList<KelGuiRadio*> m_list;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit ChooseServerModel (QWidget *parent = nullptr);
  ~ChooseServerModel() override;
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:
  /// set model into this widget
  /// @note old model will not be deleted
  void setModel (QAbstractListModel *model, ChooseServer *cs);
  QAbstractListModel *model() const;
  /// @}

  /****************************************//**
   * @name PUBLIC SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetup();
private slots:
  /// radio button toggle
  void slotToggled();
  /// @}
signals:
  void filled();
};

/*-----------------------------------------*/
#endif // CHOOSESERVERMODEL_H
