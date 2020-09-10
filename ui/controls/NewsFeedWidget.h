#ifndef NEWSFEEDWIDGET_H
#define NEWSFEEDWIDGET_H
#include "ClickableLabel.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QDesktopServices>
#include <QFontMetrics>
#include <QSizePolicy>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QTimer>
#include "Utilz.h"

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
    QHBoxLayout* m_layout{Q_NULLPTR};
    ClickableLabel* m_lblClose{Q_NULLPTR};
    RunLineLabel* m_lblText{Q_NULLPTR};
    QWidget* m_parent{Q_NULLPTR};
};

class RunLineLabel:public ClickableLabel
{
    Q_OBJECT
public:
    RunLineLabel(QString a_text, QString a_url, QWidget *a_parent);
    void setUrl(const QString &a_url);
    void setText(const QString &a_text);
    void setSpeed(const int a_speed);
    void updateGeometry();
    QString text()const;
signals:
    void startAnimation();
protected:
    virtual void timerEvent(QTimerEvent*);
    virtual void resizeEvent(QResizeEvent* e);
    virtual void showEvent(QShowEvent* e);
    virtual void closeEvent(QCloseEvent* e);
private slots:
    void timeoutAnimation();
private:  
    QString m_url{}, m_text{};
    int m_speed{}, m_timerId{}, m_widthText{};
    QWidget* m_parent{Q_NULLPTR};
    bool running{}, condition{};

    /*if animation start*/
    QHBoxLayout* m_layout{Q_NULLPTR};
    QLabel *m_lblFirst{Q_NULLPTR}, *m_lblSecond{Q_NULLPTR};
    QPropertyAnimation *m_animFirst{Q_NULLPTR}, *m_animSecond{Q_NULLPTR};
    QTimer* m_timer{Q_NULLPTR};
};
/*class AnimationLabel:public QLabel
{
    Q_OBJECT
public:
    AnimationLabel(QWidget *a_parent);
    AnimationLabel(QString a_text, QWidget *a_parent);
    void setWidth(int a_width);
    int getWidth() const;
protected:
    virtual void resizeEvent(QResizeEvent* e);
private:
    int m_width{};
};*/

#endif // NEWSFEEDWIDGET_H
