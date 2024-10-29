#ifndef CHOOSESERVERMODEL_H
#define CHOOSESERVERMODEL_H

/* INCLUDES */
#include <QAbstractListModel>
#include <dapguiradio.h>
#include "modelbase.h"
#include "DapServersData.h"

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
  QList<DapGuiRadioBase*> m_list;
  bool _hook;
  int m_currentIndex;
  QString m_currentText;
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
  void setSelectedItem(QString name);
private slots:
  /// radio button clicked
  void slotToggled ();
  /// @}
signals:
  void filled();
};

/*-----------------------------------------*/
#endif // CHOOSESERVERMODEL_H
