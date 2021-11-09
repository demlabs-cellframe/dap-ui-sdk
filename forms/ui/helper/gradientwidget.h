#ifndef GRADIENTWIDGET_H
#define GRADIENTWIDGET_H

/* INCLUDES */
#include <QWidget>

/****************************************//**
 * @brief gradient background widget
 * @date 04.10.2021
 * @author Mikhail Shilenko
 *******************************************/

class GradientWidget : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
  QLinearGradient m_gradient;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit GradientWidget(QWidget *parent = nullptr);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
protected:
  void paintEvent(QPaintEvent *) override;
  /// @}
};

/*-----------------------------------------*/
#endif // GRADIENTWIDGET_H
