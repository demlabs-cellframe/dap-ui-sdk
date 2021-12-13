#ifndef POPUPDIALOG_H
#define POPUPDIALOG_H

/* INCLUDES */
#include <QWidget>

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class PopupDialog; }
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for reset licence
 *
 * Flexible popup dialog.
 *
 * Only two answers: Yes and No
 *
 * @ingroup groupUiClasses
 * @date 18.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class PopupDialog : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::PopupDialog *ui;
  QWidget *m_parent, *m_shadow;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit PopupDialog(QWidget *parent = nullptr);
  ~PopupDialog();
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
  /**
   * @brief show dialog
   * @param parent[in] - widget that will hold this dialog on top. !must have parent widget!
   * @param a_title[in] - title text
   * @param a_description[in] - description text
   */
  void slotShow (
      QWidget *a_parent,
      const QString &a_title,
      const QString &a_description,
      const QString &a_btnYes,
      const QString &a_btnNo);

protected slots:
  void _slotButtonPress();
  /// @}
};

/*-----------------------------------------*/
#endif // POPUPDIALOG_H
