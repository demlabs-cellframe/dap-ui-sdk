#ifndef DOCUMENTPOPUP_H
#define DOCUMENTPOPUP_H

/* INCLUDES */
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include "baseform.h"
#include "dapguilabel.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class DocumentPopup; }
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for displaying documents
 *
 * Display's documents
 *
 * Features:
 * - Touch scrolling
 * - Flexible document integration
 *
 * @ingroup groupUiClasses
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class DocumentPopup : public BaseForm
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
  Q_ENUM (Type)

protected:
  class Filter : public QScrollArea { public: bool eventFilter(QObject *o, QEvent *e) override; };
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::DocumentPopup *ui;
  DapGuiLabel *m_label;
  QVBoxLayout *m_layout;
  Filter *m_filter;
  Type m_type;
  bool m_updated;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DocumentPopup (Type t, QWidget *parent = nullptr);
  ~DocumentPopup();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigReturn();
  /// @}

  /****************************************//**
   * @name PUBLIC SLOTS
   *******************************************/
  /// @{
public slots:
  void slotShowTermsOfUse();
  void slotShowPrivacyPolicy();
  void slotRetranslated();
  void slotUpdateContents();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  bool eventFilter(QObject *o, QEvent *e) override;
  /// @}
};

/*-----------------------------------------*/
#endif // DOCUMENTPOPUP_H
