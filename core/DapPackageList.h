#ifndef DAPPACKAGELIST_H
#define DAPPACKAGELIST_H

#include <QObject>
#include <QImage>
#include <QMap>
#include "DapDataLocal.h"

/* DEFS */
struct DapPackageListItem
{
    DapPackageListItem(QString name, QString lable, QImage icon){
        mPackageName = name;
        mPackageLable = lable;
        appIcon = icon;
    }

    QString mPackageName;
    QString mPackageLable;
    QImage appIcon;
};

class DapPackageList : public QObject
{
    Q_OBJECT
public:
    explicit DapPackageList(QObject *parent = nullptr);

    void clearList();
    void addItem(QString name, QString lable, QImage icon);

    void addAppToExcludeForRoutingList(DapPackageListItem item);

    void load();
    void save();

private:
    QMap<int, DapPackageListItem> m_packListItems;
    QMap<int, DapPackageListItem> m_exceptionRoutingPackListItem;

signals:
    void sigExceptionFromRoutingListLoaded();
};

#endif // DAPPACKAGELIST_H
