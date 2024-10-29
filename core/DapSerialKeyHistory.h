#ifndef DAPSERIALKEYHISTORY_H
#define DAPSERIALKEYHISTORY_H

/* INCLUDES */
#include <QObject>

/****************************************//**
 * @brief serial key history
 * @date 17.11.2022
 * @author Mikhail Shilenko
 *******************************************/

class DapSerialKeyHistory : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
private:
  class DapSerialKeyHistoryData;
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  DapSerialKeyHistoryData *_d;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapSerialKeyHistory (QObject *parent = nullptr);
  ~DapSerialKeyHistory();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  const QStringList &list() const;
  int size() const;
  QString value (int a_index) const;
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
  QString operator[] (int a_index) const { return value (a_index); }
  /// @}
};

/*-----------------------------------------*/
#endif // DAPSERIALKEYHISTORY_H
