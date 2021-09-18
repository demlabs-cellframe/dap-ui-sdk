#ifndef RESETKEYDIALOG_H
#define RESETKEYDIALOG_H

/* INCLUDES */
#include <QWidget>

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class ResetKeyDialog; }
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for reset licence
 * @ingroup groupUiClasses
 * @date 18.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class ResetKeyDialog : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::ResetKeyDialog *ui;
  QWidget *m_parent, *m_shadow;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit ResetKeyDialog(QWidget *parent = nullptr);
  ~ResetKeyDialog();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigYes();
  void sigNo();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotShow(QWidget *parent);
  void slotButtonPress();
  /// @}
};

/*-----------------------------------------*/
#endif // RESETKEYDIALOG_H
