#ifndef CHOOSESERVERMODEL_H
#define CHOOSESERVERMODEL_H

/* INCLUDES */
#include <QAbstractListModel>
#include <kelguiradio.h>
#include "modelbase.h"

/****************************************//**
 * @brief choose server model list widget
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
  void setModel (QAbstractListModel *model);
  QAbstractListModel *model() const;
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
#endif // CHOOSESERVERMODEL_H
