#ifndef DAPQMLCOUNTRYMODEL_H
#define DAPQMLCOUNTRYMODEL_H

/* INCLUDES */
#include <QAbstractTableModel>

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
//  Q_INVOKABLE QString currentCategory() const;
  Q_INVOKABLE bool isCurrent (const QString &a_name) const;
  Q_INVOKABLE void updateCheckedIndex();
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

/*-----------------------------------------*/
#endif // DAPQMLCOUNTRYMODEL_H
