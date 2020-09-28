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

#define CUSTOM_MODEL_ICON_ROLE Qt::UserRole

class WidgetListViewItemDelegate;

class WidgetDelegateListView : public CustomComboBoxListView
{
    Q_OBJECT
    // @brief When 'usingImageFromModel' property is false, this ignore any images model send in
    Q_PROPERTY(bool usingImageFromModel READ usingImageFromModel WRITE setUsingImageFromModel)

public:
    typedef WidgetDelegateBase* (WidgetDelegateFactory)();
    WidgetDelegateListView(QWidget *a_parent = nullptr);

    void setUsingImageFromModel(bool a_use);
    bool usingImageFromModel();

    virtual WidgetDelegateBase* createWidgetDelegate(); //Reimplement this method for your oun widget delegate.

    void setWidgetDelegateFactory(WidgetDelegateFactory* a_factoryFunction);

    WidgetDelegateBase* indexWidget(const QModelIndex &index);
    
    virtual void setModel(QAbstractItemModel *model) override;

signals:
    void styleChange(const QString& propertyName, QVariant value);
protected slots:
    virtual void rowsInserted(const QModelIndex &parent, int start, int end) override;
    virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;
    virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()) override;

private:
    void createIndexDelegates(int a_start = 0, int a_end = -1);
    void deleteAllWidgetDelegates();
    void setDelegateData(WidgetDelegateBase* a_delegate, QMap<int, QVariant> a_dataMap);
    void setDelegateData(WidgetDelegateBase* a_delegate, const QVariant &a_value, int a_role = Qt::DisplayRole);
    void updateDelegateImages();
    WidgetDelegateBase* widgetDelegate(const QModelIndex& a_index);

    WidgetListViewItemDelegate *m_itemDelegate{};
    WidgetDelegateFactory* m_widgetDelegateFactory{};

    bool m_usingImageFromModel = false;
};

class WidgetListViewItemDelegate: public QStyledItemDelegate
{
public:
    explicit WidgetListViewItemDelegate(QObject *parent = Q_NULLPTR);

    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;;
};

#endif // WIDGETDELEGATELISTVIEW_H
