#ifndef DAPNOTIFICATION_H
#define DAPNOTIFICATION_H

/* INCLUDES */
#include <QObject>
#include <QJsonObject>
#include <QDateTime>

/****************************************//**
 * @brief DapNotification class
 * @date 28.09.2023
 * @author Mikhail Shilenko
 *******************************************/
class DapNotification : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum class Type
  {
    Invalid,
    Error,
    Notification,
    Warning
  };

  enum class FieldId
  {
    invalid,

    created,
    message,
    type,

    SIZE,
  };

  /// @}

  /****************************************//**
   * @name VARIABLES
   *******************************************/
  /// @{
private:
  QDateTime m_created;
  QString m_message;
  Type m_type;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapNotification (QObject *parent = nullptr);
  DapNotification (const DapNotification &a_source);
  DapNotification (DapNotification &&a_source) noexcept;
  DapNotification(
    const QDateTime &a_created,
    const QString &a_message,
    Type a_type);
  ~DapNotification();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:

  /* properties */

  const QDateTime &created() const;
  QString createdString() const;
  QString createdStringAM() const;
  void setCreated (const QDateTime &a_value);
  void setCreated (const QString &a_value);

  const QString &message() const;
  void setMessage (const QString &a_value);

  Type type() const;
  void setType (Type a_value);

  QVariant value (const char *a_name) const;
  QVariant value (FieldId a_fid) const;
  void setValue (const char *a_name, const QVariant &a_value);
  void setValue (FieldId a_fid, const QVariant &a_value);

  static FieldId fieldId (const char *a_name);
  static const QHash<int, QByteArray> &fields();

  /* json */

  static DapNotification fromJson (const QJsonObject &a_source);
  QJsonObject toJson() const;
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  DapNotification &operator= (const DapNotification &a_source);
  DapNotification &operator= (DapNotification &&a_source) noexcept;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPNOTIFICATION_H
