#include <QtDebug>
#include <QGraphicsView>
#include <QScrollBar>
#include <QGraphicsItemGroup>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsItemGroup>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QToolButton>
#include "DapUiMainWindow.h"
#include "DapUiMenuBar.h"
#include "Manager.h"


QGraphicsDropShadowEffect *genShadow2(DapUiMenuBar* a_w)
{
    QGraphicsDropShadowEffect* l_dropShadowEffect = new QGraphicsDropShadowEffect(a_w);
    l_dropShadowEffect->setColor(QColor(0, 36, 25, 80));
    l_dropShadowEffect->setXOffset(0);
    l_dropShadowEffect->setYOffset(-1);
    l_dropShadowEffect->setBlurRadius(10);
    return l_dropShadowEffect;
}

QPoint DapUiMenuBar::toExpandedMenuBtnPoint(double a_f)
{
    double l_k = m_mainWindow->size().width()/360.0;
    int l_r = 55;
    int x = ((368.0 - l_r)*l_k/2.0)*cos(a_f*3.1415) + 368/2.0*l_k - 34.0/2.0*l_k;;
    int y = -((368.0 - l_r)*l_k/2.0)*sin(a_f*3.1415) - 34.0/2.0*l_k + (368.0/2.0)*l_k;
    return QPoint(x, y);
}

void DapUiMenuBar::drawExpandedMenu(double a_color, double a_pos)
{
    double l_k = m_mainWindow->size().width()/360.0;
    if (scene) delete scene;
    gv->setScene(scene = new QGraphicsScene(this));
    scene->setBackgroundBrush(QBrush(QColor("#fff")));
    // (616-204)

    QPixmap l_pix = QPixmap(m_mainWindow->sw()->size());
    m_mainWindow->sw()->render(&l_pix);
    QGraphicsPixmapItem *l_pixItem = new QGraphicsPixmapItem(l_pix);
    l_pixItem->setX(((-180 + 52/2)*l_k-1) + 1);
    l_pixItem->setZValue(0);

    scene->addItem(l_pixItem);

    QGraphicsRectItem * l_shadow = new QGraphicsRectItem((-368.0/2.0 + 52/2)*l_k, 0, 400*l_k, 700*l_k);
    l_shadow->setPen(QColor(0,0,0,0));
    l_shadow->setZValue(1);
    l_shadow->setBrush(QBrush(QColor(170, 182, 203, 255*0.16*a_color)));
    l_shadow->setGraphicsEffect(genShadow2(this));
    l_shadow->setData(0, "centerBtn");
    scene->addItem(l_shadow);

    QGraphicsItemGroup * l_ellipseGroup = new QGraphicsItemGroup();
    l_ellipseGroup->setPos((-368.0/2.0 + 52/2)*l_k, (616-204*a_pos)*l_k);
    l_ellipseGroup->setZValue(1);

    QGraphicsEllipseItem *l_grEllipse = new QGraphicsEllipseItem(0, 0, 368*l_k, 368*l_k, l_ellipseGroup);
    l_grEllipse->setBrush(QBrush(QColor(255,255,255)));
    l_grEllipse->setPen(QColor(0,0,0,0));
    l_grEllipse->setGraphicsEffect(genShadow2(this));
    l_grEllipse->setZValue(1);

    scene->addItem(l_ellipseGroup);

    scene->setSceneRect((-180 + 52/2)*l_k, 0, 360*l_k, 616*l_k);

    QGraphicsRectItem * l_rect = new QGraphicsRectItem(((-180 + 52/2)*l_k-1) + 1, (616-59)*l_k, (616 - 60)*l_k, 60*l_k);
    l_rect->setPen(QColor(0,0,0,0));
    l_rect->setZValue(2);
    l_rect->setBrush(QBrush(QColor("#20DCA3")));
    l_rect->setGraphicsEffect(genShadow2(this));

    int l_shift = (616 - 72)*l_k;
    drawCenterBtn(":/icons/add-active@3x.png", l_shift);
    drawMinimizedMenuBtn(":/icons/diary-noactive@3x.png", -108*l_k, tr("Дневник"), "diary",  l_shift);
    drawMinimizedMenuBtn(":/icons/profile-noactive@3x.png", 133*l_k, tr("Профиль"), "profile",  l_shift);

    QPoint p1 = toExpandedMenuBtnPoint(5.0/6.0);
    drawExpandedMenuBtn(":/icons/breakfast-menu-icon-noactive@3x.png", p1.x(), p1.y(), tr("Завтрак"), "break", l_ellipseGroup);

    QPoint p2 = toExpandedMenuBtnPoint(4.0/6.0);
    drawExpandedMenuBtn(":/icons/lunch-menu-icon-noactive@3x.png", p2.x(), p2.y(), tr("Обед"), "lunch", l_ellipseGroup);

    QPoint p3 = toExpandedMenuBtnPoint(3.0/6.0);
    drawExpandedMenuBtn(":/icons/activity-menu-icon-noactive@3x.png", p3.x(), p3.y(), tr("Активность"), "activity", l_ellipseGroup);

    QPoint p4 = toExpandedMenuBtnPoint(2.0/6.0);
    drawExpandedMenuBtn(":/icons/dinner-menu-icon-noactive@3x.png", p4.x(), p4.y(), tr("Ужин"), "dinner", l_ellipseGroup);

    QPoint p5 = toExpandedMenuBtnPoint(1.0/6.0);
    drawExpandedMenuBtn(":/icons/snack-menu-icon-noactive@3x.png", p5.x(), p5.y(), tr("Перекус"), "snack", l_ellipseGroup);

    QGraphicsEllipseItem *l_e2 = new  QGraphicsEllipseItem(QRect(((-180 + 52/2)*l_k-1) + 1,   1, 1, 1));
    QGraphicsEllipseItem *l_e3 = new  QGraphicsEllipseItem(QRect(((180 + 52/2)*l_k-1) - 1, 1, 1, 1));

    l_e2->setPen(QPen(QColor("#20DCA3")));
    l_e3->setPen(QPen(QColor("#20DCA3")));

    scene->addItem(l_e2);
    scene->addItem(l_e3);

    scene->addItem(l_rect);
}

