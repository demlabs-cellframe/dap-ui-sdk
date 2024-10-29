#include "ComboBoxDelegate.h"

ComboBoxDelegate::ComboBoxDelegate(QObject *parent, QComboBox *cmb):
    QStyledItemDelegate(parent),
    m_combo(cmb)
{
}


void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (isSeparator(index)) {
        QRect rect = option.rect;
            if (const QAbstractItemView *view = qobject_cast<const QAbstractItemView*>(option.widget))
                rect.setWidth(view->viewport()->width());
        QStyleOption opt;
        opt.rect = rect;
        m_combo->style()->drawPrimitive(QStyle::PE_IndicatorToolBarSeparator, &opt, painter, m_combo);
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}


QSize ComboBoxDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (isSeparator(index)) {
        return QSize(0, 1);
    }
    return QStyledItemDelegate::sizeHint(option, index);
}


bool ComboBoxDelegate::isSeparator(const QModelIndex &index) {
    return index.data(Qt::AccessibleDescriptionRole).toString() == QLatin1String("separator");
}
