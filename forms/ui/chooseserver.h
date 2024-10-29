#ifndef CHOOSESERVER_H
#define CHOOSESERVER_H

/* INCLUDES */
#include <QWidget>
#include "baseform.h"

/* DEFS */
QT_BEGIN_NAMESPACE
namespace Ui { class ChooseServer; }
QT_END_NAMESPACE

class ChooseServerModel;

/****************************************//**
 * @brief ui/class for server list
 *
 * Container for server list.
 *
 * Used ChooseServerModel to control contents
 *
 * Sends sigSelect signal on server selection
 *
 * @ingroup groupUiClasses
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class ChooseServer : public BaseForm
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::ChooseServer *ui;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit ChooseServer (QWidget *parent = nullptr);
  ~ChooseServer();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  ChooseServerModel *model();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigReturn();
  void sigSelect(int index, QString name);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotRetranslated();
  void setSelectedServer(QString name);
  /// @}
};

/*-----------------------------------------*/
#endif // CHOOSESERVER_H
