#ifndef SERIALINPUTFIELD_H
#define SERIALINPUTFIELD_H

/* INCLUDES */
#include <QWidget>
#include <QVariant>

/****************************************//**
 * @brief custom input fiels for serial key
 * @date 16.10.2021
 * @author Mikhail Shilenko
 *******************************************/

class SerialInputField : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QWidget *m_child;
  QString m_text, m_temp;
  int m_cursor;
  bool m_focused;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit SerialInputField(QWidget *parent = nullptr);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void cursorPositionChanged(int oldPos, int newPos);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
protected slots:
  void slotCursorPositionChanged(int oldPos, int newPos);
  void slotFixString();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
protected:
  void mousePressEvent(QMouseEvent *e) override;
  void keyPressEvent (QKeyEvent *e) override;
  void inputMethodEvent (QInputMethodEvent *e) override;
  void focusInEvent (QFocusEvent *e) override;
  void focusOutEvent (QFocusEvent *e) override;
  void paintEvent(QPaintEvent *) override;
public:
  QVariant inputMethodQuery (Qt::InputMethodQuery a_imq) const override;
  Q_INVOKABLE QVariant inputMethodQuery(Qt::InputMethodQuery property, QVariant argument) const;
  /// @}
};

/*-----------------------------------------*/
#endif // SERIALINPUTFIELD_H
