#ifndef NEWSFEEDWIDGET_H
#define NEWSFEEDWIDGET_H
#include "ClickableLabel.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QDesktopServices>
#include <QFontMetrics>
#include <QSizePolicy>
#include "Utilz.h"

class RunLineLabel;
class NewsFeedWidget:public QLabel
{
    Q_OBJECT
public:
    NewsFeedWidget(QString a_text, QString a_url, int a_speed, QWidget *parent);
    void updateGeometry();
    void setUrl(const QString &a_url);
    void setText(const QString &a_text);
    void setSpeed(const int a_speed);
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
/*signals:
    void newWidth(int);*/
protected:
    virtual void timerEvent(QTimerEvent*);
    virtual void resizeEvent(QResizeEvent* ev);
private:
    QString m_url{};
    QString m_text{};
    int m_shift{};
    int m_timerId{};
    int m_widthText{};
    QWidget* m_parent{Q_NULLPTR};
    bool running{}, condition{};
};


#endif // NEWSFEEDWIDGET_H