void DapUiMenuBar::drawCenterBtn(QString a_btnIcon, int a_shift)
{
    double l_k = m_mainWindow->size().width()/360.0;
    // central button
    QPixmap l_pixmap = QPixmap(a_btnIcon);
    l_pixmap = l_pixmap.scaledToHeight(52*l_k);

    QGraphicsPixmapItem *l_addBtn = new QGraphicsPixmapItem(l_pixmap);
    l_addBtn->setPos(0, a_shift);
    l_addBtn->setZValue(14);
    l_addBtn->setData(0, "centerBtn");

    QLinearGradient l_grad(0,0, 0, 52*l_k);
    l_grad.setColorAt(0, QColor(31, 197, 193));
    l_grad.setColorAt(1, QColor(26, 229, 161));

    QGraphicsEllipseItem *l_ellipse1 = new QGraphicsEllipseItem(QRect(0,a_shift, 52*l_k, 52*l_k));
    l_ellipse1->setZValue(11);
    l_ellipse1->setBrush(QColor(255,255,255));
    l_ellipse1->setPen(QPen(QColor(0,0,0,0)));
    l_ellipse1->setData(0, "centerBtn");
    l_ellipse1->setGraphicsEffect(genShadow2(this));

    QGraphicsEllipseItem *l_ellipse2 = new  QGraphicsEllipseItem(QRect(5*l_k,5*l_k+a_shift, 42*l_k, 42*l_k));
    l_ellipse2->setZValue(12);
    l_ellipse2->setBrush(QBrush(l_grad));
    l_ellipse2->setPen(QPen(QColor(0,0,0,0)));
    l_ellipse2->setData(0, "centerBtn");

    QGraphicsEllipseItem *l_ellipse3 = new  QGraphicsEllipseItem(QRect(7*l_k,7*l_k+a_shift, 38*l_k, 38*l_k));
    l_ellipse3->setZValue(13);
    l_ellipse3->setBrush(QColor(255,255,255));
    l_ellipse3->setPen(QPen(QColor(0,0,0,0)));
    l_ellipse3->setData(0, "centerBtn");
    scene->addItem(l_ellipse1);
    scene->addItem(l_ellipse2);
    scene->addItem(l_addBtn);
    scene->addItem(l_ellipse3);
    QFont l_font = QFont("Roboto");
    l_font.setPixelSize(10*l_k);
    QColor l_color = QColor(255,255,255, 255*0.85);
    if (a_shift > 1) l_color.setAlpha(255);

    drawCentredText(tr("Добавить"), 52.0*l_k/2, 48*l_k + a_shift, l_font, l_color, "centerBtn");
}

