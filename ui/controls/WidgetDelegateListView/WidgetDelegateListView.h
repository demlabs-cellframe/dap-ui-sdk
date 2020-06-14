#ifndef WIDGETDELEGATELISTVIEW_H
#define WIDGETDELEGATELISTVIEW_H

#include <QStyledItemDelegate>
#include <QListView>
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QDebug>
#include <QGestureEvent>
#include "WidgetDelegateBase.h"
#include "CustomComboBoxListView.h"

class WidgetListViewItemDelegate;

class WidgetDelegateListView : public CustomComboBoxListView
{
    Q_OBJECT
public:
    typedef WidgetDelegateBase* (WidgetDelegateFactory)();
    WidgetDelegateListView(QWidget *a_parent = nullptr);

    virtual WidgetDelegateBase* createWidgetDelegate(); //Reimplement this method for your oun wdget delegate.

    void setWidgetDelegateFactory(WidgetDelegateFactory* a_factoryFunction);

    WidgetDelegateBase* indexWidget(const QModelIndex &index);

protected slots:
    virtual void rowsInserted(const QModelIndex &parent, int start, int end) override;
    virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()) override;

private:
    void createIndexDelegates(int a_start = 0, int a_end = -1);
    void deleteAllWidgetDelegates();

    WidgetListViewItemDelegate *m_itemDelegate{};
    WidgetDelegateFactory* m_widgetDelegateFactory{};
};

class WidgetListViewItemDelegate: public QStyledItemDelegate
{
public:
    explicit WidgetListViewItemDelegate(QObject *parent = Q_NULLPTR);

    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;;
};

#endif // WIDGETDELEGATELISTVIEW_H
