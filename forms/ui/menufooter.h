#ifndef MENUFOOTER_H
#define MENUFOOTER_H

/* INCLUDES */
#include <QWidget>
#include <QMap>

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class MenuFooter;}
QT_END_NAMESPACE

/****************************************//**
 * @brief menu footer widget
 * usualy lays on top of screen
 * @date 27.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class MenuFooter : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum ButtonState
  {
    BS_NONE,
    BS_SETTINGS,
    BS_CONNECTION,
    BS_ACCAUNT,
  };
  Q_ENUM(ButtonState)
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::MenuFooter *ui;
  ButtonState m_state;
  QMap<QObject*,ButtonState> m_statesMap;
  bool m_lock; ///< prevent recursion in slotButtonToggled
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit MenuFooter(QWidget *parent = nullptr);
  ~MenuFooter();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  /// set progressbar value
  void slotSetProgressValue(int value);
  /// set progressbar range
  void slotSetProgressRange(int minimum, int maximum);
  /// change buttons states
  void slotSetButtonState(ButtonState state);
  /// fix position
  void slotMoveToBottom();

private slots:
  /// react on user interaction
  void slotButtonToggled(bool checked);
  /// @}
};

/*-----------------------------------------*/
#endif // MENUFOOTER_H
