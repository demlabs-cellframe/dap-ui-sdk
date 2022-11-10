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
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (QString hook READ hook NOTIFY sigRefresh)
  Q_PROPERTY (int hookInt READ hookInt NOTIFY sigRefresh)
  /// @}

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

  /// this basicaly provides update feature for QML variable fields
  Q_INVOKABLE QString hook();
  Q_INVOKABLE int hookInt();
  void refresh();
  /// Set current server name
  void setCheckedServer(QString name);
  /// Return previous server name which was before setCheckedServer
  QString previousServer() { return m_previousServer; }
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;

  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
//  Q_INVOKABLE QVariant value (int a_row, const QString &a_name);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigRefresh();
  /// @}
private:
  /// Current server name
  QString m_currentServer;
  /// This name is used to undo changes
  QString m_previousServer;
};

/*-----------------------------------------*/
#endif // DAPQMLMODELCHOOSESERVER_H