void DapUiMenuBar::drawMinimizedMenuBtn(QString a_path, int a_pos, QString a_btnName, QString a_key, int a_shift)
{
    double l_k = m_mainWindow->size().width()/360.0;
    QPixmap l_pm = QPixmap(a_path);
    l_pm = l_pm.scaledToHeight(22*l_k);
    QGraphicsPixmapItem *l_btn = new QGraphicsPixmapItem(l_pm);
    l_btn->setPos(a_pos, 22*l_k + a_shift);
    l_btn->setZValue(10);
    scene->addItem(l_btn);

    QFont l_font = QFont("Roboto");
    l_font.setPixelSize(10*l_k);
    drawCentredText(a_btnName, a_pos + 11*l_k, 48*l_k + a_shift, l_font, QColor(255,255,255, 255*0.85), a_key);
}

void DapUiMenuBar::drawCentredText(QString a_txt, int a_x, int a_y, QFont a_font, QColor a_color, QString a_key, QGraphicsItemGroup *a_gr)
{
    QGraphicsTextItem *l_txt = new QGraphicsTextItem(a_txt, a_gr);
    l_txt->setFont(a_font);
    l_txt->setDefaultTextColor(a_color);
    double l_shiftPos = l_txt->boundingRect().width()/2.0;
    l_txt->setPos(a_x - l_shiftPos, a_y);
    l_txt->setZValue(10);
    l_txt->setData(0, a_key);
    scene->addItem(l_txt);
}

void DapUiMenuBar::drawExpandedMenuBtn(QString a_path, int x, int y, QString a_btnName, QString a_key, QGraphicsItemGroup *a_gr)
{
    double l_k = m_mainWindow->size().width()/360.0;
    qDebug() << "DapUiMenuBar::drawExpandedMenuBtn";
    QGraphicsItemGroup *btnGroup = new QGraphicsItemGroup(a_gr);
    btnGroup->setPos(x, y);
    btnGroup->setZValue(1);

    QPixmap l_pm = QPixmap(a_path);
    l_pm = l_pm.scaledToHeight(35*l_k);
    QGraphicsPixmapItem *l_btn = new QGraphicsPixmapItem(l_pm, btnGroup);
    l_btn->setData(0, a_key);
    l_btn->setZValue(1);

    QGraphicsEllipseItem *l_el = new QGraphicsEllipseItem(0, 0, 35*l_k, 35*l_k, btnGroup);
    l_el->setBrush(QColor(255,255,255, 255));
    l_el->setPen(QPen(QColor(255,255,255, 255)));
    l_el->setData(0, a_key);

    scene->addItem(btnGroup);

    QFont l_font = QFont("Roboto");
    l_font.setPixelSize(11*l_k);
    drawCentredText(a_btnName, (35.0*l_k/2), 33*l_k, l_font, QColor("#AAB6CB"), a_key, btnGroup);
}

