#ifndef DAPQMLMODELCHOOSECOUNTRY_H
#define DAPQMLMODELCHOOSECOUNTRY_H

/* INCLUDES */
#include <QAbstractListModel>

/****************************************//**
 * @brief country list model
 * @ingroup groupUiModels
 * @date 12.05.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelChooseCountry : public QAbstractListModel
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (int current READ current WRITE setCurrent NOTIFY currentChanged)
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  int m_current;
  QStringList _list;
  QString _filter;
  QVector<quint32> _indexes;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlModelChooseCountry (QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  Q_INVOKABLE void setRowFilter (const QString &a_filter);
  Q_INVOKABLE static bool countryExist();

  Q_INVOKABLE int current() const;
  Q_INVOKABLE void setCurrent (int newCurrent);
  Q_INVOKABLE void setCurrent (const QString &a_name);

protected:
  void _applyFiltering();
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
  void currentChanged();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELCHOOSECOUNTRY_H
