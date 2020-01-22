#include "MainMenu.h"

MainMenu::MainMenu(QWidget *parent):
    QObject (parent),
    m_sideBar(new SideBar(parent)),
    m_pages(new QStackedWidget(parent)),
    m_scrSettings(new ScreenSettings(m_pages))
{
    qDebug() << "MainMenu constructor";
    //if menuIcon = menu, on click - hide
    QPushButton* btMenu = sideBar()->getWidgetCustom<QPushButton>("btMenu", sideBar()->rotation());
    
    Q_ASSERT(btMenu);
    
    connect(btMenu, &QPushButton:: clicked,
            [=] {
                if (btMenu->isChecked())
                    sideBar()->changeVisible();
            });
    
    m_pages->setObjectName("mainMenuPages");

    QWidget *scrProfile = new QWidget(m_pages);
    m_pages->addWidget(scrProfile);

    m_pages->addWidget(m_scrSettings);

    //on sidebar resizing - resize menuPages
    connect(sideBar(), SIGNAL(sgnlResize(int, int)), SLOT(resizePage(int, int)));
    //placing menu pages near the sidebar
    m_pages->move(sideBar()->width(),0);


// =========================== States  ===========================

    QState *stateHidden = sideBar()->stateHidden();
    QState *stateShowed = sideBar()->stateShowed();

    QState *stateSidebar = new QState(stateShowed);
    QState *statePages   = new QState(stateShowed);

    QState *statePagesProfile  = new QState(statePages);
    QState *statePagesSettings = new QState(statePages);

    stateShowed->setInitialState(stateSidebar);


// ======================= States properties =======================

    stateHidden->assignProperty(m_pages, "visible",false);
    stateHidden->assignProperty(btMenu, "checked", false);

    stateSidebar->assignProperty(sideBar()->screenOverlaying(), "visible", true);
    stateSidebar->assignProperty(sideBar()->getWidget("btProfile"), "checked", false);
    stateSidebar->assignProperty(sideBar()->getWidget("btSettings"), "checked", false);
    stateSidebar->assignProperty(m_pages, "visible", false);
    stateSidebar->assignProperty(btMenu, "checked", false);

    statePages->assignProperty(m_pages, "visible", true);
    statePages->assignProperty(btMenu, "checked", true);

    statePagesProfile->assignProperty(m_pages, "currentIndex", Profile);
    statePagesProfile->assignProperty(sideBar()->getWidget("btProfile"), "checked", true);
    statePagesProfile->assignProperty(sideBar()->getWidget("btSettings"), "checked", false);

    statePagesSettings->assignProperty(m_pages, "currentIndex", Settings);
    statePagesSettings->assignProperty(sideBar()->getWidget("btProfile"), "checked", false);
    statePagesSettings->assignProperty(sideBar()->getWidget("btSettings"), "checked", true);

// ========================== Transitions ==========================

    stateSidebar->addTransition(sideBar()->getWidget("btProfile"), SIGNAL(clicked()), statePagesProfile);
    stateSidebar->addTransition(sideBar()->getWidget("btSettings"), SIGNAL(clicked()), statePagesSettings);

    statePages->addTransition(sideBar()->getWidget("btMenu"), SIGNAL(clicked()), stateSidebar);

    statePagesSettings->addTransition(sideBar()->getWidget("btProfile"), SIGNAL(clicked()), statePagesProfile);
    statePagesSettings->addTransition(sideBar()->getWidget("btSettings"), SIGNAL(clicked()), statePagesSettings);

    statePagesProfile->addTransition(sideBar()->getWidget("btSettings"), SIGNAL(clicked()), statePagesSettings);
    statePagesProfile->addTransition(sideBar()->getWidget("btProfile"), SIGNAL(clicked()), statePagesProfile);

//    //Hide dialogEditServer when statePagesSettings exited
//    connect(statePagesSettings, &QState::exited, [this] {
//        m_scrSettings->dialogEditServer()->hide();
//    });
}

void MainMenu::resizePage(int a_width, int a_height)
{
    m_pages->resize(a_width -sideBar()->width(), a_height);
}
