#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>

#include "schedules.h"


Schedules::Schedules() {}

/// Set graphic styles.
/// @param style Graphics styles.
void Schedules::setStyle(const DapGraphicSceneStyle &style)
{
    mColorChartDownload = new QColor(style.value("shChartDownload").value("color").toString());
    mDepthChartDownload = style.value("shChartDownload").value("depth").toReal();
    mBackGroundmColorChartDownload = new QColor(convertRGBA(style.value("shChartDownload").value("background-color").toString()));
    mColorChartUpload = new QColor(style.value("shChartUpload").value("color").toString());
    mDepthChartUpload = style.value("shChartUpload").value("depth").toReal();
    mBackGroundmColorChartUpload = new QColor(convertRGBA(style.value("shChartUpload").value("background-color").toString()));
    mColorGrid = new QColor(convertRGBA(style.value("shGrid").value("color").toString()));
    mDepthGrid = style.value("shGrid").value("depth").toReal();
    m_horizontLines = style.value("graphicsLines").value("horizontalLines").toInt();
    m_verticalLines = style.value("graphicsLines").value("verticalLines").toInt();
    m_sceneWidth = style.value("graphicsLines").value("sceneWidth").toString().split("px")[0].toInt();
    m_sceneHeight = style.value("graphicsLines").value("sceneHeight").toString().split("px")[0].toInt();

}

void Schedules::addInp(quint64 elem){inp.addElem(elem);}
void Schedules::addOut(quint64 elem){out.addElem(elem);}

int Schedules::getWidth()
{
    return m_sceneWidth+3;
}

int Schedules::getHeight()
{
    return m_sceneHeight+3;
}

void Schedules::draw_backgraund(QGraphicsScene *scene)
{
    QPainterPath path;

    int num_of_lines = m_horizontLines;

    for (int i = 1; i < num_of_lines; i++){
        path.moveTo(i*m_sceneWidth/num_of_lines, 0);
        path.lineTo(i*m_sceneWidth/num_of_lines, m_sceneHeight);
    }

    num_of_lines = m_verticalLines;
    for (int i = 1; i < num_of_lines; i++){
        path.moveTo(0, i*m_sceneHeight/num_of_lines);
        path.lineTo(m_sceneWidth, i*m_sceneHeight/num_of_lines);
    }
    scene->setBackgroundBrush(QBrush(Qt::transparent));
    scene->addPath(path, QPen(QColor(mColorGrid->rgba()), mDepthGrid));
}

int maxInt (int a, int b) {return a > b ? a : b;}


void Schedules::draw_chart(QGraphicsScene *scene)
{
    scene->clear();
    draw_backgraund(scene);

    if (out.size() < 2) return;

    int maxValue = maxInt(inp.maxValue(), out.maxValue());
    
    inp.showChart(
        scene,
        QPen(QColor(mColorChartDownload->rgba64()), mDepthChartDownload),
        QColor(mBackGroundmColorChartDownload->rgba64()),
         m_sceneWidth, m_sceneHeight, maxValue);
    
    out.showChart(
        scene,
        QPen(QColor(mColorChartUpload->rgba()), mDepthChartUpload),
        QColor(mBackGroundmColorChartUpload->rgba64()),
        m_sceneWidth, m_sceneHeight, maxValue);
}

/// Convert color in string representation to rgba.
/// @param strRGBA String color.
QColor Schedules::convertRGBA(const QString &strRGBA) 
{
    QVector<int> result;
    QString temp;
    for(auto it(strRGBA.begin()); it != strRGBA.end(); ++it)
    {
        if((*it).isDigit() || (*it) == '.')
            temp += *it;
        else if(!(temp.isEmpty() || temp.isNull()))
        {
            result.append(temp.contains(".") ? int(temp.toDouble() * 255) : temp.toInt());
            temp.clear();
        }  
    }
    return (result.size() == 4) ? QColor(result.at(0), result.at(1), result.at(2), result.at(3)) : QColor();
}
