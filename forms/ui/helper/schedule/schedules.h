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
    QColor  m_colorChartDownload;
    /// Load curve line thickness.
    qreal   mDepthChartDownload{0};
    /// Fill infinite load integral.
    QColor  *mBackGroundmColorChartDownload{nullptr};
    /// Upload curve color.
    QColor  m_colorChartUpload;
    /// Upload curve line thickness.
    qreal   mDepthChartUpload{0};
    /// Fill infinite upload integral.
    QColor  *mBackGroundmColorChartUpload{nullptr};
    /// Grid line color.
    QColor  *mColorGrid{nullptr};
    /// Thickness of grid lines.
    qreal   mDepthGrid{0};
    /// Background color
    QColor  m_colorBackground;

    
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

    qreal m_maxValue;
public:
    int getWidth();
    int getHeight();

    void addInp(quint64 elem);
    void addOut(quint64 elem);
    void reset();

    void draw_chart(QGraphicsScene *scene);
    Schedules();
    /// Set graphic styles.
    /// @param style Graphics styles.
    void setStyle(const DapGraphicSceneStyle &style);

    const QColor &colorBackground() const;
    void setColorBackground (const QColor &newColorBackground);
    const QColor &colorChartDownload() const;
    void setColorChartDownload (const QColor &newColorChartDownload);
    const QColor &colorChartUpload() const;
    void setColorChartUpload (const QColor &newColorChartUpload);
};



#endif // SCHEDULES_H
