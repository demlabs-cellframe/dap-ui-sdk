#ifndef DAPQMLMODELFAQ_H
#define DAPQMLMODELFAQ_H

/* INCLUDES */
#include <QAbstractTableModel>

/****************************************//**
 * @brief faq model
 * @ingroup groupUiModels
 * @date 12.02.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelFaq : public QAbstractListModel
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  explicit DapQmlModelFaq();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapQmlModelFaq *instance();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;

  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELFAQ_H
