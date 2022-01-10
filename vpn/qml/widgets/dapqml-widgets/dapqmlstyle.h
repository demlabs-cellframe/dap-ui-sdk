#ifndef DAPQMLSTYLE_H
#define DAPQMLSTYLE_H

/* INCLUDES */
#include <QObject>

/****************************************//**
 * @brief style manager for qml widgets
 * @ingroup groupDapGuiStyle
 * @date 29.12.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlStyle : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (QString qss READ getQss WRITE setQss NOTIFY qssChanged)
  Q_PROPERTY (QObject *item READ getItem WRITE setItem NOTIFY itemChanged)
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QString m_qss;
  QObject *m_item;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlStyle (QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  Q_INVOKABLE QString getQss() const;
  Q_INVOKABLE void setQss (const QString &a_newQss);

  Q_INVOKABLE QObject *getItem() const;
  Q_INVOKABLE void setItem (QObject *a_newItem);

  Q_INVOKABLE double centerHor (QObject *a_root, QObject *a_item);
  Q_INVOKABLE double centerVer (QObject *a_root, QObject *a_item);

  static void setup (const QString &styleSheet);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void qssChanged();
  void itemChanged();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
protected slots:
  void _applyStyle();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLSTYLE_H
