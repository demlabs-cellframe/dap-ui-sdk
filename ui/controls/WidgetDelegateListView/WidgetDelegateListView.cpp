#include "WidgetDelegateListView.h"

#include "QPushButton"
#include <QScroller>
WidgetDelegateListView::WidgetDelegateListView(QWidget *a_parent /*= nullptr*/)
    :CustomComboBoxListView(a_parent)
{
//TODO

    qDebug() << "WidgetDelegateListView created";
}


WidgetDelegateBase *WidgetDelegateListView::indexWidget(const QModelIndex &index)
{
    return qobject_cast<WidgetDelegateBase*>(this->QListView::indexWidget(index));
}

void WidgetDelegateListView::setModel(QAbstractItemModel *a_model)
{
    this->QListView::setModel(a_model);
}

void WidgetDelegateListView::rowsInserted(const QModelIndex &parent, int start, int end)
{
    this->QListView::rowsInserted(parent, start, end);

    this->createIndexDelegates(start, end);
}

void WidgetDelegateListView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    this->QListView::dataChanged(topLeft, bottomRight, roles);

    for (int row = topLeft.row(); row <= bottomRight.row(); row++)
    {
        QModelIndex index = model()->index(row, topLeft.column(), topLeft.parent());
        WidgetDelegateBase* widget = this->indexWidget(index);

        if (!widget)
            return;

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

        WidgetDelegateBase* widget = this->createWidgetDelegate();
        if (!widget)
            return;

        connect(widget, &WidgetDelegateBase::sizeChanged, [this, index](const QSize& a_size){
             this->model()->setData(index, a_size, Qt::SizeHintRole);
        });

        connect(widget, &WidgetDelegateBase::selected, [this, row]{
             emit this->itemSelected(row);
        });

        widget->setData(this->model()->itemData(index));

        this->setIndexWidget(index, widget);
    }
}


WidgetDelegateBase *WidgetDelegateListView::createWidgetDelegate()
{
    return nullptr;
}
