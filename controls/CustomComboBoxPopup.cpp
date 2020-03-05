#include "CustomComboBoxPopup.h"

#include <QListView>
#include <QDebug>
#include "CustomComboBox.h"
#include "WidgetDelegateListView.h"

CustomComboBoxPopup::CustomComboBoxPopup(QWidget *parent)
    : AdaptiveWidget(parent)
{
    this->setObjectName("stwCustomComboBoxPopup");

    this->hide();

    connect(this, &CustomComboBoxPopup::itemSelected, [this] (int index){
        this->hide();
    });
}

void CustomComboBoxPopup::setModel(QAbstractItemModel *a_model)
{
    for(QListView* curView : this->allListViews())
        curView->setModel(a_model);
}

void CustomComboBoxPopup::setCurrentIndex(int a_index)
{
    for(QListView* curView : this->allListViews())
        curView->setCurrentIndex(curView->model()->index(a_index, 0));
}

void CustomComboBoxPopup::initVariantUi(QWidget *a_widget)
{
    WidgetDelegateListView *lvwList = a_widget->findChild<WidgetDelegateListView*>(this->listViewName()); Q_ASSERT(lvwList);

//    connect(lvwList, &QAbstractItemView::activated, [this](const QModelIndex &index)
//    {
//        qDebug() << "activated";
//        emit this->itemSelected(index.row());
//    });
//    connect(lvwList, &QAbstractItemView::pressed, [this](const QModelIndex &index)
//    {
//        qDebug() << "pressed";
//        emit this->itemSelected(index.row());
//    });
    connect(lvwList, &WidgetDelegateListView::itemSelected, [this](int a_index){
        emit itemSelected(a_index);
    });

  //  a_widget->setStyleSheet("min-width:300; min-height:300");
    //this->setStyleSheet("QStackedWidget{min-width:300; min-height:300;}");
}

//void CustomComboBoxPopup::showEvent(QShowEvent *event)
//{
//    this->AdaptiveWidget::showEvent(event);
//    for (QListView* curList: allListViews())
//    {
//        curList->setFocus(setFocus();
//    }
//}

QList<QListView *> CustomComboBoxPopup::allListViews()
{
    return this->getTheSameWidgets<QListView>(this->listViewName());
}
