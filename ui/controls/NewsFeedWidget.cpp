#include "NewsFeedWidget.h"

NewsFeedWidget::NewsFeedWidget(QString a_text, QString a_url, int a_speed, QWidget *parent)
    :QLabel(parent)
{
    m_parent=parent;

    m_layout = new QHBoxLayout(this);
    m_lblText = new RunLineLabel(a_text, a_url,this);
    setSpeed(a_speed);

    m_lblClose = new ClickableLabel(this);
    m_lblClose->setObjectName("lblCloseNews");

    m_layout->addWidget(m_lblText,0);
    m_layout->addWidget(m_lblClose,1);

    connect(m_lblClose,&ClickableLabel::clicked,[this]()
    {
        this->hide();
    });
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);
}

void NewsFeedWidget::updateGeometry()
{
    this->setGeometry(0,m_parent->height()-this->size().rheight(),m_parent->width(),0);
}

void NewsFeedWidget::setUrl(const QString &a_url)
{
    m_lblText->setUrl(a_url);
}

void NewsFeedWidget::setText(const QString &a_text)
{
    m_lblText->setText(a_text);
}

void NewsFeedWidget::setSpeed(const int a_speed)
{
    m_lblText->setSpeed(a_speed);
}

void NewsFeedWidget::resizeEvent(QResizeEvent *ev)
{
    updateGeometry();
    QLabel::resizeEvent(ev);
}


//*****************RunLineLabel*****************
RunLineLabel::RunLineLabel(QString a_text, QString a_url, QWidget *parent)
    :ClickableLabel(parent)
{
    RunLineLabel::setText(a_text);
    setUrl(a_url);
    setObjectName("lblTextNews");
    connect(this,&ClickableLabel::clicked,[this]()
    {
        QDesktopServices::openUrl(QUrl(m_url));
    });
}

void RunLineLabel::setUrl(const QString &a_url)
{
    m_url=a_url;
}

void RunLineLabel::setText(const QString &a_text)
{
    QLabel::setText(a_text);
    m_text=a_text;
    m_shift=0;
}

void RunLineLabel::setSpeed(const int a_speed)
{
    if (m_timerId)
      killTimer(m_timerId);
    m_timerId = 0;

    if (a_speed < 0) return;

    if (a_speed) m_timerId = startTimer(1000/a_speed);
}

void RunLineLabel::timerEvent(QTimerEvent *)
{
    const int length = m_text.length();
    if(++m_shift >= length) m_shift = 0;
    QLabel::setText(m_text.right(length-m_shift) + "   " + m_text.left(m_shift-3));
}
