#ifndef CLICKABLERUNNINGLABEL_H
#define CLICKABLERUNNINGLABEL_H
#include "ClickableLabel.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>

class ClickableRunningLabel:public ClickableLabel
{
    Q_OBJECT
public:
    ClickableRunningLabel(QString a_text, QWidget *a_parent);
    void setText(const QString &a_text);
    void setSpeed(const int a_speed);
    QString text()const {return m_text;}

protected:
    virtual void showEvent(QShowEvent* e);
    virtual void closeEvent(QCloseEvent* e);
    virtual void hideEvent(QHideEvent* e);

private:
    void verifyWidth();
    void startAnimation();
    void stopAnimation();

    QString     m_text{};
    int         m_speed{};
    int         m_widthText{};

            /*if animation start*/
    QHBoxLayout*            m_layout{Q_NULLPTR};
    QLabel*                 m_lblFirst{Q_NULLPTR};
    QLabel*                 m_lblSecond{Q_NULLPTR};
    QPropertyAnimation*     m_animFirst{Q_NULLPTR};
    QPropertyAnimation*     m_animSecond{Q_NULLPTR};
};

#endif // CLICKABLERUNNINGLABEL_H
