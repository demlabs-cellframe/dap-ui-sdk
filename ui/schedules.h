#ifndef SCHEDULES_H
#define SCHEDULES_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QString>

#include "schedule.h"

typedef QMap<QString, QMap<QString, QVariant>> DapGraphicSceneStyle;

class Schedules
{

private:
    /// Download curve color.
    QColor  *mColorChartDownload{nullptr};
    /// Load curve line thickness.
    qreal   mDepthChartDownload{0};
    /// Fill infinite load integral.
    QColor  *mBackGroundmColorChartDownload{nullptr};
    /// Upload curve color.
    QColor  *mColorChartUpload{nullptr};
    /// Upload curve line thickness.
    qreal   mDepthChartUpload{0};
    /// Fill infinite upload integral.
    QColor  *mBackGroundmColorChartUpload{nullptr};
    /// Grid line color.
    QColor  *mColorGrid{nullptr};
    /// Thickness of grid lines.
    qreal   mDepthGrid{0};
    
    
    Schedule inp = Schedule();
    Schedule out = Schedule();

    void draw_backgraund(QGraphicsScene *scene);
    /// Convert color in string representation to rgba.
    /// @param strRGBA String color.
    QColor convertRGBA(const QString &strRGBA);
    
    int m_horizontLines;
    int m_verticalLines;

    int m_sceneWidth;
    int m_sceneHeight;
public:
    int getWidth();
    int getHeight();

    void addInp(int elem);
    void addOut(int elem);

    void draw_chart(QGraphicsScene *scene);
    Schedules();
    /// Set graphic styles.
    /// @param style Graphics styles.
    void setStyle(const DapGraphicSceneStyle &style);
};



#endif // SCHEDULES_H
