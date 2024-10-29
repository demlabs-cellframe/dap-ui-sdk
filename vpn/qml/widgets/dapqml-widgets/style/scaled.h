#ifndef SCALED_H
#define SCALED_H

/* INCLUDES */
#include <QObject>

/* NAMESPACE */
namespace DapStyle
{

/****************************************//**
 * @brief QSS scaled rect is a class
 * that provides **positioning** and **scaling**
 * features for **QML Items**
 *
 * It allows *aspect ratio scaling* using
 * default window resolution of 428 x 926
 * as base.
 *
 * It is also provide *font scaling* and *non
 * aspect ratio scaling*.
 *
 * To start using this class you need to
 * add new **field** inside **QSS stylesheet** text.
 *
 * @note do not add more than one field
 * to one instance. the result
 * behavior is undefined
 *
 * | Name | Arguments | Description |
 * | ---- | --------- | ----------- |
 * | scaled-rect | x, y, width, height, aspectRatio | setup scaling for QML Item |
 * | scaled-font | fontSize | setup scaling font for QML Item (property name is "fontSize") |
 * | scaled-rect | x, y, width, height, aspectRatio, fontSize | scaled-rect and scaled-font combined |
 *
 * You can set arguments **x** or **y** equals
 * to **-1**, if you want Item to be centered
 * by formula "(parent.width - width) / 2"
 *
 * You can also set **x**, **y**, **width** or **height**
 * equals to **nan**, if you want class to
 * ignore those fields. In that way class
 * will ignore those and will not
 * call **setProperty** to them.
 *
 * Make sure that property **fontSize** is exists
 * inside QML Item when you setting up
 * *font scaling*. Otherwise it will print
 * error log message telling that this
 * property name is not exists.
 *
 * @code
 *
 * .ph-btn-resizer
 * {
 *     scaled-rect: scaled(nan,nan,363,64,true); // ignored positioning, but applied aspect ratio scaling
 * }
 *
 * .dv-text
 * {
 *     color: "@text";
 *     scaled-font: scaled(14); // apply font scaling based on window size
 * }
 *
 * .dv-content
 * {
 *     scaled-rect: scaled(-1,144,363,nan,false,14); // x is centered, height is ignored, no aspect ratio, font scaled by window size
 * }
 *
 * @endcode
 *
 * @ingroup groupDapGuiStyle
 * @date 28.06.2022
 * @author Mikhail Shilenko
 *******************************************/

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

protected:
  struct Result
  {
    double x,y,w,h,f;
    double scrWidth, scrHeight;
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
  mutable Result m_result;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  Scaled();
  Scaled (const Scaled &src);
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

  /// @return true if result values are stored
  bool adjust (QObject *a_item, double a_screenWidth, double a_screenHeight) const;
  /// @return true if result values are stored
  bool calcAdjusted(
    double a_screenWidth,
    double a_screenHeight,
    double &a_x,
    double &a_y,
    double &a_width,
    double &a_height,
    double &a_fontSize) const;
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

Q_DECLARE_METATYPE(DapStyle::Scaled)

/*-----------------------------------------*/
#endif // SCALED_H
