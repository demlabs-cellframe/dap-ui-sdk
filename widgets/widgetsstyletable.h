#ifndef WIDGETSSTYLETABLE_H
#define WIDGETSSTYLETABLE_H

/* INCLUDES */
#include <QWidget>
#include "style/model/stylesheettablemodel.h"

/* DEFS */
namespace Ui { class WidgetsStyleTable; }

/****************************************//**
 * @brief style profiler for QtDesigner
 * @date 27.11.2021
 * @author Mikhail Shilenko
 *******************************************/

class WidgetsStyleTable : public QWidget
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  Ui::WidgetsStyleTable *ui;
  StyleSheetTableModel *m_tableModel;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit WidgetsStyleTable(QWidget *parent = nullptr);
  ~WidgetsStyleTable();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
private slots:
  void slotDoubleClicked (const QModelIndex &index);
  /// @}
};

/*-----------------------------------------*/
#endif // WIDGETSSTYLETABLE_H
