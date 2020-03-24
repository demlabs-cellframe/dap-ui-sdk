#ifndef WIDGETDELEGATELISTVIEW_H
#define WIDGETDELEGATELISTVIEW_H

#include <QListView>
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QDebug>
#include "WidgetDelegateBase.h"
#include "WidgetDelegateFactoryBase.h"

class WidgetDelegateListView : public QListView
{
    Q_OBJECT
public:
    WidgetDelegateListView(QWidget *a_parent = nullptr);
    virtual ~WidgetDelegateListView();

//    virtual void setModel(QAbstractItemModel *a_model) override;

    WidgetDelegateFactoryBase *delegateFactory() const;

    template<typename T>
    void setDelegateFactory(T *a_delegateFactory = nullptr);

    WidgetDelegateBase* indexWidget(const QModelIndex &index);

    virtual void setModel(QAbstractItemModel *a_model) override;
protected slots:
    virtual void rowsInserted(const QModelIndex &parent, int start, int end) override;
    virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()) override;

signals:
    void itemSelected(int index);

private:
    void createIndexDelegates(int a_start = 0, int a_end = -1);

    WidgetDelegateFactoryBase* m_delegateFactory = nullptr;
};



template<typename T>
void WidgetDelegateListView::setDelegateFactory(T *a_delegateFactory)
{
    if (!a_delegateFactory)
        a_delegateFactory = new T;
    else if (m_delegateFactory == a_delegateFactory)
        return;

    delete m_delegateFactory;
    m_delegateFactory = a_delegateFactory;

    if (!this->model())
        return;

    int rowCount = this->model()->rowCount();
    if (rowCount)
        this->createIndexDelegates();
}

#endif // WIDGETDELEGATELISTVIEW_H
