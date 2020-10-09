#include "ClickableRunningLabel.h"

ClickableRunningLabel::ClickableRunningLabel(QString a_text, QWidget* a_parent)
    :ClickableLabel(a_parent),
      m_text(a_text)
{
    QLabel::setText(a_text);
    setAlignment(Qt::AlignHCenter);
}

void ClickableRunningLabel::setText(const QString &a_text)
{
    m_text = a_text;
    this->QLabel::setText(a_text);
    verifyWidth();
}

void ClickableRunningLabel::setSpeed(const int a_speed)
{
    if (a_speed <= 0) m_speed = 1000;
    else if (a_speed) m_speed = a_speed;
}

void ClickableRunningLabel::verifyWidth()
{
    QFontMetrics fm(this->font());
    m_widthText = fm.width(m_text);
    if (m_widthText > this->width()) startAnimation();
    else stopAnimation();
}

void ClickableRunningLabel::startAnimation()
{
    if(!m_layout)
    {
        m_layout = new QHBoxLayout(this);

        m_lblFirst = new QLabel(this->text(),this);
        m_lblSecond = new QLabel(this->text(),this);

        m_layout->addWidget(m_lblFirst);
        m_layout->addWidget(m_lblSecond);
        m_layout->setContentsMargins(0,0,0,0);
        m_layout->setSpacing(0);
        this->setLayout(m_layout);

        QLabel::setText("");

        int labelWidth = m_widthText + 50;
        m_animFirst = new QPropertyAnimation(m_lblFirst, "geometry");
        m_animFirst->setLoopCount(-1);
        m_animFirst->setStartValue(QRect(m_lblFirst->x(),m_lblFirst->y(),labelWidth,m_lblFirst->height()));
        m_animFirst->setEndValue(QRect(m_lblFirst->x() - labelWidth,m_lblFirst->y(),labelWidth,m_lblFirst->height()));
        m_animFirst->setDuration(m_speed);

        m_animSecond = new QPropertyAnimation(m_lblSecond, "geometry");
        m_animSecond->setLoopCount(-1);
        m_animSecond->setStartValue(QRect(m_lblFirst->x() + labelWidth,m_lblSecond->y(),labelWidth,m_lblSecond->height()));
        m_animSecond->setEndValue(QRect(m_lblFirst->x(),m_lblFirst->y(),labelWidth,m_lblSecond->height()));
        m_animSecond->setDuration(m_speed);

        m_animFirst->start(); m_animSecond->start();
    }
}

void ClickableRunningLabel::stopAnimation()
{
    if (m_layout)
    {
        this->QLabel::setText(m_lblFirst->text());
        m_animFirst->stop();
        m_animSecond->stop();
        delete m_animSecond;
        delete m_animFirst;
        delete m_lblSecond;
        delete m_lblFirst;
        delete m_layout;
    }
}

void ClickableRunningLabel::showEvent(QShowEvent *e)
{
    verifyWidth();
    QLabel::showEvent(e);
}

void ClickableRunningLabel::closeEvent(QCloseEvent *e)
{
    stopAnimation();
    QLabel::closeEvent(e);
}

void ClickableRunningLabel::hideEvent(QHideEvent *e)
{
    stopAnimation();
    QLabel::hideEvent(e);
}

