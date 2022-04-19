#ifndef DAPQMLWINDOW_H
#define DAPQMLWINDOW_H

/* INCLUDES */
#include <QQuickWindow>

/****************************************//**
 * @brief application window
 * @ingroup groupDapGuiStyle
 * @date 15.04.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlWindow : public QQuickWindow
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapQmlWindow();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void resized (int a_width, int a_height);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
private slots:
  void _resized();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLWINDOW_H
