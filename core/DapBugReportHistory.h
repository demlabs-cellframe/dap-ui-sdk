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
  DapBugReportHistoryItem &value (int a_index);
  const DapBugReportHistoryItem &value (int a_index) const;
  void remove (int a_index);
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
  void sigNewReport();
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
  DapBugReportHistoryItem &operator[] (int a_index)               { return value (a_index); }
  const DapBugReportHistoryItem &operator[] (int a_index) const   { return value (a_index); }
  /// @}
};

/*-----------------------------------------*/
#endif // DAPBUGREPORTHISTORY_H
