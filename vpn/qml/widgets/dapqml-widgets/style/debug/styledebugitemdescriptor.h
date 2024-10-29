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
 *
 * Used only for debug purposes
 *
 * @code
 * // inside qml item
 * Component.onCompleted: StyleDebugTree.describe ("info label", ["x", "y", "width", "height"], this);
 * @endcode
 *
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
  /// pair of strings: name and value
  using Line = QPair<QString, QString>;
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  /* INFORMATION */
  QString m_name;                 ///< descriptor name
  QStringList m_fields;           ///< values names to describe (requested by QObject::property(fieldName))
  QObject *m_item;                ///< pointer to QML item
  QMetaObject::Connection m_conn; ///< item destroy event connection. used to remove descriptor when QML item destroyed

  /* CACHED STORAGE */
  QList<Line> m_fieldData;        ///< cached values requested from QML item. used to display inside tree
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
protected:
  void _reconnectItem();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
protected slots:
  void slotOnDestroyed (QObject *a_ptr);
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
