#ifndef DAPROUTINGEXCEPTIONSLIST_H
#define DAPROUTINGEXCEPTIONSLIST_H

#include <QObject>
#include <QMap>

/* DEFS */
struct DapRoutingExceptionsListApp
{
    DapRoutingExceptionsListApp(QString name, QString lable, QString icon){
        m_packageName = name;
        m_appName = lable;
        m_icon = icon;
    }

    QString m_packageName, m_appName, m_icon;
    bool checked;
};

struct DapRoutingExceptionsListRoute
{
  QString address, description;
};

class DapRoutingExceptionsList : public QObject
{
    Q_OBJECT
public:
    explicit DapRoutingExceptionsList(QObject *parent = nullptr);

    void clearList();
    void setSize(int a_size){
        packageListSize = a_size;
    }
    int size(){return packageListSize;}
    void addAppRoutingExceptionsList(DapRoutingExceptionsListApp item);
    QList<DapRoutingExceptionsListApp> apps() const;
    QList<DapRoutingExceptionsListRoute> routes() const;

    void load();
    void save();

private:
    int packageListSize;
    QMap<int, DapRoutingExceptionsListApp> m_packageList;
    QMap<int, DapRoutingExceptionsListApp> m_routingExceptionsPackList;

signals:
    void sigRoutingExceptionsListLoaded();
    void sigRoutingExceptionsListFilled();
    void sigGetPackgeInfo(QString package_name);
public slots:
    void addPackageInfo(QString packageName, QString appName, QString icon);

};

#endif // DAPROUTINGEXCEPTIONSLIST_H
