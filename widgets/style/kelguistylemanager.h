#ifndef KELGUISTYLEMANAGER_H
#define KELGUISTYLEMANAGER_H

/************************************************//**
 * @defgroup groupKelGuiStyle Kel Gui Style
 * Classes responsible for Gui style
 * @ingroup groupKelGuiWidgets
 * @date 07.09.2021
 * @authors Mikhail Shilenko
 ***************************************************/

/* INCLUDES */
#include <QObject>
#include <QMutexLocker>


/****************************************//**
 * @brief style manager for widgets
 * @ingroup groupKelGuiStyle
 * @date 07.09.2021
 * @author Mikhail Shilenko
 *******************************************/

class KelGuiStyleManager : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (QString cssStyle READ cssStyle WRITE setCssStyle)
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  QString m_cssStyle;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit KelGuiStyleManager(QObject *parent);
  /// @}

  /****************************************//**
   * @name PUBLIC METHODS
   *******************************************/
  /// @{
public:
  QString cssStyle() const;
  void setCssStyle(const QString &cssStyle);
  /// @}

  /****************************************//**
   * @name PROTECTED METHODS
   *******************************************/
  /// @{
protected:
  /// get style by class name
  QString styleByClassName (const QString &className);
  /// collect all styles by class array (separated by space)
  /// @param[in] classNameList class array (separated by space)
  /// @param[in] addNamedStyle will append nametag style
  QString styleByClassList (const QString &classNameList, bool addNamedStyle = true);
  /// collect all styles by class array
  /// @param[in] classNameList class array
  /// @param[in] addNamedStyle will append nametag style
  QString styleByClassList (const QStringList &classNameList, bool addNamedStyle = true);
  /// @}
};

/*-----------------------------------------*/
#endif // KELGUISTYLEMANAGER_H
