#ifndef BUGREPORTS_H
#define BUGREPORTS_H

/* INCLUDES */
#include <QWidget>
#include <QMap>
#include <QMovie>
#include <QPlainTextEdit>
#include "baseform.h"
#include "dapguiplaintexteditinterface.h"
#include "DapDataLocal.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class BugReports; }
QT_END_NAMESPACE

/****************************************//**
 * @brief ui/class for bug reports
 * @ingroup groupUiClasses
 * @date 27.08.2021
 * @author Mikhail Shilenko
 *******************************************/

class BugReports : public BaseForm
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum Mode
  {
    List,
    Write,
    Loading,
    Result,
  };
  Q_ENUM (Mode);
  /// @}

  static const QString SCREEN_NAME;

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  /// gui
  Ui::BugReports *ui;
  QPlainTextEdit *m_edit;
  /// animation
  QMovie *movLoading;
  /// current mode
  Mode m_mode;
  /// showing/hiding map
  QMultiMap<Mode, QWidget *> m_map;
  /// testing radio map @see slotRadioTest
  QMap<QObject *, Mode> p_radioTestToMode;
  bool _textHook, _spacer;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit BugReports (QWidget *parent = nullptr);
  ~BugReports();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static BugReports *instance();
  QString text() const;
  void setText (const QString &a_text);
  void setResultText (const QString &a_result);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigReturn();
  void sigSend();
  void sigResultBack();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetMode (Mode mode);
  void refreshHistoryList();
private slots:
  void _slotRadioTest();
  void _slotTextChanged();
  void _slotTextEditFinish();
private:
  static bool _cbTextEdit (DapGuiPlainTextEditInterface *e, QString &preedit, QString &commit, int from, int to);
  void updateData (QString &a_text, int a_len);
  /// @}
};

/*-----------------------------------------*/
#endif // BUGREPORTS_H
