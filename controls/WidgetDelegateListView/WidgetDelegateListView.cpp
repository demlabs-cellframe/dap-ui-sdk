#include "WidgetDelegateListView.h"

#include "WidgetDelegateFactoryBase.h"
#include "QPushButton"

WidgetDelegateListView::WidgetDelegateListView(QWidget *a_parent /*= nullptr*/)
    :QListView(a_parent)
{
//TODO

}

WidgetDelegateListView::~WidgetDelegateListView()
{
    delete m_delegateFactory;
    m_delegateFactory = nullptr;
}

//void WidgetDelegateListView::setModel(QAbstractItemModel *a_model)
//{
// //TODO
//    this->QListView::setModel(a_model);
//}

WidgetDelegateFactoryBase *WidgetDelegateListView::delegateFactory() const
{
    return m_delegateFactory;
}


WidgetDelegateBase *WidgetDelegateListView::indexWidget(const QModelIndex &index)
{
    return qobject_cast<WidgetDelegateBase*>(this->QListView::indexWidget(index));
}

void WidgetDelegateListView::setModel(QAbstractItemModel *a_model)
{
    this->QListView::setModel(a_model);

    this->createIndexDelegates();

}

void WidgetDelegateListView::rowsInserted(const QModelIndex &parent, int start, int end)
{
    this->QListView::rowsInserted(parent, start, end);

    if (!this->m_delegateFactory)
        return;

    this->createIndexDelegates(start, end);
}

void WidgetDelegateListView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    this->QListView::dataChanged(topLeft, bottomRight, roles);

    if (!this->m_delegateFactory)
        return;

    for (int row = topLeft.row(); row <= bottomRight.row(); row++)
    {
        QModelIndex index = model()->index(row, topLeft.column(), topLeft.parent());
        WidgetDelegateBase* widget = this->indexWidget(index);

        if (!widget)
            continue;

        for (int curRole: roles) {
            widget->setData(index.data(curRole), curRole);
        }
    }
}


void WidgetDelegateListView::createIndexDelegates(int a_start /*= 0*/, int a_end /*= -1*/)
{
    int rowCount = this->model()->rowCount();

    if (a_end == -1 || a_end >= rowCount)
        a_end = rowCount - 1;

    if (a_end < a_start)
    {
        qErrnoWarning("createIndexDelegates() wrong parameters: a_start > a_end");
        return;
    }

    for (int row = a_start; row <= a_end; row++)
    {
        QModelIndex index = model()->index(row, 0);
        WidgetDelegateBase* widget = this->m_delegateFactory->createWidgetDelegate();

        connect(widget, &WidgetDelegateBase::sizeChanged, [this, index](const QSize& a_size){
             this->model()->setData(index, a_size, Qt::SizeHintRole);
        });

        connect(widget, &WidgetDelegateBase::selected, [this, row]{
             emit this->itemSelected(row);
        });

        widget->setData(this->model()->itemData(index));
        this->setIndexWidget(index, widget);

        this->model()->setData(index, widget->size(), Qt::SizeHintRole);

    }
}

