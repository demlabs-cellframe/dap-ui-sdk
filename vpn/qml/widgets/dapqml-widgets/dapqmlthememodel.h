#ifndef DAPQMLTHEMEMODEL_H
#define DAPQMLTHEMEMODEL_H

/* INCLUDES */
#include <QAbstractTableModel>

/****************************************//**
 * @brief themes model list
 * @ingroup groupUiModels
 * @date 24.01.2022
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlThemeModel : public QAbstractTableModel
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QStringList m_themes;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlThemeModel(QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  Q_INVOKABLE QString currentCategory() const;
  Q_INVOKABLE bool isCurrent (const QString &a_name) const;
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
};

/*-----------------------------------------*/
#endif // DAPQMLTHEMEMODEL_H
