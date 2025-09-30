#ifndef DAPQMLMODELCHOOSESERVER_H
#define DAPQMLMODELCHOOSESERVER_H

/* INCLUDES */
#include <QAbstractTableModel>

/****************************************//**
 * @brief servers model list
 *
 * A bridge from old environment to the new
 * server list features
 *
 * @see @ref DapQmlModelFullServerList
 *
 * @ingroup groupUiModels
 * @date 11.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelChooseServer : public QAbstractListModel
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{

  /// Current server name
  QString m_currentServer;
  /// This name is used to undo changes
  QString m_previousServer;

  /// @}

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (QString hook READ hook    NOTIFY sigRefresh)
  Q_PROPERTY (int hookInt  READ hookInt NOTIFY sigRefresh)
  Q_PROPERTY (int current  READ current WRITE setCurrent  NOTIFY currentChanged)
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

  /// this basicaly provides update feature for QML variable fields
  Q_INVOKABLE QString hook();
  /// @see hook
  Q_INVOKABLE int hookInt();
  /// notify QML elements of need to refresh/redraw
  void refresh();
  /// @return current server index
  Q_INVOKABLE int current() const;
  /// set current server index
  Q_INVOKABLE void setCurrent (int a_newCurrentServer);
  /// set current server index by name
  /// @note it is also emits dataChanged for previous and current items
  void setCurrentServerByName (const QString &a_name);
  /// return previous server name which was before setCheckedServer
  QString previousServer();

  void allowModelReset (int a_amount = 1);
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
  void currentChanged();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELCHOOSESERVER_H
