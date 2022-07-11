#ifndef DAPQMLMODELSETTINGS_H
#define DAPQMLMODELSETTINGS_H

/* INCLUDES */
#include <QAbstractTableModel>

/****************************************//**
 * @brief settings model list
 * @ingroup groupUiModels
 * @date 27.12.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelSettings : public QAbstractTableModel
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (QString notifier READ notifier NOTIFY languageChanged)
  /// @}

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:

  typedef void (*ItemCB) ();

  enum StyleId
  {
    SI_TITLE,
    SI_TITLETOP,
    SI_BUTTON,
    SI_BUTTONRED,
    SI_BUTTONGRAY,
    SI_LINK,
    SI_SPACER
  };
  Q_ENUM(StyleId)
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlModelSettings(QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapQmlModelSettings *instance();
  Q_INVOKABLE void exec(int index);
  Q_INVOKABLE QString notifier() const;
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  int columnCount (const QModelIndex &parent = QModelIndex()) const override;

  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  /* settings */
  void sigLicenceGet();
  void sigLicenceReset();
  void sigLanguage();
  void sigColorTheme();
  void sigSetDaysLeft (QString days);
  void sigResetDaysLeft();

  /* support */
  void sigBugSend();
  void sigTelegramBot();

  /* info */
  void sigBugReport();
  void sigLicenceHistory();
  void sigTermsOfUse();
  void sigPrivacyPolicy();
  void sigVersion();

  /* retranslation */
  void languageChanged();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotUpdateLabels();
  void slotSetDaysLeft (QString a_days);
  void slotResetDaysLeft();
protected slots:
  void slotRetranslate();
  /// @}
};

class DapQmlModelSettingsItem : public QObject
{
  Q_OBJECT

  friend class DapQmlModelSettings;

  DapQmlModelSettings::StyleId m_sid;
  QString m_textMain;
  QString m_textSub;
  QString m_icon;
  DapQmlModelSettings::ItemCB m_cb;

public:
  DapQmlModelSettingsItem();
  DapQmlModelSettingsItem (const DapQmlModelSettingsItem &src);
  DapQmlModelSettingsItem (DapQmlModelSettingsItem &&src);
  DapQmlModelSettingsItem (
      const DapQmlModelSettings::StyleId a_sid,
      const QString a_textMain,
      const QString a_textSub,
      const QString a_icon,
      const DapQmlModelSettings::ItemCB a_callback);

  Q_INVOKABLE void set (const QString a_name, const QVariant a_value);
  Q_INVOKABLE QVariant get (const QString a_name) const;

  DapQmlModelSettingsItem& operator=(const DapQmlModelSettingsItem &src);
  DapQmlModelSettingsItem& operator=(DapQmlModelSettingsItem &&src);
};

/*-----------------------------------------*/
#endif // DAPQMLMODELSETTINGS_H
