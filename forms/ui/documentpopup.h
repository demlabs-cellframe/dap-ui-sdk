#ifndef DOCUMENTPOPUP_H
#define DOCUMENTPOPUP_H

/* INCLUDES */
#include <QWidget>

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class DocumentPopup; }
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for displaying documents
 * @ingroup groupUiClasses
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class DocumentPopup : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum Type
  {
    TermsOfUse,
    PrivacyPolicy,
  };
  Q_ENUM(Type)
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::DocumentPopup *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DocumentPopup(Type t, QWidget *parent = nullptr);
  ~DocumentPopup();
  /// @}

  /****************************************//**
   * @name PUBLIC SLOTS
   *******************************************/
  /// @{
public slots:
  void slotShowTermsOfUse();
  void slotShowPrivacyPolicy();
  /// @}
};

/*-----------------------------------------*/
#endif // DOCUMENTPOPUP_H
