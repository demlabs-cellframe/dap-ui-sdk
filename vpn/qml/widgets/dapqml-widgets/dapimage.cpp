/* INCLUDES */
#include "dapimage.h"

#include <QMetaType>
#include <cmath>

/* DEFS */
//#define FORCE_LANCZOS
#define SAMPLE_LENGTH (2)

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapImage::DapImage()
{

}

DapImage::DapImage (const DapImage &a_src)
  : QImage (a_src)
{

}

DapImage::DapImage (DapImage &&a_src)
  : QImage (std::move (a_src))
{

}

DapImage::DapImage (const QImage &a_src)
  : QImage (a_src)
{

}

DapImage::DapImage (QImage &&a_src)
  : QImage (std::move (a_src))
{

}

DapImage::DapImage (const QSize &size, Format format)
  : QImage (size, format)
{

}

DapImage::DapImage (int width, int height, Format format)
  : QImage (width, height, format)
{

}

DapImage::DapImage (uchar *data, int width, int height, Format format, QImageCleanupFunction cleanupFunction, void *cleanupInfo)
  : QImage (data, width, height, format, cleanupFunction, cleanupInfo)
{

}

DapImage::DapImage (const uchar *data, int width, int height, Format format, QImageCleanupFunction cleanupFunction, void *cleanupInfo)
  : QImage (data, width, height, format, cleanupFunction, cleanupInfo)
{

}

DapImage::DapImage (uchar *data, int width, int height, int bytesPerLine, Format format, QImageCleanupFunction cleanupFunction, void *cleanupInfo)
  : QImage (data, width, height, bytesPerLine, format, cleanupFunction, cleanupInfo)
{

}

DapImage::DapImage (const uchar *data, int width, int height, int bytesPerLine, Format format, QImageCleanupFunction cleanupFunction, void *cleanupInfo)
  : QImage (data, width, height, bytesPerLine, format, cleanupFunction, cleanupInfo)
{

}

/********************************************
 * METHODS
 *******************************************/

void DapImage::init()
{
  qRegisterMetaType<DapImage> ("DapImage");
}

DapImage DapImage::scaled (const QSize &size, Qt::AspectRatioMode aspectRatioMode, TransformationMode transformMode) const
{
  return scaled (size.width(), size.height(), aspectRatioMode, transformMode);
}