void DapUiMenuBar::drawMinimizedMenu()
{
    qDebug() << "DapUiMenuBar::drawMinimizedMenu";
    double l_k = m_mainWindow->size().width()/360.0;
    if (scene) delete scene;
    gv->setScene(scene = new QGraphicsScene(this));
    scene->setBackgroundBrush(QBrush(QColor("#fff")));
    scene->setSceneRect((-180 + 52/2)*l_k, 0, 360*l_k, 70*l_k);

    int l_shift = 1;

    QGraphicsRectItem * l_rect = new QGraphicsRectItem(((-180 + 52/2)*l_k-1) + 1, l_shift + 13*l_k, (616 - 60)*l_k, 60*l_k);
    l_rect->setPen(QColor(0,0,0,0));
    l_rect->setZValue(2);
    l_rect->setBrush(QBrush(QColor("#20DCA3")));
    l_rect->setGraphicsEffect(genShadow2(this));
    scene->addItem(l_rect);

    if (m_pix)    delete m_pix;
    m_pix = new QPixmap(m_mainWindow->sw()->size());
    m_mainWindow->sw()->render(m_pix);
    m_pixmap = new QGraphicsPixmapItem(m_pix[0]);

    m_pixmap->setX(((-180 + 52/2)*l_k-1) + 1);
    m_pixmap->setY(-558*l_k + 15);
    m_pixmap->setZValue(0);
    scene->addItem(m_pixmap);

    drawCenterBtn(":/icons/add-noactive@3x.png", l_shift);
    drawMinimizedMenuBtn(":/icons/diary-noactive@3x.png", -108*l_k, tr("Дневник"), "diary", l_shift);
    drawMinimizedMenuBtn(":/icons/profile-noactive@3x.png", 133*l_k, tr("Профиль"), "profile", l_shift);

    QGraphicsEllipseItem *l_e2 = new  QGraphicsEllipseItem(QRect(((-180 + 52/2)*l_k -1) + 1,   70*l_k - 1 + 5, 1, 1));
    QGraphicsEllipseItem *l_e3 = new  QGraphicsEllipseItem(QRect(((180 + 52/2)*l_k -1) - 1, 70*l_k -1 + 5, 1, 1));

    l_e2->setPen(QPen(QColor("#20DCA3")));
    l_e3->setPen(QPen(QColor("#20DCA3")));

    scene->addItem(l_e2);
    scene->addItem(l_e3);
}

void DapUiMenuBar::setMinizedMask()
{
    double l_k = m_mainWindow->size().width()/360.0;
    QRegion l_reg   = QRegion(0, 13*l_k, 360*l_k, 6+58*l_k);
    QRegion l_regEl = QRegion((360-56)/2.0*l_k, 0, 56*l_k, 56*l_k, QRegion::Ellipse);
    l_reg = l_reg.united(l_regEl);
    gv->setMask(l_reg);
}


void DapUiMenuBar::gvInit(bool a_isMinimized)
{
    double l_k = m_mainWindow->size().width()/360.0;
    if (gv) delete gv;
    gv = new PressebleView(m_mainWindow);
    gv->setRenderHints(m_renderingHints);
    gv->setStyleSheet("border:none;");
    gv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    if (a_isMinimized) {
        gv->setGeometry(QRect(0, 540*l_k, 360*l_k, (616 - 540)*l_k));

    } else {
        gv->setGeometry(QRect(0, 0, 360*l_k, 616*l_k));
    }
}

void DapUiMenuBar::gvConnect()
{
    connect(gv, &PressebleView::sigClicked, [=](QPoint a_p){
        auto l_selItem = gv->itemAt(a_p.x(), a_p.y());
        if (!l_selItem) return;

        if (l_selItem->data(0) == "centerBtn") {
            emit sigCenterBtnClicked();
        } else if (l_selItem->data(0) ==  "activity" || l_selItem->data(0) ==  "profile" || l_selItem->data(0) ==  "diary") {
            return;
        } else if (l_selItem->data(0).toString() == "") {
            return;
        } else {
            Manager::me().setIngestion(l_selItem->data(0).toString());
            Manager::me().emitAddFood();
        }
    });
}

/**
 * @brief DapUiMenuBar::DapUiMenuBar
 * @param a_mainWindow
 */
