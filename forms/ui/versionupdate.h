#ifndef VERSIONUPDATE_H
#define VERSIONUPDATE_H

#include <QWidget>

namespace Ui {
  class VersionUpdate;
}

/****************************************//**
 * @brief ui/class for update install
 * @ingroup groupUiClasses
 * @date 27.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class VersionUpdate : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::VersionUpdate *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit VersionUpdate(QWidget *parent = nullptr);
  ~VersionUpdate();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigReturn();
  void sigUpdate();
  /// @}

};

/*-----------------------------------------*/
#endif // VERSIONUPDATE_H
