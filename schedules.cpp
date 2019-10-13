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
}

void Schedules::addInp(int elem){inp.addElem(elem);}
void Schedules::addOut(int elem){out.addElem(elem);}

void Schedules::draw_backgraund(QGraphicsScene *scene, int width, int height)
{
    QPainterPath path;

    int num_of_lines = 14;

    for (int i = 1; i < num_of_lines; i++){
        path.moveTo(i*width/num_of_lines, 0);
        path.lineTo(i*width/num_of_lines, height);
    }

    num_of_lines = 18;
    for (int i = 1; i < num_of_lines; i++){
        path.moveTo(0, i*height/num_of_lines);
        path.lineTo(width, i*height/num_of_lines);
    }
    scene->setBackgroundBrush(QBrush(Qt::transparent));
    scene->addPath(path, QPen(QColor(mColorGrid->rgba()), mDepthGrid));
}

int maxInt (int a, int b) {return a > b ? a : b;}


void Schedules::draw_chart(QGraphicsScene *scene, int width, int height)
{
    scene->clear();
    draw_backgraund(scene, width, height);

    if (out.size() < 2) return;

    int maxValue = maxInt(inp.maxValue(), out.maxValue());
    
    inp.showChart(
        scene,
        QPen(QColor(mColorChartDownload->rgba64()), mDepthChartDownload),
        QColor(mBackGroundmColorChartDownload->rgba64()),
         width, height, maxValue);
    
    out.showChart(
        scene,
        QPen(QColor(mColorChartUpload->rgba()), mDepthChartUpload),
        QColor(mBackGroundmColorChartUpload->rgba64()),
        width, height, maxValue);
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
