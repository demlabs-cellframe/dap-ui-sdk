#include "CustomComboBoxPopup.h"


CustomComboBoxPopup::CustomComboBoxPopup(QWidget *parent)
    : QWidget (parent),
      m_layout(new QVBoxLayout(this)),
      m_layoutScroll(new QVBoxLayout(this)),
      m_lblCaption(new QLabel(this)),
      m_scaList(new QScrollArea(this))
{
    setFixedSize(parent->width(),parent->height());
    this->setObjectName("wgtComboBoxPopup");
    m_scaList->setObjectName("scaList");

    m_scaList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scaList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_lblCaption->setObjectName("lblCaption");

    m_lblCaption->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    m_layout->setSpacing(0);
    m_layout->setMargin(0);
    m_layout->addWidget(m_lblCaption);
    m_layout->addWidget(m_scaList);
    m_layout->setAlignment(m_lblCaption,Qt::AlignHCenter);
    m_layout->setAlignment(m_scaList,Qt::AlignHCenter);

    this->setLayout(m_layout);

    m_layoutScroll->setSpacing(0);
    m_layoutScroll->setMargin(0);

    m_scaList->setLayout(m_layoutScroll);

}

void CustomComboBoxPopup::setModel(QList<DataModel> *model)
{
    for(int i=0; i<model->size();i++)
    {
       // CustomPlacementButton_New tmp;
        m_listButton.append(new CustomPlacementButton_New());
        m_listButton.last()->setIcon(model->at(i).iconPath);
        m_listButton.last()->setText(model->at(i).text);
        m_listButton.last()->setObjectName("btnServer");
        m_listButton.last()->addSubcontrol("arrow");

        m_layoutScroll->addWidget(m_listButton.last());
        m_layoutScroll->setAlignment(m_listButton.last(),Qt::AlignHCenter);
        QWidget *tmpWidget = new QWidget();
        if(i<model->size()-1)
        {
            tmpWidget->setProperty("state","sizeFixed"); //This setting is for fixing gaps between widgets other than the last.
        }

        m_layoutScroll->addWidget(tmpWidget);

    }
}

void CustomComboBoxPopup::setModel(QAbstractItemModel *a_model)
{
    for(int i=0; i<a_model->rowCount(); i++)
    {
       // CustomPlacementButton_New tmp;
        CustomPlacementButton_New *newButton = new CustomPlacementButton_New;
        m_listButton.append(newButton);
//        newButton->setIcon(a_model->at(i).iconPath);
        newButton->setText(a_model->data(a_model->index(i, 0)).toString());
        newButton->setObjectName("btnServer");
        newButton->addSubcontrol("arrow");

        m_layoutScroll->addWidget(newButton);
        m_layoutScroll->setAlignment(newButton, Qt::AlignHCenter);
        QWidget *tmpWidget = new QWidget();
        if(i < a_model->rowCount() - 1)
        {
            tmpWidget->setProperty("state", "sizeFixed"); //This setting is for fixing gaps between widgets other than the last.
        }

        m_layoutScroll->addWidget(tmpWidget);

        connect(newButton, &QPushButton::clicked, [this, i]{
            emit this->itemSelected(i);
            this->hide();
        });

    }
}


void CustomComboBoxPopup::setCaption(const QString &text)
{
    m_lblCaption->setText(text);
}
