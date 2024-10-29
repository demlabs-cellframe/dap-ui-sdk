#ifndef DAPGUILABEL_H
#define DAPGUILABEL_H

/* INCLUDES */
#include <QLabel>
#include "style/dapguistylemanager.h"

/****************************************//**
 * @brief overlap for label style
 *
 * Features:
 * - Css Style
 * - Mouse Button Press signal
 *
 * @ingroup groupDapGuiWidgets
 * @date 30.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapGuiLabel : public QLabel
{
  Q_OBJECT
  DAPGUI_ENABLECSS

  Q_PROPERTY(QString scaledPixmap READ scaledPixmap WRITE setScaledPixmap NOTIFY scaledPixmapChanged)
  Q_PROPERTY(QString scaledPixmapEx READ scaledPixmapEx WRITE setScaledPixmapEx NOTIFY setScaledPixmapExChanged)

  bool _cssHook, _hovered, _pressed;
  QString m_scaledPixmap;

  struct {
    QString idle;
    QString hovered;
    QString pressed;
  } m_scaledPixmapEx;

  struct
  {
      QPixmap pixmap;
      QSize size;
  } _cache;

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapGuiLabel (QWidget *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  QString scaledPixmap() const;
  void setScaledPixmap (const QString &scaledPixmap);
  QString scaledPixmapEx() const;
  void setScaledPixmapEx (const QString &scaledPixmapEx);
protected:
  void _calculateNewPixmapState();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  void mousePressEvent (QMouseEvent *) override;
  void mouseReleaseEvent (QMouseEvent *) override;
  void enterEvent (QEvent *) override;
  void leaveEvent (QEvent *) override;
  void paintEvent(QPaintEvent *) override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void clicked();         ///< button is clicked
  void scaledPixmapChanged();
  void setScaledPixmapExChanged();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
protected slots:
  void _slotCssStyleUpdate();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPGUILABEL_H
