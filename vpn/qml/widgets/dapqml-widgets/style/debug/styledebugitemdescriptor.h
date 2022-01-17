#ifndef STYLEDEBUGITEMDESCRIPTOR_H
#define STYLEDEBUGITEMDESCRIPTOR_H

/* INCLUDES */
#include <QString>
#include <QObject>

/****************************************//**
 * @brief style debug tree item descriptor
 *
 * Keeps information about item and
 * provides values to a StyleDebugTree
 * @ingroup groupDapGuiStyle
 * @date 17.01.2022
 * @author Mikhail Shilenko
 *******************************************/

class StyleDebugItemDescriptor : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  using Line = QPair<QString, QString>;
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  /* INFORMATION */
  QString m_name;
  QStringList m_fields;
  QObject *m_item;

  /* CACHED STORAGE */
  QList<Line> m_fieldData;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  StyleDebugItemDescriptor();
  StyleDebugItemDescriptor (QString a_name, QStringList a_fields, QObject *a_item);
  StyleDebugItemDescriptor (const StyleDebugItemDescriptor &a_src);
  StyleDebugItemDescriptor (StyleDebugItemDescriptor &&a_src);
  ~StyleDebugItemDescriptor();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  Q_INVOKABLE const QString &name() const;
  Q_INVOKABLE void setName (const QString &newName);

  Q_INVOKABLE const QStringList &fields() const;
  Q_INVOKABLE void setFields (const QStringList &newFields);

  Q_INVOKABLE QObject *item() const;
  Q_INVOKABLE void setItem (QObject *newItem);

  /// get cached field information
  Q_INVOKABLE const QList<Line> &fieldsData() const;
  /// update cache
  Q_INVOKABLE void update();
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  StyleDebugItemDescriptor &operator= (const StyleDebugItemDescriptor &a_src);
  StyleDebugItemDescriptor &operator= (StyleDebugItemDescriptor &&a_src);
  /// @}
};

Q_DECLARE_METATYPE (StyleDebugItemDescriptor)
Q_DECLARE_METATYPE (StyleDebugItemDescriptor::Line)

/*-----------------------------------------*/
#endif // STYLEDEBUGITEMDESCRIPTOR_H
