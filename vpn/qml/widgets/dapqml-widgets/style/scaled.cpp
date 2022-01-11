/* INCLUDES */
#include "scaled.h"
#include <QVariant>

/* NAMESPACE */
namespace Style
{

/********************************************
 * METHODS
 *******************************************/

Scaled::Scaled()
  : QObject()
  , m_x (0), m_y (0)
  , m_w (0), m_h (0)
  , m_aspect (false)
{

}

Scaled::Scaled (const Scaled &src)
  : QObject()
  , m_x (src.m_x), m_y (src.m_y)
  , m_w (src.m_w), m_h (src.m_h)
  , m_aspect (src.m_aspect)
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

bool Scaled::aspect() const
{
  return m_aspect;
}

void Scaled::setAspect(bool newAspect)
{
  m_aspect = newAspect;
}

void Scaled::adjust (QObject *a_item, double a_screenWidth, double a_screenHeight) const
{
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

  /* setup adjusts */
  a_item->setProperty ("x", resultX);
  a_item->setProperty ("y", resultY);
  a_item->setProperty ("width", resultW);
  a_item->setProperty ("height", resultH);
}

/********************************************
 * OPERATORS
 *******************************************/

Scaled &Scaled::operator=(const Scaled &src)
{
  m_x       = src.m_x;
  m_y       = src.m_y;
  m_w       = src.m_w;
  m_h       = src.m_h;
  m_aspect  = src.m_aspect;
  return *this;
}

/*-----------------------------------------*/
}

/*-----------------------------------------*/
