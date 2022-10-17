/* INCLUDES */
#include "dapqmlmodelsettings.h"
#include "helper/languagectl.h"
#include "DapDataLocal.h"

/* DEFS */
enum FieldId
{
  sid,
  textMain,
  textSub,
  icon,
  callback,

  invalid
};

/* VARS */
static DapQmlModelSettings *__inst = nullptr;
static QList<Item> s_settingsItemsList;
static qint32 s_daysLabelIndex     = -1;
static qint32 s_versionLabelIndex  = -1;
static qint32 s_countryIndex     = -1;

static QMap<QString, FieldId> s_fieldIdMap =
{
  {"sid", sid},
  {"textMain", textMain},
  {"textSub", textSub},
  {"icon", icon},
  {"callback", callback},
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelSettings::DapQmlModelSettings (QObject *parent)
  : QAbstractTableModel (parent)
{
  /* vars */
  __inst  = this;

  /* signals */
//  QMetaObject::invokeMethod (
//        this, &DapQmlModelSettings::slotUpdateLabels,
//        Qt::QueuedConnection);
  connect (LanguageCtl::instance(), &LanguageCtl::languageChanged,
           this, &DapQmlModelSettings::slotRetranslate,
           Qt::QueuedConnection);

  /* finish updating labels */
  slotUpdateLabels();
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelSettings *DapQmlModelSettings::instance()
{
  if (__inst == nullptr)
    __inst  = new DapQmlModelSettings;
  return __inst;
}

void DapQmlModelSettings::exec (int a_index, QObject *a_item)
{
  if (a_index < 0 || a_index >= s_items.size())
    return;

  auto cbv  = s_items.at (a_index).get ("callback");
  auto cb   = reinterpret_cast<ItemCB> (cbv.toULongLong());
  cb (a_item);
}

QString DapQmlModelSettings::notifier() const
{
  return "";
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelSettings::rowCount (const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return s_items.size();
}

int DapQmlModelSettings::columnCount (const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return 5;
}

QVariant DapQmlModelSettings::data (const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  auto field = s_fieldIdMap.key (FieldId (role));
  if (field.isEmpty())
    return QVariant();

  auto item  = s_items.value (index.row());
  return item.get (field);
}

QHash<int, QByteArray> DapQmlModelSettings::roleNames() const
{
  QHash<int, QByteArray> names;

  for (auto i = s_fieldIdMap.begin(), e = s_fieldIdMap.end(); i != e; i++)
    names[i.value()] = i.key().toStdString().c_str();

  return names;
}


void DapQmlModelSettings::_buildMenuItemsList()
{
    s_settingsItemsList =
    {
#ifndef BRAND_RISEVPN
      Item{SI_SPACER,     "", "", "1", "",                                                                   [](QObject*){} },
      Item{SI_TITLE,      tr ("Settings"), "", "settings_icon", "",                                          [](QObject*){} },

      Item{SI_BUTTONRED,  tr ("Get new licence key"), " ", "settings_icon ic_renew", "get_new_licence_key",  [](QObject*) { emit __inst->sigLicenceGet(); } },
      Item{SI_BUTTON,     tr ("Reset licence key"), "", "settings_icon ic_key", "reset_licence_key",         [](QObject*) { emit __inst->sigLicenceReset(); } },
      Item{SI_LINK,       tr ("Country"), "", "settings_icon ic_location", "country",                        [](QObject*) { emit __inst->sigCountry(); } },
  #ifndef DISABLE_SETTINGS_LANGUAGE
      Item{SI_LINK,       tr ("Language"), "", "settings_icon ic_language", "language",                      [](QObject*) { emit __inst->sigLanguage(); } },
  #endif // BRAND_KELVPN
  #ifndef DISABLE_THEMES
      Item{SI_LINK,       tr ("Color theme"), "", "settings_icon ic_theme", "color_theme",                   [](QObject*) { emit __inst->sigColorTheme(); } },
  #endif // DISABLE_THEMES

      Item{SI_TITLE,      tr ("Support"), "", "settings_icon", "support",                                    [](QObject*){} },

      Item{SI_BUTTON,     tr ("Send bug report"), "", "settings_icon ic_send-report", "send_bug_report",     [](QObject*) { emit __inst->sigBugSend(); } },
      Item{SI_BUTTON,     tr ("Telegram support bot"), "", "settings_icon ic_bot", "telegram_support_bot",   [](QObject*) { emit __inst->sigTelegramBot(); } },

      Item{SI_TITLE,      tr ("Information"), "", "settings_icon", "information",                            [](QObject*){} },

      Item{SI_LINK,       tr ("Bug reports"), "", "settings_icon ic_information_bug-report", "bug_reports",  [](QObject*) { emit __inst->sigBugReport(); } },
      Item{SI_BUTTON,     tr ("Serial key history on this device"), "", "settings_icon ic_key-history", "skey_history",  [](QObject*) { emit __inst->sigLicenceHistory(); } },
  #ifndef DISABLE_TERMSOFUSE_AND_PRIVACYPOLICY
      Item{SI_BUTTON,     tr ("Terms of use"), "", "settings_icon ic_terms_policy", "terms_of_use",          [](QObject*) { emit __inst->sigTermsOfUse(); } },
      Item{SI_BUTTON,     tr ("Privacy policy"), "", "settings_icon ic_terms_policy", "privacy_policy",      [](QObject*) { emit __inst->sigPrivacyPolicy(); } },
  #endif // DISABLE_TERMSOFUSE_AND_PRIVACYPOLICY
      Item{SI_BUTTONGRAY, tr ("Version"), "@version", "settings_icon ic_version", "release_version",         [](QObject*) { emit __inst->sigVersion(); } },

      Item{SI_TITLE,      "", "", "settings_icon", "title_a",                                                [](QObject*){} },
      Item{SI_TITLE,      "", "", "settings_icon", "title_b",                                                [](QObject*){} },
#else
      Item{SI_SPACER,     "", "", "1", "spacer",                                                                          [](QObject*){} },
      Item{SI_TITLE,      tr ("Settings"), "", "settings_icon", "settings",                                               [](QObject*){} },

      Item{SI_BUTTONRED,  tr ("Get new licence key"), " ", "settings_icon ic_renew", "get_new_licence_key",               [](QObject*) { emit __inst->sigSerialGet(); } },
      Item{SI_BUTTON,     tr ("Reset licence key"), "", "settings_icon ic_key", "reset_licence_key",                      [](QObject*) { emit __inst->sigSerialReset(); } },
      Item{SI_BUTTONRED,  tr ("Logout"), " ", "settings_icon ic_renew", "logout",                                         [](QObject*) { emit __inst->sigLogout(); } },
  #ifndef DISABLE_SETTINGS_LANGUAGE
      Item{SI_LINK,       tr ("Language"), "", "settings_icon ic_language", "language",                                           [](QObject*) { emit __inst->sigLanguage(); } },
  #endif // DISABLE_SETTINGS_LANGUAGE
      Item{SI_LINK,       tr ("Manage servers"), "", "settings_icon ic_language", "manage_servers",                       [](QObject*) { emit __inst->sigManageServers(); } },
      Item{SI_LINK,       tr ("Manage CDB"), "", "settings_icon ic_language", "manage_cdb",                               [](QObject*) { emit __inst->sigManageCDB(); } },
      Item{SI_LINK,       tr ("Cryptography"), "", "settings_icon ic_language", "cryptography",                           [](QObject*) { emit __inst->sigCryptography(); } },
  #ifndef DISABLE_THEMES
      Item{SI_CHECKBOX,   tr ("Dark theme"), "", "settings_icon ic_theme", "dark_themes",                                 [](QObject *a_item) { emit __inst->sigDarkTheme (a_item->property ("checked").toBool()); } },
  #endif // DISABLE_THEMES

      Item{SI_TITLE,      tr ("Support"), "", "settings_icon", "support",                                                 [](QObject*){} },

      Item{SI_BUTTON,     tr ("Send bug report"), "", "settings_icon ic_send-report", "send_bug_report",                  [](QObject*) { emit __inst->sigBugSend(); } },
      Item{SI_BUTTON,     tr ("Telegram support bot"), "", "settings_icon ic_bot", "telegram_support_bot",                [](QObject*) { emit __inst->sigTelegramBot(); } },

      Item{SI_TITLE,      tr ("Information"), "", "settings_icon", "information",                                         [](QObject*){} },

      Item{SI_LINK,       tr ("Bug reports"), "", "settings_icon ic_information_bug-report", "bug_reports",               [](QObject*) { emit __inst->sigBugReport(); } },
      Item{SI_BUTTON,     tr ("Serial key history on this device"), "", "settings_icon ic_key-history", "skey_history",   [](QObject*) { emit __inst->sigLicenceHistory(); } },
  #ifndef DISABLE_TERMSOFUSE_AND_PRIVACYPOLICY
      Item{SI_BUTTON,     tr ("Terms of use"), "", "settings_icon ic_terms_policy", "terms_of_use",                       [](QObject*) { emit __inst->sigTermsOfUse(); } },
      Item{SI_BUTTON,     tr ("Privacy policy"), "", "settings_icon ic_terms_policy", "privacy_policy",                   [](QObject*) { emit __inst->sigPrivacyPolicy(); } },
  #endif // DISABLE_TERMSOFUSE_AND_PRIVACYPOLICY
      Item{SI_BUTTONGRAY, tr ("Version"), "@version", "settings_icon ic_version", "release_version",                      [](QObject*) { emit __inst->sigVersion(); } },

      Item{SI_TITLE,      "", "", "settings_icon", "title_a",                                                             [](QObject*){} },
      Item{SI_TITLE,      "", "", "settings_icon", "title_b",                                                             [](QObject*){} },
#endif
    };
}

/********************************************
 * SLOTS
 *******************************************/

// menu filter
void DapQmlModelSettings::menuConstructor(QSet<QString> menuItems)
{
  _buildMenuItemsList();
#ifndef BRAND_RISEVPN
  Q_UNUSED(menuItems)
  foreach(Item item, s_settingsItemsList)
          s_items.append(item);
#else
  QStringList menuItemsList;
  // base menu
  menuItemsList << "spacer";
  menuItemsList << "settings";
  // use licence key
  if (menuItems.contains("use_licence_key"))
    menuItemsList << "get_new_licence_key" << "reset_licence_key";
  // use login
  if (menuItems.contains("use_login"))
    menuItemsList << "logout";
  // language
#ifndef DISABLE_SETTINGS_LANGUAGE
  "language" <<
#endif // DISABLE_SETTINGS_LANGUAGE
  if (menuItems.contains("use_manage_servers"))
    menuItemsList << "manage_servers" << "mancdb" << "cryptography";
  // ohter themes
#ifndef DISABLE_THEMES
  if (menuItems.contains("use_dark_themes"))
    menuItemsList << "dark_themes";
#endif // DISABLE_THEMES
  // base menu
  menuItemsList << "support";
  menuItemsList << "send_bug_report";
  // telegram support but
  if (menuItems.contains("use_telegram_suport_bot"))
    menuItemsList << "telegram_support_bot";
  menuItemsList << "information";
  menuItemsList << "bug_reports";
  // serial key history
  if (menuItems.contains("use_licence_key"))
    menuItemsList << "skey_history";
  // term of use and privacy policy
#ifndef DISABLE_TERMSOFUSE_AND_PRIVACYPOLICY
  menuItemsList << "terms_of_use";
  menuItemsList << "privacy_policy";
#endif // DISABLE_TERMSOFUSE_AND_PRIVACYPOLICY
  // relese version
  menuItemsList << "release_version";
  menuItemsList << "title_a" << "title_b";

  // create menu items list
  s_items.clear();
  foreach(QString itemName, menuItemsList)
      foreach(Item item, s_settingsItemsList)
          if (item.m_itemType == itemName)
          {
              s_items.append(item);
              break;
          }
#endif
}

void DapQmlModelSettings::slotUpdateLabels()
{
    _buildMenuItemsList();
    auto authType = DapDataLocal::instance()->authorizationType();
    if (authType == Authorization::account)
        menuConstructor(QSet<QString>() << "use_manage_servers" << "use_login");
    if (authType == Authorization::serialKey)
        menuConstructor(QSet<QString>() << "use_manage_servers" << "use_licence_key");
    if (authType == Authorization::undefined)
        menuConstructor(QSet<QString>() << "use_licence_key");


  /* find indexes */
  qint32 index = 0;
  for (auto i = s_items.cbegin(), e = s_items.cend(); i != e; i++, index++)
    {
      if (i->m_itemType == "get_new_licence_key")
        s_daysLabelIndex    = index;

      if (i->m_itemType == "release_version")
        s_versionLabelIndex = index;

      if (i->m_itemType == "country")
      {
          s_countryIndex    = index;
      }
    }

  /* set version */
  if (s_versionLabelIndex != -1)
    {
      auto version  = QString ("%1 %2")
          .arg (DAP_VERSION, __DATE__);
      s_items[s_versionLabelIndex].m_textSub  = version;
    }

  if (s_countryIndex != -1)
    {
      s_items[s_countryIndex].m_textSub = getCurrentCountryCode();
    }

}

void DapQmlModelSettings::slotUpdateItemsList()
{
    beginResetModel();
    slotUpdateLabels();
    endResetModel();

    emit dataChanged (
      index (s_daysLabelIndex, 0),
      index (s_daysLabelIndex, columnCount()));
}

void DapQmlModelSettings::slotSetDaysLeft (QString a_days)
{
  if (s_daysLabelIndex == -1)
    return;
  beginResetModel();
  s_items[s_daysLabelIndex].m_textSub = (a_days.startsWith("-")) ? "expired" : a_days;
  endResetModel();

  emit dataChanged (
    index (s_daysLabelIndex, 0),
    index (s_daysLabelIndex, columnCount()));
}

void DapQmlModelSettings::slotResetDaysLeft()
{
  if (s_daysLabelIndex == -1)
    return;
  beginResetModel();
  s_items[s_daysLabelIndex].m_textSub.clear();
  endResetModel();

  emit dataChanged (
    index (s_daysLabelIndex, 0),
        index (s_daysLabelIndex, columnCount()));
}

void DapQmlModelSettings::slotCountryChange()
{
    if (s_countryIndex == -1)
      return;
    beginResetModel();
    s_items[s_countryIndex].m_textSub = getCurrentCountryCode();
    endResetModel();

    emit dataChanged (
      index (s_daysLabelIndex, 0),
          index (s_daysLabelIndex, columnCount()));
}

void DapQmlModelSettings::slotRetranslate()
{
  slotUpdateLabels();
  emit languageChanged();
}

QString DapQmlModelSettings::getCurrentCountryCode() const
{

    QString base_location = DapDataLocal::instance()->getSetting (COUNTRY_NAME).toString();
    QString code = DapServersData::m_countryMap[base_location];
    return code;
}

/********************************************
 * ModelSettingsItem
 *******************************************/

DapQmlModelSettingsItem::DapQmlModelSettingsItem()
  : QObject()
  , m_sid (DapQmlModelSettings::SI_TITLE)
  , m_cb ([](QObject*){})
{

}

DapQmlModelSettingsItem::DapQmlModelSettingsItem (const DapQmlModelSettingsItem &src)
  : QObject (src.parent())
{
  m_sid       = src.m_sid;
  m_textMain  = src.m_textMain;
  m_textSub   = src.m_textSub;
  m_icon      = src.m_icon;
  m_itemType  = src.m_itemType;
  m_cb        = src.m_cb;
}

DapQmlModelSettingsItem::DapQmlModelSettingsItem (DapQmlModelSettingsItem &&src)
{
  if (&src == this)
    return;

  m_sid       = std::move (src.m_sid);
  m_textMain  = std::move (src.m_textMain);
  m_textSub   = std::move (src.m_textSub);
  m_icon      = std::move (src.m_icon);
  m_itemType  = std::move (src.m_itemType);
  m_cb        = std::move (src.m_cb);
  setParent (src.parent());
}

DapQmlModelSettingsItem::DapQmlModelSettingsItem (
  const DapQmlModelSettings::StyleId a_sid,
  const QString a_textMain,
  const QString a_textSub,
  const QString a_icon,
  const QString a_itemType,
  const DapQmlModelSettings::ItemCB a_callback)
{
  m_sid       = a_sid;
  m_textMain  = a_textMain;
  m_textSub   = a_textSub;
  m_icon      = a_icon;
  m_itemType  = a_itemType;
  m_cb        = a_callback;
}

void DapQmlModelSettingsItem::set (const QString a_name, const QVariant a_value)
{
  auto fieldId  = s_fieldIdMap.value (a_name, invalid);

  if (fieldId == invalid)
    return;

  switch (fieldId)
    {
    case sid:       m_sid       = DapQmlModelSettings::StyleId (a_value.toInt()); break;
    case textMain:  m_textMain  = a_value.toString(); break;
    case textSub:   m_textSub   = a_value.toString(); break;
    case icon:      m_icon      = a_value.toString(); break;
    case callback:  m_cb        = reinterpret_cast<DapQmlModelSettings::ItemCB> (a_value.toULongLong()); break;
    default: return;
    }
}

QVariant DapQmlModelSettingsItem::get (const QString a_name) const
{
  auto fieldId  = s_fieldIdMap.value (a_name, invalid);

  if (fieldId == invalid)
    return QVariant();

  switch (fieldId)
    {
    case sid:       return m_sid;
    case textMain:  return m_textMain;
    case textSub:   return m_textSub;
    case icon:      return m_icon;
    case callback:  return reinterpret_cast<quint64> (reinterpret_cast<void *> (m_cb));
    default: return QVariant();
    }
}

DapQmlModelSettingsItem &DapQmlModelSettingsItem::operator= (const DapQmlModelSettingsItem &src)
{
  m_sid       = src.m_sid;
  m_textMain  = src.m_textMain;
  m_textSub   = src.m_textSub;
  m_icon      = src.m_icon;
  m_itemType  = src.m_itemType;
  m_cb        = src.m_cb;
  setParent (src.parent());
  return *this;
}

DapQmlModelSettingsItem &DapQmlModelSettingsItem::operator= (DapQmlModelSettingsItem &&src)
{
  if (&src == this)
    return *this;

  m_sid       = std::move (src.m_sid);
  m_textMain  = std::move (src.m_textMain);
  m_textSub   = std::move (src.m_textSub);
  m_icon      = std::move (src.m_icon);
  m_itemType  = std::move (src.m_itemType);
  m_cb        = std::move (src.m_cb);
  setParent (src.parent());
  return *this;
}

/*-----------------------------------------*/
