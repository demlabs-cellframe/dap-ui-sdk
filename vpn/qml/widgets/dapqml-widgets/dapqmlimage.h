#ifndef DAPQMLIMAGE_H
#define DAPQMLIMAGE_H

/* INCLUDES */
#include "dapimage.h"

#include <QQuickPaintedItem>
#include <QPixmap>

/****************************************//**
 * @brief QML image
 *
 * Used to scale and cache image with
 * higher quality.
 *
 * @ingroup groupDapGuiStyle
 * @date 10.06.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlImageItem : public QQuickPaintedItem
{
  Q_OBJECT

  friend class DapQmlImageItemProcessWorker;

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{

  /**
   * @brief QML item image source
   * @accessors scaledPixmap(), setScaledPixmap()
   */
  Q_PROPERTY (QString scaledPixmap READ scaledPixmap WRITE setScaledPixmap NOTIFY sigScaledPixmapChanged)

  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  /// QML item image source
  QString m_scaledPixmap;

  /// scaled image cache
  struct
  {
    QString name;
    DapImage image;
    QSizeF size;
    QMetaObject::Connection conn;
  } _cache;

  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlImageItem (QQuickItem *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static void initWorkers();

  QString scaledPixmap() const;
  void setScaledPixmap (const QString &a_scaledPixmap);

  static void setDevicePixelRatio (float a_value);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigScaledPixmapChanged();
  void _sigRedraw();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
protected slots:
  void _slotRedraw();
  void _slotScalingFinished();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  void paint (QPainter *a_painter) override;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLIMAGE_H
