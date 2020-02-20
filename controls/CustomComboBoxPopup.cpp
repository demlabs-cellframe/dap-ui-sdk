#include "CustomComboBoxPopup.h"

CustomComboBoxPopup::CustomComboBoxPopup(const QString &a_caption,ListModel *model, QWidget *parent)
    : QWidget (parent)
{
    setFixedSize(parent->width(),parent->height());
    setObjectName("wgtComboBoxPopup");

    if(model != nullptr)
    {
        m_model = model;
    }
    else
    {
        m_model = new ListModel(this);
    }

    m_lblCaption = new QLabel(this);
    m_lblCaption->setObjectName("lblCaption");
    m_lblCaption->setText(a_caption);
    m_lblCaption->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    m_lvwList = new QListView(this);
    m_lvwList->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_lvwList->setObjectName("lvwList");
    m_lvwList->setModel(m_model);

    for(int i = 0; i < m_model->getData().size();i++)
    {
        m_lvwList->setIndexWidget(m_model->index(i),new CustomWidget("", "", m_lvwList));
    }

    m_layout = new QVBoxLayout;
    m_layout->setSpacing(0);
    m_layout->setMargin(0);
    m_layout->addWidget(m_lblCaption);
    m_layout->addWidget(m_lvwList);
    m_layout->setAlignment(m_lblCaption,Qt::AlignHCenter);
    m_layout->setAlignment(m_lvwList,Qt::AlignHCenter);
    this->setLayout(m_layout);
    connect(m_lvwList,&QListView::clicked,[=]{
        setVisible(false);
    });

}
