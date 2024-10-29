#ifndef DAPQMLMODELNODEPROXYBASE_H
#define DAPQMLMODELNODEPROXYBASE_H

/* INCLUDES */

#include <QSortFilterProxyModel>

/****************************************//**
 * @brief node model proxy base
 * @ingroup groupUiModels
 *
 * Provides easy way to search items.
 *
 * Also has indexHashMap
 *
 * @date 27.02.2024
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelNodeProxyBase : public QSortFilterProxyModel
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY sigCurrentIndexChanged)
  /// @}

  /****************************************//**
   * @name DEFINES
   *******************************************/
  /// @{
public:
  class Bridge
  {
  public:
    virtual ~Bridge() = default;

    /// return amount of items inside model
    virtual int size() const = 0;

    /// get current index value
    virtual int currentIndex() const = 0;

    /// set current index value
    virtual bool setCurrentIndex (int a_value) = 0;

    /// return true, if indexing is being used (example: false for networks model)
    virtual bool isIndexed() const = 0;

    /// return true if filter is matching
    virtual bool filterAcceptsRow (int a_row, const QString &a_filter) const = 0;
  };
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  QString m_filter;
  Bridge *m_bridge;
  mutable QHash<int,int> _indexMap;
  mutable int _indexMapCounter;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlModelNodeProxyBase (QObject *a_parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:

  /* basic methods */

  /**
   * @brief filter by comparing mist field with provided filter
   * @note set empty string to disable filtering
   */
  Q_INVOKABLE void setRowFilter (const QString &a_filter);

  Bridge *bridge() const;
  void setBridge (Bridge *a_bridge);

  /* fields */

  Q_INVOKABLE int currentIndex() const;
  Q_INVOKABLE void setCurrentIndex (int a_value);

protected:
  void _printFilteredResult() const;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigCurrentIndexChanged();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
protected:
  bool filterAcceptsRow (int a_row, const QModelIndex &a_parent) const override;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELNODEPROXYBASE_H
