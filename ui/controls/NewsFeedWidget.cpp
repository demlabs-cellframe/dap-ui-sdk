#include "NewsFeedWidget.h"

NewsFeedWidget::NewsFeedWidget(QString a_text, QString a_url, int a_speed, QWidget *parent)
        :QLabel(parent),
          m_url(a_url)
{
    m_layout = new QHBoxLayout(this);
    m_lblText = new ClickableRunningLabel(a_text, this);
    m_lblText->setObjectName("lblTextNews");
    setSpeed(a_speed);

    m_lblClose = new ClickableLabel(this);
    m_lblClose->setObjectName("lblCloseNews");

    m_layout->addWidget(m_lblText,0);
    m_layout->addWidget(m_lblClose,1);

    connect(m_lblClose,&ClickableLabel::clicked,[this]()
    {
        this->close();
        emit this->newsClosed();
    });

    connect(m_lblText,&ClickableLabel::clicked,[this]()
    {
        if (!m_url.isEmpty()) QDesktopServices::openUrl(QUrl(m_url));
    });

    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);
}

void NewsFeedWidget::updateGeometry()
{
    this->setGeometry(0,qobject_cast<QWidget*>(this->parent())->height() - this->size().rheight(),this->width(),0);
    if (!m_spacerFirst && !m_spacerSecond)
    {
        m_spacerFirst = new QSpacerItem((this->width() - (m_lblText->width() + m_lblClose->width()))/2,0);
        m_spacerSecond = new QSpacerItem(*m_spacerFirst);
        m_layout->insertItem(0,m_spacerFirst);
        m_layout->insertItem(2,m_spacerSecond);
    }
}

void NewsFeedWidget::setText(const QString &a_text)
{
    m_lblText->ClickableRunningLabel::setText(a_text);
}

void NewsFeedWidget::setSpeed(const int a_speed)
{
    m_lblText->setSpeed(a_speed);
}

QString NewsFeedWidget::text() const
{
    return m_lblText->ClickableRunningLabel::text();
}

void NewsFeedWidget::resizeEvent(QResizeEvent *ev)
{
    updateGeometry();
    QLabel::resizeEvent(ev);
}
