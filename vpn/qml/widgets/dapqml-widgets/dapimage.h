#ifndef DAPIMAGE_H
#define DAPIMAGE_H

/* INCLUDES */
#include <QImage>

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
  enum TransformationMode
  {
    FastTransformation,
    SmoothTransformation,
    LanczosTransformation,
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

/*-----------------------------------------*/
#endif // DAPIMAGE_H
