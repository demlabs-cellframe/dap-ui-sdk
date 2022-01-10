#ifndef SCALED_H
#define SCALED_H

/* INCLUDES */
#include <QObject>

/* NAMESPACE */
namespace Style
{

class Scaled
{
  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
  double m_x, m_y, m_w, m_h;
  bool m_aspect;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  Scaled();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  double x() const;
  void setX(double newX);

  double y() const;
  void setY(double newY);

  double w() const;
  void setW(double newW);

  double h() const;
  void setH(double newH);

  bool aspect() const;
  void setAspect(bool newAspect);

  void adjust (QObject *a_item, double a_screenWidth, double a_screenHeight) const;
  /// @}
};

/*-----------------------------------------*/
}

Q_DECLARE_METATYPE(Style::Scaled)

/*-----------------------------------------*/
#endif // SCALED_H
