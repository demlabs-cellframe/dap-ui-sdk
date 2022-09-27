#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>

#include "schedules.h"


Schedules::Schedules(): m_maxValue(0.0) {}

/// Set graphic styles.
/// @param style Graphics styles.
void Schedules::setStyle (const DapGraphicSceneStyle &style)
{
  m_colorChartDownload = QColor (style.value ("shChartDownload").value ("color").toString());
  mDepthChartDownload = style.value ("shChartDownload").value ("depth").toReal();
  mBackGroundmColorChartDownload = new QColor (convertRGBA (style.value ("shChartDownload").value ("background-color").toString()));
  m_colorChartUpload = QColor (style.value ("shChartUpload").value ("color").toString());
  mDepthChartUpload = style.value ("shChartUpload").value ("depth").toReal();
  mBackGroundmColorChartUpload = new QColor (convertRGBA (style.value ("shChartUpload").value ("background-color").toString()));
  mColorGrid = new QColor (convertRGBA (style.value ("shGrid").value ("color").toString()));
  mDepthGrid = style.value ("shGrid").value ("depth").toReal();
  m_horizontLines = style.value ("graphicsLines").value ("horizontalLines").toInt();
  m_verticalLines = style.value ("graphicsLines").value ("verticalLines").toInt();
  m_sceneWidth = style.value ("graphicsLines").value ("sceneWidth").toString().split ("px")[0].toInt();
  m_sceneHeight = style.value ("graphicsLines").value ("sceneHeight").toString().split ("px")[0].toInt();

}

void Schedules::addInp (quint64 elem) {inp.addElem (elem);}
void Schedules::addOut (quint64 elem) {out.addElem (elem);}

void Schedules::reset()
{
  inp.reset();
  out.reset();
}

int Schedules::getWidth()
{
  return m_sceneWidth + 3;
}

int Schedules::getHeight()
{
  return m_sceneHeight + 3;
}

const QColor &Schedules::colorBackground() const
{
  return m_colorBackground;
}

void Schedules::setColorBackground(const QColor &newColorBackground)
{
  m_colorBackground = newColorBackground;
}

const QColor &Schedules::colorChartDownload() const
{
  return m_colorChartDownload;
}

void Schedules::setColorChartDownload (const QColor &newColorChartDownload)
{
  m_colorChartDownload = newColorChartDownload;
}

const QColor &Schedules::colorChartUpload() const
{
  return m_colorChartUpload;
}

void Schedules::setColorChartUpload (const QColor &newColorChartUpload)
{
  m_colorChartUpload = newColorChartUpload;
}

void Schedules::draw_backgraund (QGraphicsScene *scene)
{
  QPainterPath path;

  int num_of_lines = m_horizontLines;

  for (int i = 0; i < num_of_lines + 1; i++)
    {
      path.moveTo (i * m_sceneWidth / num_of_lines, 0);
      path.lineTo (i * m_sceneWidth / num_of_lines, m_sceneHeight);
    }

  num_of_lines = m_verticalLines;
  for (int i = 0; i < num_of_lines + 1; i++)
    {
      path.moveTo (0, i * m_sceneHeight / num_of_lines);
      path.lineTo (m_sceneWidth, i * m_sceneHeight / num_of_lines);
    }
  scene->addRect (0, 0, m_sceneWidth, m_sceneHeight, QPen (Qt::transparent), QBrush (colorBackground()));
  scene->setBackgroundBrush (QBrush (Qt::transparent));
  scene->addPath (path, QPen (QColor (mColorGrid->rgba()), mDepthGrid));
}

qreal maxInt (qreal a, qreal b) {return a > b ? a : b;}


void Schedules::draw_chart (QGraphicsScene *scene, bool diagramVisible)
{
  m_sceneWidth  = scene->width();
  m_sceneHeight = scene->height();
  scene->clear();
  draw_backgraund (scene);

  if (!diagramVisible)
    return;

  if (out.size() < 2)
    return;

  m_maxValue = maxInt (inp.maxValue(), out.maxValue());

  inp.showChart (
    scene,
    QPen (QColor (m_colorChartDownload.rgba64()), mDepthChartDownload),
    QColor (mBackGroundmColorChartDownload->rgba64()),
    m_sceneWidth, m_sceneHeight, m_maxValue);

  out.showChart (
    scene,
    QPen (QColor (m_colorChartUpload.rgba()), mDepthChartUpload),
    QColor (mBackGroundmColorChartUpload->rgba64()),
    m_sceneWidth, m_sceneHeight, m_maxValue);
}

/// Convert color in string representation to rgba.
/// @param strRGBA String color.
QColor Schedules::convertRGBA (const QString &strRGBA)
{
  QVector<int> result;
  QString temp;
  for (auto it (strRGBA.begin()); it != strRGBA.end(); ++it)
    {
      if ((*it).isDigit() || (*it) == '.')
        temp += *it;
      else if (! (temp.isEmpty() || temp.isNull()))
        {
          result.append (temp.contains (".") ? int (temp.toDouble() * 255) : temp.toInt());
          temp.clear();
        }
    }
  return (result.size() == 4) ? QColor (result.at (0), result.at (1), result.at (2), result.at (3)) : QColor();
}
