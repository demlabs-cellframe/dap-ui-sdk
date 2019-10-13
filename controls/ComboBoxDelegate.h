#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QFrame>
#include <QApplication>
#include <QComboBox>
#include <QAbstractItemView>
#include <QDebug>

class ComboBoxDelegate : public QStyledItemDelegate
{
public:

    explicit ComboBoxDelegate(QObject *parent, QComboBox *cmb);
    static bool isSeparator(const QModelIndex &index);

protected:

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:

    QComboBox *m_combo;

};

#endif // COMBOBOXDELEGATE_H
