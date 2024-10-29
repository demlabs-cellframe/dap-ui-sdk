#ifndef MODELBASE_H
#define MODELBASE_H

/* INCLUDES */
#include <QScrollArea>
#include <QBoxLayout>

/****************************************//**
 * @brief base class for model
 * inside scrollarea
 * @ingroup groupUiModels
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class ModelBase : public QScrollArea
{
  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QVBoxLayout *lay;
  //// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit ModelBase (QWidget *parent = nullptr);
  /// @}

  /****************************************//**
   * @name PROTECTED METHODS
   *******************************************/
  /// @{
protected:
  void setupLayout();
  void clearLayout(QLayout *layout);
  /// @}
};

/*-----------------------------------------*/
#endif // MODELBASE_H
