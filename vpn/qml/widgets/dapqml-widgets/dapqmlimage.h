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
  static void initWorkers (void (*a_workersDoneCallback)());

  QString scaledPixmap() const;
  void setScaledPixmap (const QString &a_scaledPixmap);
protected:
  void _setupImage (DapImage &&a_image, const QSizeF &a_size);
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
  void slotRedraw();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  void paint (QPainter *a_painter) override;
  /// @}
};

class DapQmlImageItemProcessWorker : public QObject
{
  Q_OBJECT

public:
  DapQmlImageItemProcessWorker();

public slots:
  void slotProcess();
};

/*-----------------------------------------*/
#endif // DAPQMLIMAGE_H
