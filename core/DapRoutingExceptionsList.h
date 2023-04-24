#ifndef DAPROUTINGEXCEPTIONSLIST_H
#define DAPROUTINGEXCEPTIONSLIST_H

/* INCLUDES */
#include <QObject>
#include <QList>

/****************************************//**
 * @brief Routing Exceptions List
 *******************************************/

class DapRoutingExceptionsList : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  struct App
  {
    QString packageName, appName, icon;
    bool checked;
  };

  struct Route
  {
    QString address, description;
  };
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  int m_packageListSize;
  QList<App> m_packageList;
  QList<App> m_routingExceptionsPackList;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapRoutingExceptionsList (QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  int size();
  void setSize (int a_size);

  const QList<App> &apps() const;
  const QList<Route> &routes() const;

  void clearList();
  void load();
  void save();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigRoutingExceptionsListLoaded();
  void sigRoutingExceptionsListFilled();
  void sigGetPackgeInfo (QString a_packageName);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotAddPackageInfo (const QString &a_packageName, const QString &appName, const QString &a_icon);
  void slotAddAppRoutingExceptionsList (const App &a_item);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPROUTINGEXCEPTIONSLIST_H
