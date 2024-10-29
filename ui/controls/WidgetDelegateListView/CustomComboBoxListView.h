#ifndef CUSTOMCOMBOBOXLISTVIEW_H
#define CUSTOMCOMBOBOXLISTVIEW_H

#include <QListView>


class CustomComboBoxListView : public QListView
{
    Q_OBJECT
public:
    CustomComboBoxListView(QWidget *a_parent);

signals:
    void itemSelected(int a_index);
};

#endif // CUSTOMCOMBOBOXLISTVIEW_H
