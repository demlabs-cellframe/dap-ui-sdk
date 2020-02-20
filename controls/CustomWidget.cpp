#include "CustomWidget.h"
#include <QDebug>
#include <QPixmap>
#include <QStyle>
#include <QModelIndex>

CustomWidget::CustomWidget(QWidget *parent)
    : QWidget (parent)
{

}

CustomWidget::CustomWidget(QString iconPath/* = ""*/, QString text/* = ""*/, QWidget *parent/* = nullptr*/)
    : QWidget (parent)
{
//    m_index = index;
//    m_data = &model;
//    m_listView = listView;

    setObjectName("wgtDelegate");
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    m_lblFlag = new QLabel(this);
    m_lblFlag->setObjectName("lblFlag");
    m_lblFlag->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    QPixmap pixmap(m_data->iconPath);
    m_lblFlag->setPixmap(pixmap);

    m_lblText = new QLabel(this);
    m_lblText->setObjectName("lblText");
    m_lblText->setText(m_data->text);
    m_lblText->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    m_lblCheckIcon = new QLabel(this);
    m_lblCheckIcon->setObjectName("lblCheckIcon");

    m_lblCheckIcon->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    m_mainLayout = new QHBoxLayout();

    m_mainLayout->setSpacing(0);
    m_mainLayout->setMargin(0);

    m_mainLayout->addWidget(m_lblFlag);
    m_mainLayout->addWidget(m_lblText);
    m_mainLayout->addWidget(m_lblCheckIcon);
    this->setLayout(m_mainLayout);

    m_styledshadow = new StyledDropShadowEffect(this);

    show();
}

void CustomWidget::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    setStyle(1);
}

void CustomWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    if(m_listView->currentIndex().row()!=m_index)
        setStyle(0);
}

void CustomWidget::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event)
    if(m_listView->currentIndex().row()==m_index)
    {
        setStyle(1);
    }
    else
        setStyle(0);
}

void CustomWidget::setStyle(int state)
{
    setProperty("state",state);
    style()->unpolish(this);
    style()->polish(this);
    m_lblCheckIcon->setProperty("state",state);
    style()->unpolish(m_lblCheckIcon);
    style()->polish(m_lblCheckIcon);
    m_lblText->setProperty("state",state);
    style()->unpolish(m_lblText);
    style()->polish(m_lblText);
    update();
    if(!state)
    {
        m_styledshadow->updateStyle(DEFAULT_SHADOW);
        setGraphicsEffect(m_styledshadow);
    }
    else
    {
        m_styledshadow->updateStyle(HOVER_SHADOW);
        setGraphicsEffect(m_styledshadow);
    }
}

bool CustomWidget::selected() const
{
    return m_selected;
}

void CustomWidget::setSelected(bool selected)
{
    m_selected = selected;
}
