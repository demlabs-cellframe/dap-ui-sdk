#ifndef DAPNOTIFICATIONHISTORY_H
#define DAPNOTIFICATIONHISTORY_H

/* INCLUDES */
#include "DapNotification.h"

#include <QAbstractListModel>
#include <QVector>
#include <QDateTime>

/****************************************//**
 * @brief DapNotificationHistory class
 * @date 28.09.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapNotificationHistory : public QAbstractListModel
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum class FieldId
  {
    invalid,

    created,
    message,
    type,
    isTitle,
    titleDate,
    typeString,
    createdTime,

    size,
  };

  struct Item
  {
    bool isTitle;
    QDate titleDate;

    DapNotification notification;
  };

  /// @}

  /****************************************//**
   * @name VARIABLES
   *******************************************/
  /// @{
private:
  QVector<Item> m_list;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  DapNotificationHistory (QObject *parent = nullptr);
  ~DapNotificationHistory();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapNotificationHistory *instance();

  /* list */

  void append (const QString &a_message, DapNotification::Type a_type);
  void clear();
  static const QHash<int, QByteArray> &fields();

  /* storage */

  void load();
  void save();
  void removeOutdated();

protected:
  void _removeOutdated (bool a_resetModel = true);
  void _delayedSave();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
protected:
  QHash<int, QByteArray> roleNames() const override;
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPNOTIFICATIONHISTORY_H
