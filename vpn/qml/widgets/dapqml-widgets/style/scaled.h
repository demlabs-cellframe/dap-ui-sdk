#ifndef SCALED_H
#define SCALED_H

/* INCLUDES */
#include <QObject>

/* NAMESPACE */
namespace Style
{

class Scaled : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum Type
  {
    Invalid,

    RectOnly,
    FontOnly,
    All
  };
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  double m_x, m_y, m_w, m_h;
  int m_fontSize;
  bool m_aspect;
  Type m_type;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  Scaled();
  Scaled(const Scaled &src);
  ~Scaled();
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

  int fontSize() const;
  void setFontSize(int newFontSize);

  bool aspect() const;
  void setAspect(bool newAspect);

  Type type() const;
  void setType(Type newType);

  void adjust (QObject *a_item, double a_screenWidth, double a_screenHeight) const;
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  Scaled& operator=(const Scaled &src);
  /// @}
};

/*-----------------------------------------*/
}

Q_DECLARE_METATYPE(Style::Scaled)

/*-----------------------------------------*/
#endif // SCALED_H
