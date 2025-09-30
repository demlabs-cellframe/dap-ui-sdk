#ifndef DAPUIMENUBAR_H
#define DAPUIMENUBAR_H

#include <QWidget>
#include <QMap>
#include <QStateMachine>
#include <QTimer>
#include "PressebleView.h"


class QGraphicsItem;
class QGraphicsRectItem;
class QGraphicsItemGroup;
class QGraphicsView;
class QGraphicsScene;
class DapUiMainWindow;
class QToolButton;

class DapUiMenuBar : public QWidget
{
    Q_OBJECT
private:
    DapUiMainWindow * m_mainWindow;
    PressebleView   * gv    = nullptr;
    QGraphicsScene  * scene = nullptr;

    int    m_barHeigth;
    QBrush m_bgBrush;

    QStateMachine m_sm;
    QState *stateHided;
    QState *stateExpanded;
    QState *stateExpandedToMinimized;
    QState *stateMinimizedToExpanded;
    QState *stateMinimized;

    // state check is needed for some functions
    QString m_currentState;

    QTimer *m_t;

    QPainter::RenderHints  m_renderingHints;
    QGraphicsPixmapItem * m_pixmap = nullptr;
    QPixmap             * m_pix = nullptr;

    void drawExpandedMenu(double a_color = 1, double a_pos = 1);
    void drawMinimizedMenu();
    void drawCenterBtn(QString a_btnIcon = ":/icons/add-active@3x.png", int a_shift = 0);
    void drawMinimizedMenuBtn(QString a_path, int a_pos, QString a_btnName, QString a_key, int a_shift = 0);
    void drawExpandedMenuBtn(QString a_path, int x, int y, QString a_btnName, QString a_key, QGraphicsItemGroup *a_gr = nullptr);
    void drawCentredText(QString a_txt, int a_x, int a_y, QFont a_font, QColor a_color, QString a_key, QGraphicsItemGroup *a_gr = nullptr);
    void setMinizedMask();

    void gvInit(bool a_isMinimized);
    void gvConnect();

    // find the top left right corner of the button
    QPoint toExpandedMenuBtnPoint(double a_f);

protected:
    void resizeEvent(QResizeEvent *event) override;
public:
    DapUiMenuBar(DapUiMainWindow * a_mainWindow, QBrush a_bgBrush, int a_barHeight);

signals:
    void sigCenterBtnClicked();
    void sigAnimationStop();
};

#endif // DAPUIMENUBAR_H
