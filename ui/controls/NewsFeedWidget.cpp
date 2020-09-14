#include "NewsFeedWidget.h"

NewsFeedWidget::NewsFeedWidget(QString a_text, QString a_url, int a_speed, QWidget *parent)
        :QLabel(parent),
        m_parent(parent)
{
    m_layout = new QHBoxLayout(this);
    m_lblText = new RunLineLabel(a_text, a_url,this);
    setSpeed(a_speed);

    m_lblClose = new ClickableLabel(this);
    m_lblClose->setObjectName("lblCloseNews");

    m_layout->addWidget(m_lblText,0);
    m_layout->addWidget(m_lblClose,1);

    connect(m_lblClose,&ClickableLabel::clicked,[this]()
    {
        this->close();
    });
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);
}

void NewsFeedWidget::updateGeometry()
{
    this->setGeometry(0,m_parent->height()-this->size().rheight(),this->width(),0);
    if (!m_spacerFirst && !m_spacerSecond)
    {
        m_spacerFirst = new QSpacerItem((this->width() - (m_lblText->width() + m_lblClose->width()))/2,0);
        m_spacerSecond = new QSpacerItem(*m_spacerFirst);
        m_layout->insertItem(0,m_spacerFirst);
        m_layout->insertItem(2,m_spacerSecond);
    }
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
      m_url(a_url),
      m_parent(a_parent)
{
    QLabel::setText(a_text);
    setObjectName("lblTextNews");
    m_timer = new QTimer(this);
    m_timer->setInterval(30);
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
        }
        if (!m_timer->isActive()) m_timer->start();
    });
    connect(this, &RunLineLabel::stopAnimation, [this]()
    {
        if (m_timer->isActive()) m_timer->stop();
        if (m_layout)
        {
            this->QLabel::setText(m_lblFirst->text());
            delete m_lblSecond;
            delete m_lblFirst;
            delete m_layout;
        }
    });

    connect(this,&ClickableLabel::clicked,[this]()
    {
        if (!m_url.isEmpty()) QDesktopServices::openUrl(QUrl(m_url));
    });
    setAlignment(Qt::AlignHCenter);
}

void RunLineLabel::setText(const QString &a_text)
{
    m_text = a_text;
    this->QLabel::setText(a_text);
    verifyWidth();
}

void RunLineLabel::setSpeed(const int a_speed)
{
    if (a_speed <= 0) m_speed = 2;
    else if (a_speed) m_speed = a_speed;
}

void RunLineLabel::verifyWidth()
{
    QFontMetrics fm(this->font());
    m_widthText = fm.width(m_text);
    if (m_widthText > this->width()) emit startAnimation();
    else emit stopAnimation();
}

void RunLineLabel::showEvent(QShowEvent *e)
{
    verifyWidth();
    QLabel::showEvent(e);
}

void RunLineLabel::closeEvent(QCloseEvent *e)
{
    emit stopAnimation();
    QLabel::closeEvent(e);
}

void RunLineLabel::hideEvent(QHideEvent *e)
{
    emit stopAnimation();
    QLabel::hideEvent(e);
}

void RunLineLabel::timeoutAnimation()
{
    static int widthLabel{m_widthText + 50};
    static int first_x{};
    static int second_x{first_x + widthLabel};
    if (first_x < -widthLabel) first_x = widthLabel + second_x;
    if (second_x < -widthLabel) second_x = widthLabel + first_x;
    m_lblFirst->setGeometry(QRect(first_x,m_lblFirst->y(),widthLabel,m_lblFirst->height()));
    m_lblSecond->setGeometry(QRect(second_x,m_lblSecond->y(),widthLabel,m_lblSecond->height()));
    first_x -= m_speed;
    second_x -= m_speed;
}
