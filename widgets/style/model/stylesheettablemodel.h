#ifndef STYLESHEETTABLEMODEL_H
#define STYLESHEETTABLEMODEL_H

/* INCLUDES */
#include <QAbstractItemModel>

/****************************************//**
 * @brief style table model for QtDesigner
 * @date 27.11.2021
 * @author Mikhail Shilenko
 *******************************************/

class StyleSheetTableModel : public QAbstractTableModel
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  StyleSheetTableModel();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int columnCount(const QModelIndex &parent) const override;
  int rowCount(const QModelIndex &parent) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
protected slots:
  void forcedStyleUpdate();
  /// @}
};

/*-----------------------------------------*/
#endif // STYLESHEETTABLEMODEL_H
