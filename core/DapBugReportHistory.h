#ifndef DAPBUGREPORTHISTORY_H
#define DAPBUGREPORTHISTORY_H

/* INCLUDES */
#include <QObject>
#include <QMap>

/* DEFS */
struct DapBugReportHistoryItem
{
  int number;
  QString status;
  QString asString() const { return QString().asprintf ("%05d", number); }
};

/****************************************//**
 * @brief bug reports history
 * @date 17.11.2022
 * @author Mikhail Shilenko
 *******************************************/

class DapBugReportHistory : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  QMap<int, DapBugReportHistoryItem> m_items;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapBugReportHistory (QObject *a_parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  const QList<DapBugReportHistoryItem> &list() const;
  int size() const;
  bool isEmpty() const { return size() == 0; }
  DapBugReportHistoryItem &value (int a_id);
  const DapBugReportHistoryItem &value (int a_id) const;
  void remove (int a_id, int a_index);
  void load();
  void save();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigBugReportListLoaded();
  void sigUpdateReportsStatusError();
  void sigUpdateReportsStatusSuccess();
  void sigNewReport (int a_index);
  void sigRemoved (int a_index);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotUpdateReportsStatus (const QString &a_json);
  void slotNewReport (const QString &a_reportNumber);
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  DapBugReportHistoryItem &operator[] (int a_id)               { return value (a_id); }
  const DapBugReportHistoryItem &operator[] (int a_id) const   { return value (a_id); }
  /// @}
};

/*-----------------------------------------*/
#endif // DAPBUGREPORTHISTORY_H
