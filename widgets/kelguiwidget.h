#ifndef KELGUIWIDGET_H
#define KELGUIWIDGET_H

/* INCLUDES */
#include <QWidget>

/****************************************//**
 * @brief widget with custom properties
 * @ingroup groupKelGuiWidgets
 * @date 30.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiWidget : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (bool frame READ frame WRITE setFrame)
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  bool m_frame; ///< show frame
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit KelGuiWidget(QWidget *parent = nullptr);
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:
  bool frame() const;
  void setFrame(bool frame);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  void paintEvent (QPaintEvent *) override;
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUIWIDGET_H
