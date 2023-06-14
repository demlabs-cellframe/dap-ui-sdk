/* INCLUDES */
#include "scaled.h"
#include <QVariant>
#include <QQmlProperty>

/* NAMESPACE */
namespace DapStyle
{

/********************************************
 * METHODS
 *******************************************/

Scaled::Scaled()
  : QObject()
  , m_x (0), m_y (0)
  , m_w (0), m_h (0)
  , m_fontSize (0)
  , m_aspect (false)
  , m_type (Invalid)
  , m_result ({0,0,0,0,0})
{

}

Scaled::Scaled (const Scaled &src)
  : QObject()
  , m_x (src.m_x), m_y (src.m_y)
  , m_w (src.m_w), m_h (src.m_h)
  , m_fontSize (src.m_fontSize)
  , m_aspect (src.m_aspect)
  , m_type (src.m_type)
  , m_result (src.m_result)
{

}

Scaled::~Scaled()
{

}

/********************************************
 * METHODS
 *******************************************/

double Scaled::x() const
{
  return m_x;
}

void Scaled::setX(double newX)
{
  m_x = newX;
}

double Scaled::y() const
{
  return m_y;
}

void Scaled::setY(double newY)
{
  m_y = newY;
}

double Scaled::w() const
{
  return m_w;
}

void Scaled::setW(double newW)
{
  m_w = newW;
}

double Scaled::h() const
{
  return m_h;
}

void Scaled::setH(double newH)
{
  m_h = newH;
}

int Scaled::fontSize() const
{
  return m_fontSize;
}

void Scaled::setFontSize(int newFontSize)
{
  m_fontSize = newFontSize;
}

bool Scaled::aspect() const
{
  return m_aspect;
}

void Scaled::setAspect(bool newAspect)
{
  m_aspect = newAspect;
}

Scaled::Type Scaled::type() const
{
  return m_type;
}

void Scaled::setType(Type newType)
{
  m_type = newType;
}

bool Scaled::adjust (QObject *a_item, double a_screenWidth, double a_screenHeight) const
{
  /* variables */
  double resultX, resultY, resultW, resultH, resultFontSize;

  /* defs */
  bool storedResult = calcAdjusted(
    a_screenWidth, a_screenHeight,
    resultX, resultY,
    resultW, resultH,
    resultFontSize);

  /* setup adjusts */
  if (type() == RectOnly || type() == All)
    {
      if (x() != -32000)
        QQmlProperty::write (a_item, "x", resultX); // a_item->setProperty ("x", resultX);
      if (y() != -32000)
        QQmlProperty::write (a_item, "y", resultY); // a_item->setProperty ("y", resultY);
      if (w() != -32000)
        QQmlProperty::write (a_item, "width", resultW); // a_item->setProperty ("width", resultW);
      if (h() != -32000)
        QQmlProperty::write (a_item, "height", resultH); // a_item->setProperty ("height", resultH);
    }

  /* setup font adjustments */
  if (type() == FontOnly || type() == All)
    {
      /* if item have fontSize property */
      if (a_item->property ("fontSize").isValid())
        {
          QQmlProperty::write (a_item, "fontSize", resultFontSize); // a_item->setProperty ("fontSize", fs);
        }
    }
  return storedResult;
}

bool Scaled::calcAdjusted(
    double a_screenWidth,
    double a_screenHeight,
    double &a_x,
    double &a_y,
    double &a_width,
    double &a_height,
    double &a_fontSize) const
{
  /* return precalc */
  if (m_result.scrWidth == a_screenWidth
      && m_result.scrHeight == a_screenHeight)
  {
    a_x         = m_result.x;
    a_y         = m_result.y;
    a_width     = m_result.w;
    a_height    = m_result.h;
    a_fontSize  = m_result.f;
    return false;
  }

  /* defs */
  bool centerHor  = x() == -1;
  bool centerVer  = y() == -1;
  bool fullWidth  = w() == -1;
  bool fullHeight = h() == -1;

  /* multipliers */
  double multH    = a_screenWidth / double(428);
  double multV    = a_screenHeight / double(926);
  double resultX  = x(),
         resultY  = y(),
         resultW, resultH;

  /* calc */
  resultX = (!centerHor) ? (resultX * multH) : (0);
  resultY = (!centerVer) ? (resultY * multV) : (0);
  resultW = (!fullWidth) ? (w() * multH) : (a_screenWidth);
  resultH = (!fullHeight) ? (h() * multV) : (a_screenHeight);

  /* aspect */
  if (aspect() && !fullWidth && !fullHeight)
    {
      /* put centered inside rectangle */
      if (resultW > resultH)
        {
          resultX += (w() * multH - w() * multV) / 2;
          resultW = w() * multV;
        }
      else
        {
          resultY += (h() * multV - h() * multH) / 2;
          resultH = h() * multH;
        }
    }

    /* move to center on the whole screen */
    if (centerHor)
      resultX = (a_screenWidth - resultW) / 2;
    if (centerVer)
      resultY = (a_screenHeight - resultH) / 2;

  /* return result */
  a_x         = resultX;
  a_y         = resultY;
  a_width     = resultW;
  a_height    = resultH;
  a_fontSize  = fontSize() * multV;

  /* store result */
  m_result = Result {
    resultX,
    resultY,
    resultW,
    resultH,
    a_fontSize,
    a_screenWidth,
    a_screenHeight
  };

  return true;
}

/********************************************
 * OPERATORS
 *******************************************/

Scaled &Scaled::operator=(const Scaled &src)
{
  m_x         = src.m_x;
  m_y         = src.m_y;
  m_w         = src.m_w;
  m_h         = src.m_h;
  m_fontSize  = src.m_fontSize;
  m_aspect    = src.m_aspect;
  m_type      = src.m_type;
  m_result    = src.m_result;
  return *this;
}

/*-----------------------------------------*/
}

/*-----------------------------------------*/
