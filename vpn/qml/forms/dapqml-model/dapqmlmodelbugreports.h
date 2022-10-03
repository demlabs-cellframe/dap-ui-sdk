#ifndef DAPQMLMODELBUGREPORTS_H
#define DAPQMLMODELBUGREPORTS_H

/* INCLUDES */
#include <QAbstractTableModel>

/****************************************//**
 * @brief bug reports model list
 * @ingroup groupUiModels
 * @date 09.03.2022
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelBugReports : public QAbstractTableModel
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlModelBugReports(QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapQmlModelBugReports *instance();
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
#endif // DAPQMLMODELBUGREPORTS_H