DapUiMenuBar::DapUiMenuBar(DapUiMainWindow * a_mainWindow, QBrush a_bgBrush, int a_barHeight)
    : QWidget(a_mainWindow), m_mainWindow(a_mainWindow),m_barHeigth(a_barHeight),m_bgBrush(a_bgBrush)
{
    double l_k = a_mainWindow->size().width()/360.0;
    setMinimumHeight(m_barHeigth*l_k);
    setMaximumHeight(m_barHeigth*l_k);

    m_renderingHints =
        QPainter::Antialiasing |
        QPainter::HighQualityAntialiasing |
        QPainter::TextAntialiasing;

    m_t = new QTimer(this);
    connect(m_t, &QTimer::timeout, [=]{
        Manager::me().emitRending();
    });

    connect(&Manager::me(), &Manager::sigRending, [=]{
        if (m_currentState != "minimized") return;
        if (m_pix)    delete m_pix;
        m_pix = new QPixmap(m_mainWindow->sw()->size());
        m_mainWindow->sw()->render(m_pix);
        m_pixmap = new QGraphicsPixmapItem(m_pix[0]);
        m_pixmap->setX(((-180 + 52/2)*l_k-1) + 1);
        m_pixmap->setY(-558*l_k + 15);
        m_pixmap->setZValue(0);
        scene->addItem(m_pixmap);
    });

    // Create main layout
    setObjectName("DapUiMenuBarWidget");
    setLayout(new QVBoxLayout());
    layout()->setContentsMargins(0,0,0,0);
    QWidget *l_w = new QWidget(this);
    layout()->addWidget(l_w);
    l_w->setStyleSheet("background: #20DCA3;");

    // QStateMachine;
    stateHided               = new QState();     // menu hidden
    stateExpanded            = new QState();     // menu expanded
    stateExpandedToMinimized = new QState();
    stateMinimizedToExpanded = new QState();
    stateMinimized           = new QState();     // menu collapsed

    m_sm.addState(stateHided);
    m_sm.addState(stateExpanded);
    m_sm.addState(stateMinimized);
    m_sm.addState(stateExpandedToMinimized);
    m_sm.addState(stateMinimizedToExpanded);

    stateHided->addTransition(&Manager::me(), SIGNAL(sigMenuMinimize()), stateMinimized);
    stateMinimized->addTransition(this, SIGNAL(sigCenterBtnClicked()), stateMinimizedToExpanded);
    stateExpanded->addTransition(this, SIGNAL(sigCenterBtnClicked()), stateExpandedToMinimized);
    stateExpandedToMinimized->addTransition(this, SIGNAL(sigAnimationStop()), stateMinimized);
    stateMinimizedToExpanded->addTransition(this, SIGNAL(sigAnimationStop()), stateExpanded);
    stateMinimized->addTransition(&Manager::me(), SIGNAL(sigMenuHide()), stateHided);
    stateExpanded->addTransition(&Manager::me(), SIGNAL(sigMenuHide()), stateHided);

/*

*/

    // hidden menu state
    connect(stateHided, &QState::entered, [=]{
        // hide everything
        m_currentState = "hided";
        if (gv) gv->setVisible(false);
        setVisible(false);
    });

    connect(stateHided, &QState::exited, [=]{
        if (gv) gv->setVisible(true);
        setVisible(true);
    });

    // expanded menu state
    connect(stateExpanded, &QState::entered, [=]{
        m_currentState = "expanded";
        gvInit(false);
        drawExpandedMenu();
        gvConnect();
        gv->show();
    });

    connect(stateMinimizedToExpanded, &QState::entered, [=]{
        m_currentState = "minimizedToExpanded";
        QTimeLine *l_timeLine = new QTimeLine(700, this);
        l_timeLine->setFrameRange(0, 28);
        connect(l_timeLine, &QTimeLine::frameChanged, [=](int s){
            if (s == 28) {
                emit sigAnimationStop();
            } else {
                gvInit(false);
                double l_pos = s/16.0;
                if (l_pos > 1) l_pos = 1;
                drawExpandedMenu(s/28.0, l_pos);
                gv->show();
            }
        });
        l_timeLine->start();
    });

    connect(stateExpandedToMinimized, &QState::entered, [=]{
        m_currentState = "expandedToMinimized";
        QTimeLine *l_timeLine = new QTimeLine(700, this);
        l_timeLine->setFrameRange(0, 28);
        connect(l_timeLine, &QTimeLine::frameChanged, [=](int s){
            if (s == 28) {
                emit sigAnimationStop();
            } else {
                gvInit(false);
                double l_pos = (16 - s)/16.0;
                if (l_pos < 0) l_pos = 0;
                drawExpandedMenu((28 - s)/28.0, l_pos);
                gv->show();
            }
        });
        l_timeLine->start();
    });

    connect(stateMinimized, &QState::entered, [=]{
        m_currentState = "minimized";
        gvInit(true);
        drawMinimizedMenu();
        setMinizedMask();
        gvConnect();
        gv->show();
        m_t->setSingleShot(true);
        m_t->start(10);
    });

    m_sm.setInitialState(stateHided);
    m_sm.start();
}


/**
 * @brief resizeEvent
 * @param event
 */
void DapUiMenuBar::resizeEvent(QResizeEvent *event)
{

    QWidget::resizeEvent(event);
}




