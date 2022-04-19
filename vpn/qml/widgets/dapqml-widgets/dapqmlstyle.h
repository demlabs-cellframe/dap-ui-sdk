#ifndef DAPQMLSTYLE_H
#define DAPQMLSTYLE_H

/* INCLUDES */
#include <QObject>
#include <QSize>
#include "style/scaled.h"

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
  Style::Scaled m_scaled;
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

  Q_INVOKABLE void windowResized (int a_width, int a_height);
  Q_INVOKABLE void requestRedraw();

  Q_INVOKABLE double centerHor (QObject *a_root, QObject *a_item);
  Q_INVOKABLE double centerVer (QObject *a_root, QObject *a_item);

  static void setup (const QString &styleSheet);
  static void update();
  static void sWindowResized (int a_width, int a_height);
  static void sRequestRedraw();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  /**
   * @brief redraw requested
   * This signal is sent from static object
   *
   * To send this signal simply execute sRequestRedraw() or requestRedraw()
   */
  void redrawRequested();
  void qssChanged();
  void itemChanged();
  void resized (int a_width, int a_height);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
protected slots:
  void _applyStyle();
  void _resized (int a_width, int a_height);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLSTYLE_H
