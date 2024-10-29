#ifndef DAPQMLMODELCHOOSECERT_H
#define DAPQMLMODELCHOOSECERT_H

/* INCLUDES */
#include <QAbstractTableModel>

/****************************************//**
 * @brief certificates model list
 * @ingroup groupUiModels
 * @date 25.09.2022
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelChooseCert : public QAbstractTableModel
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapQmlModelChooseCert(QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  int columnCount (const QModelIndex &parent = QModelIndex()) const override;

  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetup();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELCHOOSECERT_H
