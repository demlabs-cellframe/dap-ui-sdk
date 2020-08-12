#include "CustomComboBoxPopup.h"

#include "CustomComboBoxListView.h"
#include "defines.h"


CustomComboBoxPopup::CustomComboBoxPopup(QMainWindow *parent)
    : CustomPopup(parent)
{
    this->setObjectName("stwCustomComboBoxPopup");

    this->setWindowType(Qt::ToolTip);
//    this->setSizePolicy(QSizePolicy::Preferred, this->sizePolicy().horizontalPolicy());

    connect(this, static_cast<void(CustomComboBoxPopup::*)(int)>(&CustomComboBoxPopup::activated), [this] (int index){
        Q_UNUSED(index)

        this->hide();
    });
}

QAbstractItemModel *CustomComboBoxPopup::model()
{
    return m_model;
}

void CustomComboBoxPopup::setModel(QAbstractItemModel *a_model)
{
    for(QListView* curView : this->allListViews())
        curView->setModel(a_model);
}

void CustomComboBoxPopup::addItem(const QString &a_text, const QVariant &a_userData /*= QVariant()*/)
{
    QAbstractItemModel* model = this->model();
    int row = model->rowCount();

    if (!model->insertRow(row))
        return;

    QModelIndex index = model->index(row, 0);

    model->setData(index, a_text, Qt::DisplayRole);
    if (a_userData.isValid())
        model->setData(index, a_userData, Qt::UserRole);
}


void CustomComboBoxPopup::setCaptionVisible(bool a_visible)
{
    if (m_captionIsVisible == a_visible)
        return;
    m_captionIsVisible = a_visible;

    QLabel *lblCaption = findChild<QLabel*>(this->captionLabelName());
    
    if (lblCaption)
    	lblCaption->hide();
}

bool CustomComboBoxPopup::captionIsVisible() const
{
    return m_captionIsVisible;
}

void CustomComboBoxPopup::setCaption(const QString &a_caption)
{
    if (m_caption == a_caption)
        return;
    m_caption = a_caption;

    this->setChildProperties(this->captionLabelName(), Properties::TEXT, a_caption);
}

QString CustomComboBoxPopup::caption() const
{
    return m_caption;
}

void CustomComboBoxPopup::setCurrentIndex(int a_index)
{
    for(QListView* curView : this->allListViews())
        curView->setCurrentIndex(curView->model()->index(a_index, 0));
}

void CustomComboBoxPopup::initVariantUi(QWidget *a_widget)
{
    CustomComboBoxListView *lvwList; Utils::findChild(a_widget, this->listViewName(), lvwList);

#ifdef Q_OS_MAC
    lvwList->setAttribute(Qt::WA_MacShowFocusRect,false);
#endif

    if (m_model)
        lvwList->setModel(m_model);
    else
        m_model=lvwList->model();

    connect(lvwList, &CustomComboBoxListView::itemSelected, [this, lvwList](int a_index){
        emit this->activated(a_index);
        emit this->activated(lvwList->model()->index(a_index, 0).data().toString());
    });
}

QString CustomComboBoxPopup::captionLabelName()
{
    return "";
}

QList<QListView *> CustomComboBoxPopup::allListViews()
{
    return this->getTheSameWidgets<QListView>(this->listViewName());
}
