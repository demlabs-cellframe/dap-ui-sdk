#ifndef DAPQMLMODELMANAGECDB_H
#define DAPQMLMODELMANAGECDB_H

/* INCLUDES */
#include <QAbstractTableModel>
#include <QSharedPointer>

/* DEFS */
class QQmlEngine;
class QJSEngine;
class AbstractCdbManager;
class DapQmlModelManageCdbRowsCtl;

class DapQmlModelManageCdb : public QAbstractTableModel
{
  Q_OBJECT

  friend class DapQmlModelManageCdbRowsCtl;

  Q_PROPERTY(int notifyInt READ notifyInt NOTIFY sigMoveFilterChanged)
  Q_PROPERTY(QString notifyString READ notifyString NOTIFY sigMoveFilterChanged)

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  DapQmlModelManageCdbRowsCtl *d;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  DapQmlModelManageCdb();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapQmlModelManageCdb *instance();
  Q_INVOKABLE static QObject *singletonProvider (QQmlEngine *, QJSEngine *);
  void installManager (QSharedPointer<AbstractCdbManager> a_manager);

  Q_INVOKABLE int length() const;
  Q_INVOKABLE bool add (const QVariant &a_data);
  Q_INVOKABLE bool edit (int a_index, const QVariant &a_data);
  Q_INVOKABLE void remove (int a_index);
  Q_INVOKABLE void move (int a_firstIndex, int a_secondIndex);
  Q_INVOKABLE void refreshContent();
  Q_INVOKABLE QVariant value (int a_index, const QString &a_name);
  Q_INVOKABLE void doImport (const QString &a_filename);
  Q_INVOKABLE void doExport (const QString &a_filename) const;

  Q_INVOKABLE void setMoveFilter (int a_from, int a_to);

  Q_INVOKABLE int notifyInt() { return 0; }
  Q_INVOKABLE QString notifyString() { return QString(); }
  Q_INVOKABLE void regRow (QObject *a_row);
  Q_INVOKABLE void regWindow (QObject *a_window);

  Q_INVOKABLE void updateRows();
  Q_INVOKABLE int checkDrop (const QPoint &a_point);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  int columnCount (const QModelIndex &parent = QModelIndex()) const override;

  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigSetMoveFilter (int a_from, int a_to);
  void sigMoveFilterChanged();
  void sigError (QString a_errorText);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
//  void slotSetup();
  void slotReceivedPing (const QString &a_address, quint16 a_port, quint16 a_ping);

protected slots:
  void _slotUpdatePingsTimeout();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELMANAGECDB_H
