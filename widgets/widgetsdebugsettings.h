#ifndef WIDGETSDEBUGSETTINGS_H
#define WIDGETSDEBUGSETTINGS_H

/* INCLUDES */
#include <QWidget>

/* DEFS */
namespace Ui { class WidgetsDebugSettings; }

/****************************************//**
 * @brief widget settings for QtDesigner
 * @date 14.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class WidgetsDebugSettings : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::WidgetsDebugSettings *ui;
  QString m_globalStyleFilename;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit WidgetsDebugSettings (QWidget *parent = nullptr);
  ~WidgetsDebugSettings() override;
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  void closeEvent (QCloseEvent *event) override;
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
private slots:
  void on_btnChooseGlobalStyle_clicked();
  void on_btnUpdateGlobalStyle_clicked();
  /// @}
};

/*-----------------------------------------*/
#endif // WIDGETSDEBUGSETTINGS_H
