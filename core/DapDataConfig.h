#ifndef DAPDATACONFIG_H
#define DAPDATACONFIG_H

/* INCLUDES */

#include <QObject>
#include <QSet>
#include <QVariantMap>
#include <QJsonObject>

/****************************************//**
 * @brief Dap Config Data
 * @note side is used to figure which instance applyed changes
 * @date 02.10.2024
 * @author Mikhail Shilenko
 *******************************************/

class DapDataConfig : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name VARIABLES
   *******************************************/
  /// @{
public:
  QVariantMap _data;
  QSet<QString> _changed;
  QSet<QString> _removed;
  QString _filename;

  int m_msgCounter;
  QList<int> _receivedMessagesIds;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapDataConfig (const QString &a_filename);
  virtual ~DapDataConfig();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  int msgCounter() const;
  void setMsgCounter (const int a_id);
  QVariant value (const QString &a_name) const;
  bool setValue (const QString &a_name, const QVariant &a_value, const int a_msgId = 0);
  bool setValue (const QString &a_name, QVariant &&a_value, const int a_msgId = 0);
  bool remove (const QString &a_name, const int a_msgId = 0);
  virtual void sync() = 0;

  const QSet<QString> &changed() const  { return _changed; }
  const QSet<QString> &removed() const  { return _removed; }
  const QString &filename() const       { return _filename; }

  QJsonObject toJson() const;
  void fromJson (const QJsonObject &a_json);
protected:
  bool _isUniqueMessageId (int a_msgId);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigValueUpdated (QString a_name, QVariant a_value, const int a_msgId);
  void sigValueRemoved (QString a_name, const int a_msgId);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPDATACONFIG_H
