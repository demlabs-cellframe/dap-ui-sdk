#ifndef DAPQMLCOUNTRYMODEL_H
#define DAPQMLCOUNTRYMODEL_H

/* INCLUDES */
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

/****************************************//**
 * @brief themes model list
 * @ingroup groupUiModels
 * @date 08.2022
 * @author
 *******************************************/

class DapQmlCountryModel : public QAbstractTableModel
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
  explicit DapQmlCountryModel(QObject *parent = nullptr);
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


class DapQmlCountrySortFilterProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT
public:
  explicit DapQmlCountrySortFilterProxyModel(QObject *parent = nullptr);
  Q_INVOKABLE void updateCheckedIndex();
  Q_INVOKABLE void setRowFilter (const QString &a_filter);
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
private:
    QString m_filter;
    DapQmlCountryModel * m_model;
};


/*-----------------------------------------*/
#endif // DAPQMLCOUNTRYMODEL_H
