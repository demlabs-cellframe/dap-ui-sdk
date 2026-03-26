#ifndef DAPIMAGE_H
#define DAPIMAGE_H

/* INCLUDES */
#include <QImage>
#include <QMetaType>

/****************************************//**
 * @brief Dap image
 *
 * Better quality image scaling
 *
 * @ingroup groupDapGuiStyle
 * @date 26.06.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapImage : public QImage
{
  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  /// scaling methods
  enum TransformationMode
  {
    FastTransformation,     ///< build-in qt fast method
    SmoothTransformation,   ///< build-in qt smooth method
    BicubicTransformation,  ///< bicubic interpolation method
    LanczosTransformation,  ///< lanczos method
  };
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapImage();
  DapImage (const DapImage &a_src);
  DapImage (DapImage &&a_src);
  DapImage (const QImage &a_src);
  DapImage (QImage &&a_src);
  DapImage (const QSize &size, Format format);
  DapImage (int width, int height, Format format);
  DapImage (uchar *data, int width, int height, Format format, QImageCleanupFunction cleanupFunction = nullptr, void *cleanupInfo = nullptr);
  DapImage (const uchar *data, int width, int height, Format format, QImageCleanupFunction cleanupFunction = nullptr, void *cleanupInfo = nullptr);
  DapImage (uchar *data, int width, int height, int bytesPerLine, Format format, QImageCleanupFunction cleanupFunction = nullptr, void *cleanupInfo = nullptr);
  DapImage (const uchar *data, int width, int height, int bytesPerLine, Format format, QImageCleanupFunction cleanupFunction = nullptr, void *cleanupInfo = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static void init();
  DapImage scaled (const QSize &size, Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio, TransformationMode transformMode = LanczosTransformation) const;
  DapImage scaled (int width, int height, Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio, TransformationMode transformMode = LanczosTransformation) const;
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  DapImage &operator= (const DapImage &a_src);
  DapImage &operator= (DapImage &&a_src);
  DapImage &operator= (const QImage &a_src);
  DapImage &operator= (QImage &&a_src);
  /// @}
};

Q_DECLARE_METATYPE (DapImage)

/*-----------------------------------------*/
#endif // DAPIMAGE_H
