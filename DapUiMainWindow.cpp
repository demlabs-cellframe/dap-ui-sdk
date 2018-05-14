#include <QtDebug>
#include <QStackedWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QLayout>
#include "DapUiMainWindow.h"
#include "DapUiMenuBar.h"
#include "Manager.h"

/**
 * @brief DapUiMainWindow::DapUiMainWindow
 * @param parent
 */
DapUiMainWindow * DapUiMainWindow::m_instance=nullptr;
//это базовый же вроде?
DapUiMainWindow::DapUiMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(new QVBoxLayout());
    centralWidget()->layout()->setContentsMargins(0,0,0,0);
    m_sw = new QStackedWidget(centralWidget());
    centralWidget()->layout()->addWidget(sw());

    m_instance = this;
    m_currentScreen = nullptr;
    m_menuBar = nullptr;
    QGuiApplication::primaryScreen()->setOrientationUpdateMask(
                Qt::LandscapeOrientation | Qt::PortraitOrientation |
                Qt::InvertedLandscapeOrientation | Qt::InvertedLandscapeOrientation);
    connect(QGuiApplication::primaryScreen(),&QScreen::orientationChanged
            ,this, &DapUiMainWindow::onScreenOrientationChanged);

    onScreenOrientationChanged(QGuiApplication::primaryScreen()->orientation());

}

/**
 * @brief DapUiMainWindow::~DapUiMainWindow
 */
DapUiMainWindow::~DapUiMainWindow()
{
   if(m_currentScreen)
       delete m_currentScreen;
}

void DapUiMainWindow::initMenuBarBackground()
{
    QWidget * l_w = new QWidget(this);
    l_w->setStyleSheet("background: #20DCA3;");
    double l_k = this->size().width()/360.0;
    l_w->setMinimumHeight(58*l_k);
    l_w->setMaximumHeight(58*l_k);
    l_w->setVisible(false);
    centralWidget()->layout()->addWidget(l_w);
    connect(&Manager::me(), &Manager::sigMenuVisible, [=](bool f){l_w->setVisible(f);});
}

void DapUiMainWindow::initMenuBar( QBrush a_bgBrush, int a_barHeight)
{
    /*
    if(m_menuBar){
        qWarning() << "MenuBar is already created, delete that first";
        delete m_menuBar;
    }
    */
    auto m_menuBar = new DapUiMenuBar(this,a_bgBrush,a_barHeight);
    centralWidget()->layout()->addWidget(m_menuBar);
    centralWidget()->layout()->setSpacing(0);
    //menuBar()->hide();

    // Здесь должна инитаться подложка
}



/**
 * @brief DapUiMainWindow::onScreenOrientationChanged
 * @param a_orientation
 */
void DapUiMainWindow::onScreenOrientationChanged(Qt::ScreenOrientation a_orientation)
{
    if(currentScreen()){
        switch (a_orientation){
            case Qt::PortraitOrientation:
            case Qt::InvertedPortraitOrientation:
                currentScreen()->setCurrentVariant(DapUiScreen::VariantVer);
            break;
            case Qt::LandscapeOrientation:
            case Qt::InvertedLandscapeOrientation:
                currentScreen()->setCurrentVariant(DapUiScreen::VariantHor);
            break;
            default:
                currentScreen()->setCurrentVariantDefault();
        }
    }
}

/**
 * @brief DapUiMainWindow::newScreen
 * @param a_screen
 */
void DapUiMainWindow::newScreen(DapUiScreen * a_screen)
{
    deleteCurrentScreen();
    m_currentScreen = a_screen;
    m_currentScreen->show();
    emit sigScreenChanged();
}
