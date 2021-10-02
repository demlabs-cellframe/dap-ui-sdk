#ifndef SERIALINPUT_H
#define SERIALINPUT_H

/* INCLUDES */
#include <QWidget>
#include <QMap>

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class SerialInput; }
QT_END_NAMESPACE

class KelGuiLineEdit;

/****************************************//**
 * @brief ui/class for serial input
 * @ingroup groupUiClasses
 * @date 01.10.2021
 * @author Mikhail Shilenko
 *******************************************/

class SerialInput : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::SerialInput *ui;
  KelGuiLineEdit *m_input;
  bool m_textChangeHook;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit SerialInput (QWidget *parent = nullptr);
  ~SerialInput();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  QString serialKey() const;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigReturn();
  void sigConfirm();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public:
  void slotTextChanged();
  void slotSetSerial (const QString &a_serial);
  /// @}
};

/*-----------------------------------------*/
#endif // SERIALINPUT_H
