#ifndef DAPSERIALKEYHISTORY_H
#define DAPSERIALKEYHISTORY_H

/* INCLUDES */
#include <QObject>

/* DEFS */
typedef QString DapSerialKeyHistoryItem;

/****************************************//**
 * @brief serial key history
 * @date 17.11.2022
 * @author Mikhail Shilenko
 *******************************************/

class DapSerialKeyHistory : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  QList<DapSerialKeyHistoryItem> m_list;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapSerialKeyHistory(QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  const QList<DapSerialKeyHistoryItem> &list() const;
  int size() const;
  DapSerialKeyHistoryItem &value (int a_index);
  const DapSerialKeyHistoryItem &value (int a_index) const;
  void load();
  void save();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigNewKey();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotNewKey();
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  DapSerialKeyHistoryItem &operator[] (int a_index)             { return value (a_index); }
  const DapSerialKeyHistoryItem &operator[] (int a_index) const { return value (a_index); }
  /// @}
};

/*-----------------------------------------*/
#endif // DAPSERIALKEYHISTORY_H
