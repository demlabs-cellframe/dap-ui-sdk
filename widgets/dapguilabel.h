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

  Q_PROPERTY(QString scaledPixmap READ scaledPixmap WRITE setScaledPixmap)

  bool _cssHook;
  QString m_scaledPixmap;

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
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  void mousePressEvent (QMouseEvent *) override;
  void paintEvent(QPaintEvent *) override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void clicked();         ///< button is clicked
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
