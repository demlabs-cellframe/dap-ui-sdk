#ifndef DAPQMLMODELCHOOSESERVER_H
#define DAPQMLMODELCHOOSESERVER_H

/* INCLUDES */
#include <QAbstractTableModel>
#include "dapqml-abstract/abstractservermanager.h"

/****************************************//**
 * @brief servers model list
 * @ingroup groupUiModels
 * @date 11.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelChooseServer : public QAbstractListModel
{
  Q_OBJECT

  QSharedPointer<AbstractServerManager> m_serverManager;

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  explicit DapQmlModelChooseServer (QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapQmlModelChooseServer *instance();
  void setServerManager (QSharedPointer<AbstractServerManager> a_serverManager);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;

  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELCHOOSESERVER_H
