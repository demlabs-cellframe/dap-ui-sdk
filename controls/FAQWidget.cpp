#include "FAQWidget.h"

FAQWidget::FAQWidget(const FAQDataModel &model, QWidget *parent): QWidget (parent)
{

    setObjectName("wgtFAQWidget");

    m_lblIcon = new QLabel(this);
    m_lblHeader = new CustomLineHeightLabel(this);
    m_lblText = new CustomLineHeightLabel(this);
    m_lblBottomLine = new QLabel(this);

    m_topLayout = new QHBoxLayout();
    m_mainLayout = new QVBoxLayout();

    m_lblIcon->setObjectName("lblIcon");
    m_lblHeader->setObjectName("lblHeaderText");
    m_lblText->setObjectName("lblText");
    m_lblBottomLine->setObjectName("lblBottomLine");

    m_lblHeader->setText(model.text);
    m_lblHeader->setWordWrap(true);
    m_lblText->setText(model.aboutText);
    m_lblText->setWordWrap(true);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_lblIcon->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_lblHeader->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    m_lblText->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);

    m_topLayout->addWidget(m_lblHeader);
    m_topLayout->addWidget(m_lblIcon);
    m_mainLayout->addLayout(m_topLayout);
    m_mainLayout->addWidget(m_lblText);
    m_mainLayout->addWidget(m_lblBottomLine);
    setLayout(m_mainLayout);

    m_topLayout->setSpacing(0);
    m_topLayout->setMargin(0);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setMargin(0);

    m_topLayout->setAlignment(m_lblHeader,Qt::AlignTop);
    m_topLayout->setAlignment(m_lblIcon,Qt::AlignTop);
    m_mainLayout->setAlignment(m_lblText,Qt::AlignLeft);

    m_lblText->setVisible(false);
    show();
}

void FAQWidget::mousePressEvent(QMouseEvent *event)
{       
    if(!m_lblText->isVisible())
    {
        m_lblIcon->setProperty("state","arow-down");
        m_lblText->setVisible(true);
        style()->unpolish(m_lblIcon);
        style()->polish(m_lblIcon);
        update();
    }
    else
    {
        m_lblIcon->setProperty("state","default");
        m_lblText->setVisible(false);
        style()->unpolish(m_lblIcon);
        style()->polish(m_lblIcon);
        update();
    }

    QWidget::mousePressEvent(event);
}

