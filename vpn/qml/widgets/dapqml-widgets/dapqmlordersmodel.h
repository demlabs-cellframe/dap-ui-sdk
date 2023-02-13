#ifndef DAPQMLORDERSMODEL_H
#define DAPQMLORDERSMODEL_H

/* INCLUDES */
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

/****************************************//**
 * @brief orders model list
 * @ingroup groupUiModels
 * @date 02.2023
 * @author
 *******************************************/

class DapQmlOrdersModel : public QAbstractTableModel
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QStringList m_Countries;
  int         m_checkedIndex;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlOrdersModel(QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  Q_INVOKABLE void updateCheckedIndex();
  Q_INVOKABLE static bool countryExist();
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


class DapQmlOrdersSortFilterProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT
public:
  explicit DapQmlOrdersSortFilterProxyModel(QObject *parent = nullptr);
  Q_INVOKABLE void updateCheckedIndex();
  Q_INVOKABLE void setRowFilter(QString str);
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
private:
    QString m_filter;
    DapQmlOrdersModel * m_model;
};


/*-----------------------------------------*/
#endif // DAPQMLORDERSMODEL_H
