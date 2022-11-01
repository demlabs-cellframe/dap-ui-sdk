#ifndef DAPQMLNODEDATAMODEL_H
#define DAPQMLNODEDATAMODEL_H

/* INCLUDES */
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

/****************************************//**
 * @brief themes model list
 * @ingroup groupUiModels
 * @date 08.2022
 * @author
 *******************************************/

class DapQmlNodeDataModel : public QAbstractTableModel
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QStringList m_nodeData;
  int         m_checkedIndex;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlNodeDataModel(QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  Q_INVOKABLE void updateCheckedIndex(QString checkedName);
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
};


class DapQmlNodeDataSortFilterProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT
public:
  explicit DapQmlNodeDataSortFilterProxyModel(QObject *parent = nullptr);
  Q_INVOKABLE void updateCheckedIndex(QString checkedName);
  Q_INVOKABLE void setRowFilter(QString str);
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
private:
    QString m_filter;
    DapQmlNodeDataModel * m_model;
};


/*-----------------------------------------*/
#endif // DAPQMLNODEDATAMODEL_H
