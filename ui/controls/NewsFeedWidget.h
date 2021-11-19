#ifndef NEWSFEEDWIDGET_H
#define NEWSFEEDWIDGET_H
#include "ClickableLabel.h"
#include "ClickableRunningLabel.h"
#include <QHBoxLayout>
#include <QDesktopServices>

class NewsFeedWidget:public QLabel
{
    Q_OBJECT
public:
    NewsFeedWidget(QString a_text, QString a_url, int a_speed, QWidget *parent);
    void updateGeometry();
    void setUrl(const QString &a_url){m_url = a_url;}
    void setText(const QString &a_text);
    void setSpeed(const int a_speed);
    QString text()const;
protected:
    virtual void resizeEvent(QResizeEvent* ev);
private:
    QHBoxLayout*            m_layout{Q_NULLPTR};
    ClickableLabel*         m_lblClose{Q_NULLPTR};
    ClickableRunningLabel*  m_lblText{Q_NULLPTR};
    QSpacerItem*            m_spacerFirst{Q_NULLPTR};
    QSpacerItem*            m_spacerSecond{Q_NULLPTR};
    QString                 m_url{};
};
#endif // NEWSFEEDWIDGET_H
