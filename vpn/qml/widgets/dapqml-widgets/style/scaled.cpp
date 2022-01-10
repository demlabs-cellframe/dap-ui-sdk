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
  : m_x (0), m_y (0)
  , m_w (0), m_h (0)
  , m_aspect (false)
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
  bool centerHor  = false;
  bool centerVer  = false;
  bool fullWidth  = false;
  bool fullHeight = false;

  /* multipliers */
  double multH    = a_screenWidth / double(428);
  double multV    = a_screenHeight / double(926);
  double resultX  = x(),
         resultY  = y(),
         resultW, resultH;

  /* centers and fulls */
  if (x() == -1)
    { resultX = (a_screenWidth / 2); centerHor = true; }
  if (y() == -1)
    { resultY = (a_screenHeight / 2); centerVer = true; }

  if (w() == -1)
    { fullWidth = true; }
  if (h() == -1)
    { fullHeight = true; }

  /* calc aspect */
  if (!aspect())
    {
      resultW   = (fullWidth) ? (a_screenWidth) : (w() * multH);
      resultH   = (fullHeight) ? (a_screenHeight) : (h() * multV);
    }
  else
    {
      bool widthHigh  = w() > h();
      resultW   = (fullWidth) ? (a_screenWidth) : (w() * ((widthHigh) ? multV : multH));
      resultH   = (fullHeight) ? (a_screenHeight) : (h() * ((widthHigh) ? multV : multH));
    }

  /* calc */
  resultX   = (resultX * multH) - ((centerHor) ? (resultW / 2) : (0));
  resultY   = (resultY * multV) - ((centerVer) ? (resultH / 2) : (0));

  /* setup adjusts */
  a_item->setProperty ("x", resultX);
  a_item->setProperty ("y", resultY);
  a_item->setProperty ("width", resultW);
  a_item->setProperty ("height", resultH);
}

/*-----------------------------------------*/
}

/*-----------------------------------------*/
