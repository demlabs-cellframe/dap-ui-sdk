#ifndef NEWSFEEDWIDGET_H
#define NEWSFEEDWIDGET_H
#include "ClickableLabel.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QDesktopServices>
#include <QFontMetrics>
#include <QTimer>
#include "Utilz.h"
#include "UiScaling.h"

class RunLineLabel;
class AnimationLabel;
class NewsFeedWidget:public QLabel
{
    Q_OBJECT
public:
    NewsFeedWidget(QString a_text, QString a_url, int a_speed, QWidget *parent);
    void updateGeometry();
    void setUrl(const QString &a_url);
    void setText(const QString &a_text);
    void setSpeed(const int a_speed);
    QString text()const;
protected:
    virtual void resizeEvent(QResizeEvent* ev);
private:
    QHBoxLayout*        m_layout{Q_NULLPTR};
    ClickableLabel*     m_lblClose{Q_NULLPTR};
    RunLineLabel*       m_lblText{Q_NULLPTR};
    QWidget*            m_parent{Q_NULLPTR};
    QSpacerItem*        m_spacerFirst{Q_NULLPTR};
    QSpacerItem*        m_spacerSecond{Q_NULLPTR};
};

class RunLineLabel:public ClickableLabel
{
    Q_OBJECT
public:
    RunLineLabel(QString a_text, QString a_url, QWidget *a_parent);
    void setUrl(const QString &a_url) {m_url = a_url;}
    void setText(const QString &a_text);
    void setSpeed(const int a_speed);
    QString text()const {return m_text;}
signals:
    void startAnimation();
    void stopAnimation();
protected:
    virtual void showEvent(QShowEvent* e);
    virtual void closeEvent(QCloseEvent* e);
    virtual void hideEvent(QHideEvent* e);
private slots:
    void timeoutAnimation();
private:
    void verifyWidth();
    QString     m_text{};
    QString     m_url{};
    QWidget*    m_parent{Q_NULLPTR};
    int         m_speed{};
    int         m_widthText{};

            /*if animation start*/
    QHBoxLayout*            m_layout{Q_NULLPTR};
    QLabel*                 m_lblFirst{Q_NULLPTR};
    QLabel*                 m_lblSecond{Q_NULLPTR};
    QTimer*                 m_timer{Q_NULLPTR};

};

#endif // NEWSFEEDWIDGET_H
