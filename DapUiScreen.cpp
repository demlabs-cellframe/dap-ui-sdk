#include <QtDebug>
#include <QStackedWidget>
#include <QDesktopWidget>
#include <QApplication>
#include <QScreen>
#include "DapUiScreen.h"
#include "DapUiMainWindow.h"
#include "MainWindow.h"

/**
 * @brief DapUiScreen::DapUiScreen
 * @param parent
 * @param a_sw
 */
DapUiScreen::DapUiScreen(QObject *parent, QStackedWidget * a_sw)
    : QObject(parent),m_sw(a_sw)
{
    Q_ASSERT(m_sw);

// Set up default rotations, childs could change that by their own
    m_rotations.insert(Hor);
#ifdef DAP_PLATFORM_MOBILE
    m_rotations.insert(Ver);
#endif

}

DapUiScreen::~DapUiScreen()
{
//    for(auto l_page: m_page)
//        delete l_page;
}

QSize DapUiScreen::getWindowSizeInPoints(ScreenSize screenSize = DapUiScreen::ScreenSize::Medium)
{
    switch (screenSize) {
    case DapUiScreen::ScreenSize::Small:
        return QSize(640, 400);
    case DapUiScreen::ScreenSize::Medium:
        return QSize(1024, 768);
    case DapUiScreen::ScreenSize::Big:
        return QSize(1664, 1040);
    default:
        qWarning() << "Not supported Screen";
        throw "Not supported Screen";
    }
}

const char *DapUiScreen::PROP_SCREEN_SIZE = "screenSize";

DapUiScreen::ScreenSize DapUiScreen::getScreenSize()
{
    ScreenSize screenSize; //returned value

    QCoreApplication *app = QApplication::instance();
    if (app->property(PROP_SCREEN_SIZE).isValid()) { //retun value from dinamic property if exist
        return app->property(PROP_SCREEN_SIZE).value<ScreenSize>();
    }
    else {

        QScreen *screen = QGuiApplication::primaryScreen();
        QSize avSize =  screen->availableSize();

        int avWidth  = avSize.width(); 
        int avHeight = avSize.height();


#ifdef DAP_PLATFORM_DESKTOP
        const QSize WINDOW_BORDER_MAX_SIZE_PX = QSize(0, 25); // width of left and right borders and height of caption and bottom border
        const int TASK_BAR_MAX_SIZE_PX = 100; // For checking

        QSize fullScreenSize = screen->size();

        // in some cases task availableSize method return wrong sizes (too small).
        // If there are several screens
        bool severalScreens = QApplication::desktop()->screenCount() > 1;
        bool wrongSize = (fullScreenSize.height() - avHeight) > TASK_BAR_MAX_SIZE_PX;

        if (severalScreens && wrongSize)
        {
            avSize = fullScreenSize;
        }

        screenSize = Small;
        for (auto curSize: {Big, Medium}) {
            QSize screenResolution (getWindowSizeInPoints(curSize) += WINDOW_BORDER_MAX_SIZE_PX);

            if (avHeight >= screenResolution.height()
                    && avWidth >= screenResolution.width()) {
                screenSize = curSize;
                break;
            }
        }

#else
        if(avHeight > avWidth){
            avWidth  = avSize.height();
            avHeight = avSize.width();
        }

        if(avHeight < 700)
            screenSize = Small;
        if(avHeight > 1400)
            screenSize = Big;
        else
            screenSize = Medium;
#endif

        QApplication::instance()->setProperty(PROP_SCREEN_SIZE, QVariant(screenSize));
        
        return screenSize;
    }
}

/**
 * @brief DapUiScreen::show
 */
void DapUiScreen::show()
{
   rotate(rotation());
}

/**
 * @brief DapUiScreenAbstract::rotate
 * @param a_rot
 */
void DapUiScreen::rotate(ScreenRotation a_rot)
{
    m_rotation=a_rot;
    if(rotations().contains(a_rot))
        sw()->setCurrentWidget(page(a_rot));
}



/**
 * @brief DapUiScreenAbstract::connectTo
 * @param a_objName
 * @param a_signal
 * @param a_recv
 * @param a_slot
 */
void DapUiScreen::connectTo(const QString& a_objName,const char *a_signal, const QObject *a_recv, const char *a_slot )
{
    foreach (auto rotation, rotations()) {
        QWidget * w = getWidget(a_objName,rotation);
        if(w)
            connect(w,a_signal,a_recv,a_slot);
    }
}


void DapUiScreen::update()
{
    if(rotations().contains( DapUiMainWindow::getInstance()->rotation())){
        QWidget * w = page(DapUiMainWindow::getInstance()->rotation());
        if(w != sw()->currentWidget()){
            sw()->setCurrentWidget(w);
        }
    }
}
