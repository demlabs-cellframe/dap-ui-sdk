#ifndef IMAGESCALINGTHREADPOOL_H
#define IMAGESCALINGTHREADPOOL_H

/* INCLUDES */

#include <QObject>

#include "../dapimage.h"

/****************************************//**
 * @brief QML image scaling thread pool
 *
 * Used to scale assets inside thread pool
 * and deliver results to DapQmlImage widgets
 *
 * @ingroup groupDapGuiStyle
 * @date 18.01.2024
 * @author Mikhail Shilenko
 *******************************************/

class ImageScalingThreadPool : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  explicit ImageScalingThreadPool();
  ~ImageScalingThreadPool();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static ImageScalingThreadPool *instance();

  void queueScaling (const QString &a_filename, const QSize &a_newSize);
  void requestResult (const QString &a_filename, const QSize &a_newSize, DapImage &a_dest);
  static void performScaling (const QString &a_filename, const QSize &a_newSize, DapImage &a_dest);

  static bool isActive();
  static void setActive (bool a_active);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  /// @brief all scaling operations are finished
  void sigFinished();

  void _sigStartQueueTimer();
  void _sigStartClearTimer();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  /// @brief thread finished scaling
  void slotScalingResult (const QString &a_filename, const QSize &a_newSize, const DapImage &a_result);
  /// @}
};

/****************************************//**
 * @brief scaling worker
 * @date 18.01.2024
 * @author Mikhail Shilenko
 *******************************************/

class ImageScalingWorker : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  ImageScalingWorker();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotProcess (const QString &a_filename, const QSize &a_newSize);
  /// @}
};

/*-----------------------------------------*/
#endif // IMAGESCALINGTHREADPOOL_H
