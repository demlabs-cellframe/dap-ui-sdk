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
        m_lblText->close();
        this->close();
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
    m_lblText->RunLineLabel::setText(a_text);
}

void NewsFeedWidget::setSpeed(const int a_speed)
{
    m_lblText->setSpeed(a_speed);
}

QString NewsFeedWidget::text() const
{
    return m_lblText->RunLineLabel::text();
}

void NewsFeedWidget::resizeEvent(QResizeEvent *ev)
{
    updateGeometry();
    QLabel::resizeEvent(ev);
}


//*****************RunLineLabel*****************
RunLineLabel::RunLineLabel(QString a_text, QString a_url, QWidget* a_parent)
    :ClickableLabel(a_parent),
      m_text(a_text),
      m_parent(a_parent)
{
    QLabel::setText(a_text);

    setUrl(a_url);
    setObjectName("lblTextNews");
    m_timer = new QTimer(this);
    m_timer->setInterval(25);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeoutAnimation()));

    connect(this, &RunLineLabel::startAnimation, [this]()
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

            m_animFirst = new QPropertyAnimation(m_lblFirst, "geometry");
            m_animFirst->setEasingCurve(QEasingCurve::Linear);

            m_animSecond = new QPropertyAnimation(m_lblSecond, "geometry");
            m_animSecond->setEasingCurve(QEasingCurve::Linear);
        }
        running = true;
        if (!m_timer->isActive()) m_timer->start();
    });
    connect(this,&ClickableLabel::clicked,[this]()
    {
        if (!m_url.isEmpty()) QDesktopServices::openUrl(QUrl(m_url));
        qDebug()<<"****CLICK!****";
        emit startAnimation();
    });
    setAlignment(Qt::AlignHCenter);
}

void RunLineLabel::setUrl(const QString &a_url)
{
    m_url=a_url;
}

void RunLineLabel::setText(const QString &a_text)
{
    if (this->text().isEmpty()) m_widthText = 1;
    QLabel::setText(a_text);
    m_text = a_text;
    running = condition = false;
    if (m_widthText)
    {
        m_widthText = 0;
        updateGeometry();
        if (this->width() - 100 < m_widthText) emit startAnimation();
    }
}

void RunLineLabel::setSpeed(const int a_speed)
{
    /*if (m_timerId)
      killTimer(m_timerId);
    m_timerId = 0;*/

    if (a_speed < 0) return;

    if (a_speed)
    {
        //m_timerId = startTimer(100/a_speed);
        m_speed = 1000/a_speed;
    }
}

void RunLineLabel::updateGeometry()
{
    qDebug()<<"width = "<<this->width();
    qDebug()<<"font = "<<this->font();
    QFontMetrics fm(this->font());
    m_widthText = fm.width(this->text());
    qDebug()<<"text width = "<<m_widthText;
    if (m_widthText > this->width()) emit startAnimation();
    /*if(condition && m_widthText > this->width()) emit startAnimation();
    if (!m_widthText)
    {
        QFontMetrics fm(this->font());
        m_widthText = fm.width(m_text);
        condition = true;
    }*/
    //setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Expanding);
}

QString RunLineLabel::text() const
{
    if(!m_lblFirst) return this->QLabel::text();
    else return m_lblFirst->text();
}

void RunLineLabel::resizeEvent(QResizeEvent *e)
{
    QLabel::resizeEvent(e);
    updateGeometry();
}

void RunLineLabel::showEvent(QShowEvent *e)
{
    QLabel::showEvent(e);
    qDebug()<<"*****SHOW*****";
}

void RunLineLabel::closeEvent(QCloseEvent *e)
{
    qDebug()<<"STOP TIMER!";
    if(m_timer->isActive()) m_timer->stop();
    QLabel::closeEvent(e);
}

void RunLineLabel::timeoutAnimation()
{
    static bool first{};
    if (!first)
    {
        m_lblFirst->setGeometry(QRect(m_lblFirst->x(),m_lblFirst->y(),
                                      m_widthText+50,m_lblFirst->height()));
        //m_lblFirst->setFixedWidth(m_widthText+50);
        qDebug()<<m_lblFirst->geometry();

        m_lblSecond->setGeometry(QRect(m_widthText + m_lblFirst->x() + 50,
                                       m_lblSecond->y(),m_widthText+50,m_lblSecond->height()));
        //m_lblSecond->setFixedWidth(m_widthText+50);
        qDebug()<<m_lblSecond->geometry();
        first=true;

        //this->setFixedWidth(750);
    }

    else if (running && this->isVisible())
    {
        if(m_animFirst->state()==QPropertyAnimation::Running
                || m_animSecond->state()==QPropertyAnimation::Running) return;

        if(m_lblFirst->x() < -m_lblFirst->width())
        {
            qDebug()<<"FIRST **x** = "<<m_lblFirst->x()<<"**width** = "<<-m_lblFirst->width();
            m_lblFirst->setGeometry(QRect(m_lblSecond->x() + m_lblSecond->width(),m_lblFirst->y(),
                                          m_lblFirst->width(),m_lblFirst->height()));
        }

        if(m_lblSecond->x() < -m_lblSecond->width())
        {
            qDebug()<<"SECOND **x** = "<<m_lblSecond->x()<<"**width** = "<<-m_lblSecond->width();
            m_lblSecond->setGeometry(QRect(m_lblFirst->x() + m_lblFirst->width(),m_lblSecond->y(),
                                          m_lblSecond->width(),m_lblSecond->height()));
        }
        //qDebug()<<"m_timer"<<m_lblFirst->geometry()<<m_lblSecond->geometry();
        m_animFirst->setDuration(m_speed);
        m_animSecond->setDuration(m_speed);

        m_animFirst->setStartValue(QRect(m_lblFirst->x(),m_lblFirst->y(),
                                         m_lblFirst->width(),m_lblFirst->height()));
        m_animFirst->setEndValue(QRect(m_lblFirst->x() - 10,m_lblFirst->y(),
                                       m_lblFirst->width(),m_lblFirst->height()));

        m_animSecond->setStartValue(m_lblSecond->geometry());
        m_animSecond->setEndValue(QRect(m_lblSecond->x() - 10,m_lblSecond->y(),
                                        m_lblSecond->width(),m_lblSecond->height()));

        m_animFirst->start(); m_animSecond->start();
    }
}

//*****************AnimationLabel*****************
/*AnimationLabel::AnimationLabel(QWidget *a_parent)
    :QLabel(a_parent)
{

}

AnimationLabel::AnimationLabel(QString a_text, QWidget *a_parent)
    :QLabel(a_parent)
{
    this->setText(a_text);
}

void AnimationLabel::setWidth(int a_width)
{
    m_width=a_width;
}

int AnimationLabel::getWidth() const
{
    return m_width;
}

void AnimationLabel::resizeEvent(QResizeEvent *e)
{
    qDebug()<<"AnimationLabel::resizeEvent"<<this->geometry()<<this->objectName();
    if (this->objectName()=="lblFirst")
    {
        this->setGeometry(QRect(this->x(),this->y(),m_width,this->height()));
        this->setFixedWidth(m_width);
    }
    else if(this->objectName()=="lblSecond")
    {
        this->setGeometry(QRect(m_width,this->y(),m_width,this->height()));
        this->setFixedWidth(m_width);
    }
    qDebug()<<this->geometry();
    QLabel::resizeEvent(e);

}*/
