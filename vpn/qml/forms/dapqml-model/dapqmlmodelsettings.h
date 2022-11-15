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
class DapQmlModelSettingsItem;
typedef DapQmlModelSettingsItem Item;

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

  typedef void (*ItemCB) (QObject *a_item);

  enum StyleId
  {
    SI_TITLE,
    SI_TITLETOP,
    SI_BUTTON,
    SI_BUTTONRED,
    SI_BUTTONGRAY,
    SI_LINK,
    SI_CHECKBOX,
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
  Q_INVOKABLE void exec (int a_index, QObject *a_item = nullptr);
  Q_INVOKABLE QString notifier() const;
  Q_INVOKABLE QVariant value (int a_index, const QString &a_fieldName) const;
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
private:
  void _buildMenuItemsList();
  void _updateMenuContent (const QSet<QString> &a_filterKeywords);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:

  void sigSetDaysLeft (QString days);
  void sigResetDaysLeft();

#ifndef BRAND_RISEVPN

  /* settings */
  void sigLicenceGet();
  void sigLicenceReset();
  void sigLanguage();
  void sigCountry();
  void sigColorTheme();

  /* support */
  void sigBugSend();
  void sigTelegramBot();

  /* info */
  void sigBugReport();
  void sigLicenceHistory();
  void sigTermsOfUse();
  void sigPrivacyPolicy();
  void sigVersion();

#else // BRAND_RISEVPN

  /* settings */
  void sigSerialGet();
  void sigSerialReset();
  void sigLogout();
  void sigLanguage();
  void sigManageServers();
  void sigCertificate();
  void sigDarkTheme (bool a_state);

  /* support */
  void sigBugSend();
  void sigTelegramBot();

  /* info */
  void sigBugReport();
  void sigLicenceHistory();
  void sigTermsOfUse();
  void sigPrivacyPolicy();
  void sigVersion();

#endif // BRAND_RISEVPN

  /* retranslation */
  void languageChanged();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotUpdateLabels (bool a_forced);
  void slotSetDaysLeft (QString a_days);
  void slotResetDaysLeft();
  void slotCountryChange();
  void slotUpdateItemsList();
private:
  QString getCurrentCountryCode() const;
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
  /*item menu attribute*/
  QString m_itemType;
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
      const QString a_itemType,
      const DapQmlModelSettings::ItemCB a_callback);

  Q_INVOKABLE void set (const QString a_name, const QVariant a_value);
  Q_INVOKABLE QVariant get (const QString a_name) const;

  DapQmlModelSettingsItem& operator=(const DapQmlModelSettingsItem &src);
  DapQmlModelSettingsItem& operator=(DapQmlModelSettingsItem &&src);
};

/*-----------------------------------------*/
#endif // DAPQMLMODELSETTINGS_H
