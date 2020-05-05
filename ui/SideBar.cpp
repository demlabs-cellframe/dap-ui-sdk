#include "SideBar.h"



SideBar::SideBar(QWidget *parent) :
    DapUiScreen (parent, new QStackedWidget(parent)),
    m_sm(new QStateMachine(this)),
    m_stateShowed (new QState(m_sm)),
    m_stateHidden (new QState(m_sm)),
    m_ui(new Ui::SideBar),
    m_screenOverlaying(new ScreenOverlaying(parent)),
    m_resizeFilter(new ResizeFilter(this))
{
    qDebug() << "SideBar constructor";
    create<Ui::SideBar>();
    
    //currentPage()->show();
    sw()->resize(currentPage()->size());
    sw()->raise();
    sw()->setObjectName("SideBarStackedWidget");

    sw()->findChild<QLabel*>("lblVersion")->setText("Version " + QString(DAP_VERSION) + " (Beta)");

    //widget that fill screen under menu:
    m_screenOverlaying->setObjectName(QStringLiteral("screenOverlayimg"));

   //connect screenOverlaying click with hiding menu (changeVisible):
    connect(m_screenOverlaying, SIGNAL(mousePressed()), this, SLOT(changeVisible()));

    //positions of SideBar vidget in states (Hidden/showed)
    setupStatesPositions();

    //Opacity of screen:
    QGraphicsOpacityEffect* overlayingOpacity = new QGraphicsOpacityEffect(m_screenOverlaying);
    m_screenOverlaying->setGraphicsEffect(overlayingOpacity);


    //**********************************  Animation ***********************************
    //setup States:
    m_stateHidden->assignProperty(m_screenOverlaying, "visible", false);
    m_stateHidden->assignProperty(overlayingOpacity, "opacity", 0.0);

    m_stateShowed->assignProperty(m_screenOverlaying, "visible", true);
    m_stateShowed->assignProperty(overlayingOpacity, "opacity", 1.0);

    m_sm->setInitialState(m_stateHidden);

    //Adding transitions (hiding/showing)
    m_hidingTransition  = m_stateShowed->addTransition(this, SIGNAL(sgnlHide()), m_stateHidden);
    m_showingTransition = m_stateHidden->addTransition(this, SIGNAL(sgnlShow()), m_stateShowed);

    //Setup transition animations:
    //== Showing ==

    QSequentialAnimationGroup* anmGrp = new QSequentialAnimationGroup(this);

    QPropertyAnimation *anmWidgetPos = new QPropertyAnimation(sw(), "pos");
    QPropertyAnimation *anmOverlayingVisible = new QPropertyAnimation(m_screenOverlaying, "visible");
    QPropertyAnimation *anmOverlayingOpacity = new QPropertyAnimation(overlayingOpacity, "opacity");

    anmWidgetPos->setDuration(100);
    anmWidgetPos->setEasingCurve(QEasingCurve::OutCirc);
    anmOverlayingOpacity->setDuration(100);
    anmOverlayingVisible->setDuration(0);

    anmGrp->addAnimation(anmOverlayingVisible);
    anmGrp->addAnimation(anmOverlayingOpacity);
    anmGrp->addAnimation(anmWidgetPos);

    m_showingTransition->addAnimation(anmGrp);

    //== Hiding ==

    anmGrp = new QSequentialAnimationGroup(this);

    anmWidgetPos = new QPropertyAnimation(sw(), "pos");
    anmOverlayingVisible = new QPropertyAnimation(m_screenOverlaying, "visible");
    anmOverlayingOpacity = new QPropertyAnimation(overlayingOpacity, "opacity");

    anmWidgetPos->setDuration(100);
    anmWidgetPos->setEasingCurve(QEasingCurve::OutCirc);
    anmOverlayingOpacity->setDuration(100);
    anmOverlayingVisible->setDuration(0);

    anmGrp->addAnimation(anmWidgetPos);
    anmGrp->addAnimation(anmOverlayingOpacity);
    anmGrp->addAnimation(anmOverlayingVisible);

    m_hidingTransition->addAnimation(anmGrp);

    m_sm->start();

}

SideBar::~SideBar()
{
    delete m_ui;
}


void SideBar::setupStatesPositions()
{
    m_stateHidden->assignProperty(sw(), "pos", QPoint(-(sw()->width()),0));
    m_stateShowed->assignProperty(sw(), "pos", QPoint(0,0));
}

QWidget *SideBar::getWidget(const QString &a_objName) {
    return DapUiScreen::getWidget(a_objName, rotation());
}

void SideBar::changeVisible()
{
    if (m_stateShowed->active()){
         //Enable clickable for screen overlaying:
        m_screenOverlaying->setAttribute(Qt::WA_TransparentForMouseEvents);

        emit sgnlHide();

        //remove resizefilter from parent widget
        parent()->removeEventFilter(m_resizeFilter);
    }
    else {

        //resize widget of SideBar
        QWidget* parentWidget;
        parentWidget = qobject_cast<QWidget*>(parent());
        resize(parentWidget->width(), parentWidget->height());

        //install resizefilter to parent widget
        parent()->installEventFilter(m_resizeFilter);

        emit sgnlShow();

        //Disable clickable for screen overlaying:
        m_screenOverlaying->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    }
}

void SideBar::initUi(QWidget *a_w, DapUiScreen::ScreenRotation a_rotation)
{
    Q_UNUSED(a_w); Q_UNUSED(a_rotation);
}

void SideBar::resize(int a_width, int a_height)
{
    sw()->resize(sw()->width(), a_height);
    m_screenOverlaying->resize(a_width, a_height);
    emit sgnlResize(a_width, a_height);
}




