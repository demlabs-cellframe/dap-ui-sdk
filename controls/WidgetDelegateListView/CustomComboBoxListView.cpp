#include <QDebug>
#include "CustomComboBoxListView.h"
#include "CustomPlacementButton.h"

CustomComboBoxListView::CustomComboBoxListView(QWidget *a_parent)
    :QListView(a_parent)
{
    connect(this, &QListView::clicked, [this](const QModelIndex &a_index){
        qDebug() << "clicked";
        emit this->itemSelected(a_index.row());
    });

}

