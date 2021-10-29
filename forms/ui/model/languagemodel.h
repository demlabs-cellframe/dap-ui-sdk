#ifndef LANGUAGEMODEL_H
#define LANGUAGEMODEL_H

/* INCLUDES */
#include <QScrollArea>
#include <QBoxLayout>
#include <QAbstractListModel>
#include <dapguiradio.h>
#include "modelbase.h"

/****************************************//**
 * @brief choose language model list widget
 * @ingroup groupUiModels
 * @date 01.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class LanguageModel : public ModelBase
{
  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  QList<DapGuiRadio*> m_list;
  QAbstractListModel *m_model;
  int m_currentIndex;
  QString m_currentText;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit LanguageModel (QWidget *parent = nullptr);
  ~LanguageModel();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigSelect(int index, QString name);
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:
  /// set model into this widget
  /// @note old model will not be deleted
  void setModel (QAbstractListModel *model);
  QAbstractListModel *model() const;
  /// @}

  /****************************************//**
   * @name PUBLIC SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetup();
private slots:
  /// radio button toggle
  void slotToggled (bool checked);
  /// @}
};

/*-----------------------------------------*/
#endif // LANGUAGEMODEL_H
