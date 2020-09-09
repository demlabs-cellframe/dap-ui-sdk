#include "WidgetDelegateListView.h"

#include "Utilz.h"
#include "QPushButton"
#include <QScroller>
WidgetDelegateListView::WidgetDelegateListView(QWidget *a_parent /*= nullptr*/)
    :CustomComboBoxListView(a_parent)
{
//TODO

    qDebug() << "WidgetDelegateListView created";

    m_itemDelegate = new WidgetListViewItemDelegate(this);
    this->setItemDelegate(m_itemDelegate);
}


WidgetDelegateBase *WidgetDelegateListView::indexWidget(const QModelIndex &index)
{
    return qobject_cast<WidgetDelegateBase*>(this->QListView::indexWidget(index));
}

void WidgetDelegateListView::rowsInserted(const QModelIndex &parent, int start, int end)
{
    this->QListView::rowsInserted(parent, start, end);

    this->createIndexDelegates(start, end);
}

void WidgetDelegateListView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (!m_widgetDelegateFactory)
        return;

    for (auto l_selectedIndex: selected.indexes())
    {
        WidgetDelegateBase* selectedWidget = qobject_cast<WidgetDelegateBase*>(this->indexWidget(l_selectedIndex));
        if (selectedWidget)
            selectedWidget->setSelected(true);
    }
    for (auto l_deselectedIndex: deselected.indexes())
    {
        WidgetDelegateBase* deselectedWidget = qobject_cast<WidgetDelegateBase*>(this->indexWidget(l_deselectedIndex));
        if (deselectedWidget)
            deselectedWidget->setSelected(false);
    }

    this->CustomComboBoxListView::selectionChanged(selected, deselected);
}

void WidgetDelegateListView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    this->QListView::dataChanged(topLeft, bottomRight, roles);

    if (!m_widgetDelegateFactory)
        return;

    for (int row = topLeft.row(); row <= bottomRight.row(); row++)
    {
        QModelIndex index = model()->index(row, topLeft.column(), topLeft.parent());
        WidgetDelegateBase* widget = this->indexWidget(index);

        if (!widget)
            return;

        if (roles.isEmpty())
            widget->setData(this->model()->data(index));
        else
            for (int curRole: roles) {
                widget->setData(index.data(curRole), curRole);
            }
    }
}


void WidgetDelegateListView::createIndexDelegates(int a_start /*= 0*/, int a_end /*= -1*/)
{
    if (!m_widgetDelegateFactory || !this->model() || this->model()->rowCount() == 0)
        return;

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
            Q_UNUSED(a_size)
            emit m_itemDelegate->sizeHintChanged(index);
        });

        connect(widget, &WidgetDelegateBase::selected, [this, row]{
             emit this->itemSelected(row);
        });

        widget->setData(this->model()->itemData(index));
        widget->setSelected(this->selectionModel()->isSelected(index));

        this->setIndexWidget(index, widget);
    }
}

void WidgetDelegateListView::deleteAllWidgetDelegates()
{
    if (!m_widgetDelegateFactory || !this->model() || this->model()->rowCount() == 0)
        return;

    for (int row = 0; row <= this->model()->rowCount(); row++)
    {
        QWidget * widget = this->indexWidget(this->model()->index(row, 0));
        delete widget;
    }

}

WidgetDelegateBase *WidgetDelegateListView::createWidgetDelegate()
{
    if (m_widgetDelegateFactory)
        return m_widgetDelegateFactory();
    else
        return nullptr;
}

void WidgetDelegateListView::setWidgetDelegateFactory(WidgetDelegateListView::WidgetDelegateFactory *a_factoryFunction)
{
    if (m_widgetDelegateFactory == a_factoryFunction)
        return;

    if (m_widgetDelegateFactory && !a_factoryFunction)
    {
        this->deleteAllWidgetDelegates();
        m_widgetDelegateFactory = a_factoryFunction;
    }
    else
    {
        m_widgetDelegateFactory = a_factoryFunction;
        this->createIndexDelegates();
    }
}


// *************************** WidgetListViewItemDelegate ***************************

WidgetListViewItemDelegate::WidgetListViewItemDelegate(QObject *parent)
    :QStyledItemDelegate(parent)
{}

QSize WidgetListViewItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto listView = qobject_cast<const QListView*>(option.widget);
    if (!listView)
        return QStyledItemDelegate::sizeHint(option, index);

    auto itemWgt = listView->indexWidget(index);
    if (!itemWgt)
        return QStyledItemDelegate::sizeHint(option, index);

    return itemWgt->size();
}


void WidgetDelegateListView::setModel(QAbstractItemModel *model)
{
    CustomComboBoxListView::setModel(model);
    createIndexDelegates();
}
