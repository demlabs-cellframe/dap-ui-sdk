#ifndef KELVPNNEUROMORPHIC_H
#define KELVPNNEUROMORPHIC_H

/************************************************//**
 * @defgroup groupKelGuiWidgets Kel Gui Widgets
 * Classes represents custom widgets for Kel Gui
 *
 * Basic classes:
 * - KelGuiButton
 * - KelGuiPushButton
 * - KelGuiRadio
 * - KelGuiCheckBox
 * - KelGuiSeparator
 * - KelGuiPlainTextEdit
 *
 * Misc. classes:
 * - KelGuiLabel
 * - KelGuiWidget
 *
 * @date 02.09.2021
 * @authors Mikhail Shilenko
 ***************************************************/

/* INCLUDES */
#ifdef Q_OS_ANDROID
#include <QtUiPlugin/QDesignerCustomWidgetCollectionInterface>
#else
#include <QtDesigner>
#endif
#include <qplugin.h>

/****************************************//**
 * @brief plugin definition
 * @ingroup groupKelGuiWidgets
 * @date 02.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelVpnNeuromorphic : public QObject, public QDesignerCustomWidgetCollectionInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface")
#endif // QT_VERSION >= 0x050000

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit KelVpnNeuromorphic(QObject *parent = 0);
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:
  virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  QList<QDesignerCustomWidgetInterface*> m_widgets;
  /// @}
};

/*-----------------------------------------*/
#endif // KELVPNNEUROMORPHIC_H
