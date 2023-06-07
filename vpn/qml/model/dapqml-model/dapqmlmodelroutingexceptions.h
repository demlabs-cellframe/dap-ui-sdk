#ifndef DAPQMLMODELROUTINGEXCEPTIONS_H
#define DAPQMLMODELROUTINGEXCEPTIONS_H

/* INCLUDES */
#include <QAbstractTableModel>
#include <QImage>
#include <QQuickImageProvider>
#include <QSortFilterProxyModel>

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

    EXC_CHECKED_APPS,
    INC_CHECKED_APPS,
    EXC_SORTED_APPS,
    INC_SORTED_APPS,

    /* apps */
    ALL_APPS,

    /* routes */
    ALL_ROUTES,

    _SIZE,
  };
  Q_ENUM (Mode)

  enum Field
  {
    invalid       = Qt::UserRole + 10,
    appName       = Qt::DisplayRole,
    packageName   = Qt::UserRole + 20,
    checked,
    icon          = Qt::DecorationRole,

    address       = Qt::UserRole + 30,
    description,
  };
  Q_ENUM (Field)

  struct App
  {
    QString packageName, appName;
    struct
    {
      bool included;
      bool excluded;
    } checked;
  };

  struct Route
  {
    QString address, description;
  };

  struct AppsContainer
  {
    // only checked apps
    QList<App> checkedApps;
    // only unchecked apps
    QList<App> uncheckedApps;
    // sorted apps : checked at top, unchecked at the bottom
    QList<App> sortedApps;
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
  AppsContainer *_container;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlModelRoutingExceptions();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  Q_INVOKABLE int mode() const;
  Q_INVOKABLE void setMode (int a_newMode);

  Q_INVOKABLE void append (const App &a_app, const QString &a_icon);
  Q_INVOKABLE void append (App &&a_app, QString &&a_icon);
  Q_INVOKABLE void append (const Route &a_route);
  Q_INVOKABLE void append (Route &&a_route);
  Q_INVOKABLE void appendJson (const QVariant &a_value);
  void appendQuietly (App &&a_app, QString &&a_icon);

  Q_INVOKABLE void insert (int a_index, const App &a_app, const QString &a_icon);
  Q_INVOKABLE void insert (int a_index, App &&a_app, QString &&a_icon);
  Q_INVOKABLE void insert (int a_index, const Route &a_route);
  Q_INVOKABLE void insert (int a_index, Route &&a_route);
  Q_INVOKABLE void insertJson (int a_index, const QVariant &a_value);

  Q_INVOKABLE void replace (int a_index, const App &a_app, const QString &a_icon = QString());
  Q_INVOKABLE void replace (int a_index, App &&a_app, const QString &a_icon = QString());
  Q_INVOKABLE void replaceSorted (int a_index, const App &a_app, const QString &a_icon = QString());
  Q_INVOKABLE void replaceSorted (int a_index, App &&a_app, const QString &a_icon = QString());
  Q_INVOKABLE void replace (int a_index, const Route &a_route);
  Q_INVOKABLE void replace (int a_index, Route &&a_route);
  Q_INVOKABLE void replaceJson (int a_index, const QVariant &a_value);

  Q_INVOKABLE const App &app (int a_index) const;
  Q_INVOKABLE const App &appSorted (int a_index) const;
  Q_INVOKABLE const QImage &appIcon (const QString &a_packageName) const;
  Q_INVOKABLE const Route &route (int a_index) const;
  Q_INVOKABLE QVariant appJson (int a_index) const;
  Q_INVOKABLE QVariant routeJson (int a_index) const;

  Q_INVOKABLE int indexOfChecked (int a_index) const;
  Q_INVOKABLE int indexOfSorted (int a_index) const;

  Q_INVOKABLE int appSize() const;
  Q_INVOKABLE int routeSize() const;

  Q_INVOKABLE void removeApp (int a_index);
  Q_INVOKABLE void removeAppFromChecked (int a_index);
  Q_INVOKABLE void removeRoute (int a_index);

  Q_INVOKABLE void moveApp (int a_from, int a_to);
  Q_INVOKABLE void moveRoute (int a_from, int a_to);

  /// refresh checked and sorted models
  Q_INVOKABLE void sortAndUpdateAllLists();
  /// refresh checked and sorted models
  Q_INVOKABLE void updateAllLists();
  /// save routes
  Q_INVOKABLE void save() const;
  /// load routes
  Q_INVOKABLE void load();
  /// remove everything
  Q_INVOKABLE void clear();
  /// remove all apps
  Q_INVOKABLE void clearApps();
  /// remove all routes
  Q_INVOKABLE void clearRoutes();

  static QStringList getIncludedCheckedPackageList();
  static QStringList getExcludedCheckedPackageList();

  Q_INVOKABLE bool isExcludedList() const;
  Q_INVOKABLE bool isIncludedList() const;
  static bool isTestMode();
protected:
  void _appendCheckedApp (int a_index, bool a_combine = true);
//  void _removeCheckedApp (int a_index);
  void _sortCheckedApps();

  void _clearBeforeLoad();
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

/****************************************//**
 * @brief Routing Exceptions Image Provider
 *
 * Class that provides Images to QML Engine.
 *
 * The name format: packageName + ".png"
 *
 * Example:
 * > packageName    = "CoolPackageName.pak"
 * > image id name  = "CoolPackageName.pak.png"
 *
 * cpp: engine->addImageProvider ("DapQmlModelRoutingExceptionsImageProvider", new DapQmlModelRoutingExceptionsImageProvider);
 * qml: scaledPixmap: "image://DapQmlModelRoutingExceptionsImageProvider/" + model.packageName + ".png"
 *
 * @ingroup groupUiModels
 * @date 18.04.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelRoutingExceptionsImageProvider : public QQuickImageProvider
{
  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapQmlModelRoutingExceptionsImageProvider();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapQmlModelRoutingExceptionsImageProvider *instance();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  QImage requestImage (const QString &id, QSize *size, const QSize &requestedSize) override;
  QPixmap requestPixmap (const QString &id, QSize *size, const QSize &requestedSize) override;
  /// @}
};

class DapQmlModelRoutingExceptionsFilterProxy : public QSortFilterProxyModel
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QString m_filter;
  QAbstractListModel *_model;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlModelRoutingExceptionsFilterProxy (QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  void setModel (QAbstractListModel *a_model);
  Q_INVOKABLE const QString &filter() const;
  Q_INVOKABLE void setFilter (const QString &a_filter);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
protected:
  bool filterAcceptsRow (int sourceRow, const QModelIndex &sourceParent) const override;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELROUTINGEXCEPTIONS_H
