#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QStateMachine>
#include <QSignalTransition>
#include <QResizeEvent>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QGraphicsEffect>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QRgb>

#include "DapUiScreen.h"

#include "controls/ScreenOverlaying.h"

#ifdef DAP_PLATFORM_DESKTOP
    #include "ui_sidebar_desktop.h"
#else
    // TODO: include header files for mobile version forms.
#endif


class ScreenOverlaying; class ResizeFilter;

namespace Ui {
class SideBar;
}

class SideBar : public DapUiScreen
{
    Q_OBJECT
public:

    explicit SideBar(QWidget *parent);
    ~SideBar();

    QWidget* getWidget(const QString& a_objName);

    // Getters:
    QWidget* widget(){return sw();}
    ScreenOverlaying* screenOverlaying(){return m_screenOverlaying;}
    QStateMachine * stateMachine(){return m_sm;}
    QState * stateShowed(){return m_stateShowed;}
    QState * stateHidden(){return m_stateHidden;}

    int width(){return sw()->width();}

    void start(){m_sm->start();}

    void resize(int a_height, int a_width);

//    QState* stateHidden(){return m_stateHidden;}

public slots:

    void changeVisible();


signals:

    void sgnlHide();
    void sgnlShow();
    void sgnlResize(int a_width, int a_height);

protected:
    //StateMacine:
    QStateMachine *m_sm;
        //States:
        QState *m_stateShowed;
        QState *m_stateHidden;

    virtual void initUi(QWidget * a_w,ScreenRotation a_rotation);
private:

    void setupStatesPositions();

    Ui::SideBar *m_ui;

    ScreenOverlaying *m_screenOverlaying;


    //Transitions:
    QSignalTransition *m_hidingTransition;
    QSignalTransition *m_showingTransition;

    ResizeFilter* m_resizeFilter;
};


//class wich catch parent's widget resizeEvent and resize SideBar
class ResizeFilter: public QObject
{
Q_OBJECT
public:
    explicit ResizeFilter(SideBar *a_sideBar):QObject (a_sideBar){}
    virtual bool eventFilter(QObject* a_obj, QEvent* a_event)
    {
        Q_UNUSED(a_obj);
        // if event type = resize:
        if (a_event->type() == QEvent::Resize){

            QResizeEvent *resizeEvent = static_cast<QResizeEvent*>(a_event);

            //resize:
            qobject_cast<SideBar*>(parent())->resize(resizeEvent->size().width(), resizeEvent->size().height());
        }
        return false;
    }
};

#endif // SIDEBAR_H
