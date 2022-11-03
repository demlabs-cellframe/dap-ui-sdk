#ifndef DAPQMLMODELMANAGECDB_H
#define DAPQMLMODELMANAGECDB_H

/* INCLUDES */
#include <QAbstractTableModel>
#include <QSharedPointer>

/* DEFS */
class QQmlEngine;
class QJSEngine;
class AbstractCdbManager;

class DapQmlModelManageCdb : public QAbstractTableModel
{
  Q_OBJECT

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
  Q_INVOKABLE void add (const QVariant &a_data);
  Q_INVOKABLE void edit (int a_index, const QVariant &a_data);
  Q_INVOKABLE void remove (int a_index);
  Q_INVOKABLE void move (int a_firstIndex, int a_secondIndex);
  Q_INVOKABLE void refreshContent();
  Q_INVOKABLE QVariant value (int a_index, const QString &a_name);
  Q_INVOKABLE void doImport (const QString &a_filename);
  Q_INVOKABLE void doExport (const QString &a_filename) const;
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
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetup();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELMANAGECDB_H
