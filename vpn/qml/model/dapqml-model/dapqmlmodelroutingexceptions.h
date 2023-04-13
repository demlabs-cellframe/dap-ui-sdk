#ifndef DAPQMLMODELROUTINGEXCEPTIONS_H
#define DAPQMLMODELROUTINGEXCEPTIONS_H

/* INCLUDES */
#include <QAbstractTableModel>

/****************************************//**
 * @brief Routing Exceptions model
 * @ingroup groupUiModels
 * @date 13.04.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelRoutingExceptions : public QAbstractListModel
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum Mode
  {
    NONE,
    APPS,
    ROUTES,
  };
  Q_ENUMS (Mode)

  struct App
  {
    QString name, icon;
  };

  struct Route
  {
    QString address, description;
  };

  /// @}

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (int mode READ mode WRITE setMode NOTIFY sigModeChanged)
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  Mode m_mode;
  QAbstractListModel *_model;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  explicit DapQmlModelRoutingExceptions();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  Q_INVOKABLE int mode() const;
  Q_INVOKABLE void setMode (int a_newMode);

  Q_INVOKABLE void append (const App &a_app);
  Q_INVOKABLE void append (App &&a_app);
  Q_INVOKABLE void append (const Route &a_route);
  Q_INVOKABLE void append (Route &&a_route);
  Q_INVOKABLE void appendJson (const QVariant &a_value);

  Q_INVOKABLE void insert (int a_index, const App &a_app);
  Q_INVOKABLE void insert (int a_index, App &&a_app);
  Q_INVOKABLE void insert (int a_index, const Route &a_route);
  Q_INVOKABLE void insert (int a_index, Route &&a_route);
  Q_INVOKABLE void appendJson (int a_index, const QVariant &a_value);

  Q_INVOKABLE const App &app (int a_index) const;
  Q_INVOKABLE const Route &route (int a_index) const;
  Q_INVOKABLE QVariant appJson (int a_index) const;
  Q_INVOKABLE QVariant routeJson (int a_index) const;

  Q_INVOKABLE void removeApp (int a_index);
  Q_INVOKABLE void removeRoute (int a_index);

  Q_INVOKABLE void moveApp (int a_from, int a_to);
  Q_INVOKABLE void moveRoute (int a_from, int a_to);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigModeChanged();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELROUTINGEXCEPTIONS_H
