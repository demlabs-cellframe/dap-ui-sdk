#include <QtDebug>
#include <QStackedWidget>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen>
#include "DapUiScreen.h"
#include "DapUiMainWindow.h"

/**
 * @brief DapUiScreen::DapUiScreen
 * @param parent
 * @param a_sw
 */
DapUiScreen::DapUiScreen(QObject *parent, QStackedWidget * a_sw)
    : QObject(parent),m_sw(a_sw)
{
    Q_ASSERT(m_sw);
    m_defaultVariant = VariantDefault;
}

/**
 * @brief DapUiScreen::~DapUiScreen
 */
DapUiScreen::~DapUiScreen()
{
    for(auto l_page: m_page){
        sw()->removeWidget(l_page);
        delete l_page;
    }
}

/**
 * @brief DapUiScreen::getScreenSize
 * @return
 */
DapUiScreen::ScreenSize DapUiScreen::getScreenSize()
{
    ScreenSize ret=Medium;
    QRect avGem=QGuiApplication::primaryScreen()->availableGeometry();
    int h = avGem.height();
    int w = avGem.width();
    if(h>w) h=avGem.width();

#ifdef DAP_PLATFORM_DESKTOP
    if(h<800)
        ret=Small;
    if(h>1500)
        ret=Big;
#else
    if(h<700)
        ret=Small;
    if(h>1400)
        ret=Big;
#endif
    return ret;
}

/**
 * @brief DapUiScreen::show
 */
void DapUiScreen::show()
{
   setCurrentVariant(currentVariant());
}

/**
 * @brief DapUiScreenAbstract::rotate
 * @param a_rot
 */
void DapUiScreen::setCurrentVariant(ScreenVariant a_newVariant)
{
    if(variants().contains(a_newVariant)){
        m_currentVariant = a_newVariant;
        sw()->setCurrentWidget(page(a_newVariant));
    } else {
        qDebug() <<"No such variant "<<a_newVariant<<" in created forms map, setting up VariantDefault";
        setCurrentVariantDefault();
    }
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
    foreach (auto rotation, variants()) {
        QWidget * w = getWidget(a_objName,rotation);
        if(w)
            connect(w,a_signal,a_recv,a_slot);
    }
}