DapImage DapImage::scaled (int width, int height, Qt::AspectRatioMode aspectRatioMode, TransformationMode transformMode) const
{
#ifdef FORCE_LANCZOS
  //transformMode = BicubicTransformation;
  transformMode = LanczosTransformation;
#else // FORCE_LANCZOS
  /* old transformation technics */
  if (transformMode == TransformationMode::FastTransformation)
    return QImage::scaled (width, height, aspectRatioMode, Qt::FastTransformation);
  else if (transformMode == TransformationMode::SmoothTransformation)
    return QImage::scaled (width, height, aspectRatioMode, Qt::SmoothTransformation);
#endif // FORCE_LANCZOS


  /* defs */
  struct ColorData
  {
    double red    = 0.0;
    double green  = 0.0;
    double blue   = 0.0;
    double alpha  = 0.0;
  };

  /* get pixel data */
  auto getPixelData = [] (QImage & a_src, int x, int y) -> uchar *
  {
    return &a_src.bits()[ x * sizeof (qint32) + y * a_src.width() * sizeof (qint32) ];
  };

  /* calculate lanczos weight value */
  auto calcLanczosWeight = [] (double a_distance, double a_window) -> double
  {
    if (a_distance == 0.0)
      return 1.0;
    else if (std::abs (a_distance) < a_window)
      {
        constexpr double pi = M_PI;
        return (a_window * std::sin (pi * a_distance) * std::sin (pi * a_distance / a_window))
        / (pi * pi * a_distance * a_distance);
      }
    else
      return 0.0;
  };

  /* calc bicubic interpolation weight */
  auto calcBicubicInterpolationWeight = [] (double a_distance, double) -> double
  {
    double a     = -0.05;
    double absX  = std::abs (a_distance);
    double absX2 = absX * absX;
    double absX3 = absX2 * absX;

    if (absX <= 1.0)
      return ((a + 2.0) * absX3 - (a + 3.0) * absX2 + 1.0);
    else if (absX <= 2.0)
      return (a * absX3 - 5.0 * a * absX2 + 8.0 * a * absX - 4.0 * a);
    else
      return 0.0;
  };

  /* add color into QImage */
  auto addColor = [] (ColorData & a_dest, double red, double green, double blue, double alpha)
  {
    a_dest.red    += red;
    a_dest.green  += green;
    a_dest.blue   += blue;
    a_dest.alpha  += alpha;
  };

  auto calcWeight = transformMode == TransformationMode::LanczosTransformation
      ? calcLanczosWeight
      : calcBicubicInterpolationWeight;

  /* create images with desired format */
  DapImage source = (format() == QImage::Format_ARGB32)
                    ? *this
                    : DapImage (convertToFormat (QImage::Format_ARGB32));
  DapImage result (width, height, QImage::Format_ARGB32);

  /* clear result data */
  result.fill (0);

  /* keep aspect ratio */
  if (aspectRatioMode != Qt::IgnoreAspectRatio)
    result = result.scaled (result.size(), aspectRatioMode);

  /* get sizes */
  QSize sourceSize = source.size();
  QSize resultSize (result.width() - 1, result.height() - 1);

  /* get factors */
  double horizontalFactor  = double (sourceSize.width())  / double (resultSize.width());
  double verticalFactor    = double (sourceSize.height()) / double (resultSize.height());

  /* calc window size */
  double windowWidth         = SAMPLE_LENGTH * horizontalFactor;
  double windowHeight        = SAMPLE_LENGTH * verticalFactor;

  /* run thru result pixels */
  for (double y = 0; y < resultSize.height(); y++)
    {
      for (double x = 0; x < resultSize.width(); x++)
        {
          /* create window buffer */
          ColorData resultColor;

          /* calc position inside source image */
          double sourcePosX  = x * horizontalFactor;
          double sourcePosY  = y * verticalFactor;

          /* calc window boundaries */
          double winBeginX = qBound<double> (0.0, sourcePosX - windowWidth / 2,  sourceSize.width() - 1);
          double winBeginY = qBound<double> (0.0, sourcePosY - windowWidth / 2,  sourceSize.height() - 1);
          double winEndX   = winBeginX + windowWidth;//qBound<double> (0.0, winBeginX + windowWidth,       sourceSize.width());
          double winEndY   = winBeginY + windowHeight;//qBound<double> (0.0, winBeginY + windowHeight,      sourceSize.height());

          /* run thru source window */
          for (double filterY = floor (winBeginY); filterY < floor (winEndY); filterY++)
            {
              for (double filterX = floor (winBeginX); filterX < floor (winEndX); filterX++)
                {
                  /* calc distance and result weight */
                  double distanceX = qRound (sourcePosX) - filterX;
                  double distanceY = qRound (sourcePosY) - filterY;
                  double weight = calcWeight (distanceX, windowWidth)
                                * calcWeight (distanceY, windowHeight);

                  /* calc result pixel value */
                  const uchar *data = getPixelData(
                        source,
                        qBound<double> (0.0, filterX, sourceSize.width() - 1),
                        qBound<double> (0.0, filterY, sourceSize.height() - 1));
                  double red   = std::abs (double (data[0]));
                  double green = std::abs (double (data[1]));
                  double blue  = std::abs (double (data[2]));
                  double alpha = std::abs (double (data[3]));

                  /* add value to result image */
                  addColor (resultColor,
                            red   * weight,
                            green * weight,
                            blue  * weight,
                            alpha * weight);
                }
            }

          /* apply buffer result */
          uchar *resultData = getPixelData (result, x, y);
          resultData[0] = static_cast<uchar> (qBound (0, qRound (resultColor.red),   255));
          resultData[1] = static_cast<uchar> (qBound (0, qRound (resultColor.green), 255));
          resultData[2] = static_cast<uchar> (qBound (0, qRound (resultColor.blue),  255));
          resultData[3] = static_cast<uchar> (qBound (0, qRound (resultColor.alpha), 255));
        }
    }

  return result;
}

/********************************************
 * OPERATORS
 *******************************************/

DapImage &DapImage::operator= (const DapImage &a_src)
{
  QImage::operator= (a_src);
  return *this;
}

DapImage &DapImage::operator= (DapImage &&a_src)
{
  QImage::operator= (std::move (a_src));
  return *this;
}

DapImage &DapImage::operator= (const QImage &a_src)
{
  QImage::operator= (a_src);
  return *this;
}

DapImage &DapImage::operator= (QImage &&a_src)
{
  QImage::operator= (std::move (a_src));
  return *this;
}

/*-----------------------------------------*/
