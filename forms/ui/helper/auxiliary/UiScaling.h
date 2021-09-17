#ifndef UISCALING_H
#define UISCALING_H

#include <QSize>

class UiScaling
{
  UiScaling();             ///< private! Can't be created. Only for static methods

public:

  static float pointsToPixels (float a_pointsValue, float dpi = 0);
  static QSize pointsToPixels (const QSize &a_pointsSize);
  static float getNativDPI();
  static void setPseudoDPI (const float &pseudoDPI);


private:
  inline static float aptToPt (float apt);
  inline static double pointsToInches (float a_pointsValue);
};

#endif // UISCALING_H
