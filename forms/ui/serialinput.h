#ifndef SERIALINPUT_H
#define SERIALINPUT_H

/* INCLUDES */
#include <QWidget>
#include <QMap>

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class SerialInput; }
QT_END_NAMESPACE

class DapGuiLineEdit;

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
  DapGuiLineEdit *m_input;
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
private:
  void fixSerialKey (QString &a_serial) const;
  static void cbFocusEvent (DapGuiLineEdit *e, const Qt::FocusReason &reason);
  static void cbTextEdit (DapGuiLineEdit *e, QString &preedit, QString &commit, int from, int to);
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
  void slotSetSerial (const QString &a_serial);
  void slotActiavted();
  /// @}
};

/*-----------------------------------------*/
#endif // SERIALINPUT_H
