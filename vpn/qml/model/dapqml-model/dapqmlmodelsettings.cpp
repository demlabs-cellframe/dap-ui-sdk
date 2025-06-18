/* INCLUDES */
#include "dapqmlmodelsettings.h"
#include "DapDataLocal.h"
#include "DapSerialKeyData.h"

/* DEFS */

/* turn this on, to see settings filter prints */
//#define PRINT_ITEMS_FILTER_RESULT

#ifdef PRINT_ITEMS_FILTER_RESULT
#define PRINT_ITEM(a,b,c) printInfo (a, b, c);
#else // PRINT_ITEMS_FILTER_RESULT
#define PRINT_ITEM(a,b,c)
#endif // PRINT_ITEMS_FILTER_RESULT


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
static QList<Item> s_items;
static QSet<QString> s_lastFilterKeywords;

static qint32 s_daysLabelIndex    = -1;
static qint32 s_rouExcIndex       = -1;
static qint32 s_countryIndex      = -1;
static qint32 s_versionLabelIndex = -1;

static QString s_daysLeftString;

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
//  connect (LanguageCtl::instance(), &LanguageCtl::languageChanged,
//           this, &DapQmlModelSettings::slotRetranslate,
//           Qt::QueuedConnection);

  /* finish updating labels */
  slotUpdateLabels (true);
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

QVariant DapQmlModelSettings::value(int a_index, const QString &a_fieldName) const
{
  auto valueId  = s_fieldIdMap.value (a_fieldName, textMain);
  auto result   = data (index (a_index, 0), valueId);
  return result;
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

  return s_fieldIdMap.size();
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
    s_items =
    {
#ifndef BRAND_RISEVPN
      Item{SI_SPACER,     "", "", "1", "",                                                                   [](QObject*){} },
      Item{SI_TITLE,      tr ("Settings"), "", "settings_icon", "",                                          [](QObject*){} },

      Item{SI_BUTTONRED,  tr ("Get new licence key"), " ", "settings_icon ic_renew", "get_new_licence_key",  [](QObject*) { emit __inst->sigLicenceGet(); } },
      Item{SI_BUTTON,     tr ("Reset licence key"), "", "settings_icon ic_key", "reset_licence_key",         [](QObject*) { emit __inst->sigLicenceReset(); } },
      Item{SI_LINK,       tr ("Your country"), "", "settings_icon ic_location", "country",                   [](QObject*) { emit __inst->sigCountry(); } },
  #ifdef BRAND_KELVPN
  #ifdef Q_OS_ANDROID
      Item{SI_LINK,       tr ("Routing Exceptions"), "", "settings_icon ic_route", "rouexc",                 [](QObject*) { emit __inst->sigRouExc(); } },
  #endif // Q_OS_ANDROID
  #endif // BRAND_KELVPN
  #ifndef DISABLE_SETTINGS_LANGUAGE
      Item{SI_LINK,       tr ("Language"), "", "settings_icon ic_language", "language",                      [](QObject*) { emit __inst->sigLanguage(); } },
  #endif // BRAND_KELVPN
  #ifndef DISABLE_THEMES
      Item{SI_CHECKBOX,   tr ("Dark theme"), "", "settings_icon ic_theme", "dark_themes",                    [](QObject *a_item) { emit __inst->sigDarkTheme (a_item->property ("checked").toBool()); } },
      //Item{SI_LINK,       tr ("Color theme"), "", "settings_icon ic_theme", "color_theme",                   [](QObject*) { emit __inst->sigColorTheme(); } },
      Item{SI_LINK,       tr ("Notification center"), "", "settings_icon ic_notification", "notification",   [](QObject*) { emit __inst->sigNotification(); } },
  #endif // DISABLE_THEMES

      Item{SI_TITLE,      tr ("Support"), "", "settings_icon", "support",                                    [](QObject*){} },

      Item{SI_LINK,       tr ("Send bug report"), "", "settings_icon ic_send-report", "send_bug_report",     [](QObject*) { emit __inst->sigBugSend(); } },
      Item{SI_BUTTON,     tr ("Telegram support bot"), "", "settings_icon ic_bot", "telegram_support_bot",   [](QObject*) { emit __inst->sigTelegramBot(); } },
      Item{SI_BUTTON,     tr ("Share logs"), "", "settings_icon ic_share_log", "share_logs",                 [](QObject*) { emit __inst->sigShareLog(); } },

      Item{SI_TITLE,      tr ("Information"), "", "settings_icon", "information",                            [](QObject*){} },

      Item{SI_LINK,       tr ("Bug reports"), "", "settings_icon ic_information_bug-report", "bug_reports",  [](QObject*) { emit __inst->sigBugReport(); } },
  #ifndef BRAND_VENDETA
      Item{SI_LINK,       tr ("FAQ"), "", "ic_faq", "faq",                                                   [](QObject*) { emit __inst->sigFaq(); } },
  #endif // BRAND_VENDETA
      Item{SI_BUTTON,     tr ("Serial key history on this device"), "", "settings_icon ic_key-history", "skey_history",  [](QObject*) { emit __inst->sigLicenceHistory(); } },
  #ifndef DISABLE_TERMSOFUSE_AND_PRIVACYPOLICY
      Item{SI_BUTTON,     tr ("Terms of use"), "", "settings_icon ic_terms_policy", "terms_of_use",          [](QObject*) { emit __inst->sigTermsOfUse(); } },
      Item{SI_BUTTON,     tr ("Privacy policy"), "", "settings_icon ic_terms_policy", "privacy_policy",      [](QObject*) { emit __inst->sigPrivacyPolicy(); } },
  #endif // DISABLE_TERMSOFUSE_AND_PRIVACYPOLICY
      Item{SI_BUTTONGRAY, tr ("Version"), "@version", "settings_icon ic_version", "release_version",         [](QObject*) { emit __inst->sigVersion(); } },

      Item{SI_TITLE,      "", "", "settings_icon", "title_a",                                                [](QObject*){} },
      Item{SI_TITLE,      "", "", "settings_icon", "title_b",                                                [](QObject*){} },
#else // BRAND_RISEVPN
      Item{SI_SPACER,     "", "", "1", "spacer",                                                                          [](QObject*){} },
      Item{SI_TITLE,      tr ("Settings"), "", "settings_icon", "settings",                                               [](QObject*){} },

      //Item{SI_BUTTONRED,  tr ("Get new licence key"), " ", "settings_icon ic_renew", "get_new_licence_key",               [](QObject*) { emit __inst->sigSerialGet(); } },
      Item{SI_BUTTON,     tr ("Reset licence key"), "", "settings_icon ic_key", "reset_licence_key",                      [](QObject*) { emit __inst->sigSerialReset(); } },
      Item{SI_BUTTONRED,  tr ("Logout"), " ", "settings_icon ic_renew", "logout",                                         [](QObject*) { emit __inst->sigLogout(); } },
 #ifndef DISABLE_SETTINGS_LANGUAGE
      Item{SI_LINK,       tr ("Language"), "", "settings_icon ic_language", "language",                                           [](QObject*) { emit __inst->sigLanguage(); } },
 #endif // DISABLE_SETTINGS_LANGUAGE
      Item{SI_LINK,       tr ("Manage CDB"), "", "settings_icon ic_cdb-manager", "manage_cdb",                            [](QObject*) { emit __inst->sigManageCDB(); } },
      Item{SI_LINK,       tr ("Manage servers"), "", "settings_icon ic_server-manager", "manage_servers",                 [](QObject*) { emit __inst->sigManageServers(); } },
      Item{SI_LINK,       tr ("Certificate"), "", "settings_icon ic_certificate", "certificate",                          [](QObject*) { emit __inst->sigCertificate(); } },
  #ifndef DISABLE_THEMES
      Item{SI_CHECKBOX,   tr ("Dark theme"), "", "settings_icon ic_theme", "dark_themes",                                 [](QObject *a_item) { emit __inst->sigDarkTheme (a_item->property ("checked").toBool()); } },
  #endif // DISABLE_THEMES

      Item{SI_TITLE,      tr ("Support"), "", "settings_icon", "support",                                                 [](QObject*){} },

      Item{SI_LINK,       tr ("Send bug report"), "", "settings_icon ic_send-report", "send_bug_report",                  [](QObject*) { emit __inst->sigBugSend(); } },
      //Item{SI_BUTTON,     tr ("Telegram support bot"), "", "settings_icon ic_bot", "telegram_support_bot",                [](QObject*) { emit __inst->sigTelegramBot(); } },

      Item{SI_TITLE,      tr ("Information"), "", "settings_icon", "information",                                         [](QObject*){} },

      Item{SI_LINK,       tr ("Bug reports"), "", "settings_icon ic_information_bug-report", "bug_reports",               [](QObject*) { emit __inst->sigBugReport(); } },
      Item{SI_LINK,       tr ("FAQ"), "", "ic_faq", "faq",                                                   [](QObject*) { emit __inst->sigFaq(); } },
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

// menu filter
void DapQmlModelSettings::_updateMenuContent (const QSet<QString> &a_filterKeywords)
{
  if (s_lastFilterKeywords == a_filterKeywords)
    return;
  s_lastFilterKeywords  = a_filterKeywords;

  _buildMenuItemsList();

#ifdef BRAND_RISEVPN
  /* conditional value definition */
  auto conditionalValue = [a_filterKeywords] (const char *a_condition, const char *a_value) -> const char *
    {
      if (a_filterKeywords.contains (a_condition))
        return a_value;
      return "";
    };

  /* collect keywords */
  QSet<QString> keywords = {
    /* ----- */
    /* title */
    /* ----- */

    "spacer",
    "settings",

    /* use licence key */
    //conditionalValue ("use_licence_key", "get_new_licence_key"),
    conditionalValue ("use_licence_key", "reset_licence_key"),

    /* use login */
    conditionalValue ("use_login", "logout"),
    "language",
    conditionalValue ("use_manage_servers", "manage_servers"),
    conditionalValue ("use_manage_servers", "manage_cdb"),
    conditionalValue ("use_manage_servers", "certificate"),

    /* themes */
    "dark_themes",


    /* ----- */
    /* title */
    /* ----- */
    "support",

    /* support */
    "send_bug_report",
    "telegram_support_bot",


    /* ----- */
    /* title */
    /* ----- */
    "information",

    /* lists */
    "bug_reports",
    conditionalValue ("use_licence_key", "skey_history"),

    /* tersm & poliies */
#ifndef DISABLE_TERMSOFUSE_AND_PRIVACYPOLICY
    "terms_of_use",
    "privacy_policy",
#endif // DISABLE_TERMSOFUSE_AND_PRIVACYPOLICY

    /* version */
    "release_version",
    "title_a",
    "title_b"
  };

#ifdef PRINT_ITEMS_FILTER_RESULT
  auto printInfo = [] (const Item &a_item, const char *a_text, const char *a_funcName)
    {
      qDebug() << a_funcName
               << QString ("%1: %2, \"%3\"")
                  .arg (a_text, a_item.m_itemType, a_item.m_textMain);
    };
#endif // PRINT_ITEMS_FILTER_RESULT

  /* remove items which keyword is missing */
  for (auto it = s_items.begin(); it != s_items.end(); it++)
    {
      auto &item  = *it;
      if (keywords.contains (item.m_itemType))
        {
          PRINT_ITEM (item, "saved item", __PRETTY_FUNCTION__);
          continue;
        }

      PRINT_ITEM (item, "erased item", __PRETTY_FUNCTION__);
      s_items.erase (it);
      // it--;
    }
#endif
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelSettings::slotUpdateLabels (bool a_forced)
{
  if (a_forced)
    s_lastFilterKeywords = {"__dummy_items__", "__to_fill_settings__"};

  auto authType = DapDataLocal::instance()->authorizationType();
  switch (authType)
    {
    case Authorization::account:    _updateMenuContent ({"use_manage_servers", "use_login"}); break;
    case Authorization::serialKey:  _updateMenuContent ({"use_manage_servers", "use_licence_key"}); break;
    case Authorization::undefined:  _updateMenuContent ({}); break;
    default:break;
    }

  /* find indexes */
  qint32 index = 0;
  for (auto i = s_items.cbegin(), e = s_items.cend(); i != e; i++, index++)
    {
      if (i->m_itemType      == "get_new_licence_key")
        s_daysLabelIndex    = index;

      else if (i->m_itemType == "rouexc")
        s_rouExcIndex       = index;

      else if (i->m_itemType == "release_version")
        s_versionLabelIndex = index;

      else if (i->m_itemType == "country")
        s_countryIndex      = index;
    }

  /* set version */
  if (s_versionLabelIndex != -1)
    {
      auto version  = QString ("%1 %2")
          .arg (DAP_VERSION, __DATE__);
      s_items[s_versionLabelIndex].m_textSub  = version;
    }

  /* update country label name */
  if (s_countryIndex != -1)
    {
      s_items[s_countryIndex].m_textSub = getCurrentCountryCode();
    }

  /* update rouexc label text */
  slotRouExcModeUpdated();
}

void DapQmlModelSettings::slotUpdateItemsList()
{
  beginResetModel();
  slotUpdateLabels (false);
  endResetModel();
}

void DapQmlModelSettings::slotRouExcModeUpdated()
{
  if (s_rouExcIndex == -1)
    return;

  bool includedMode = DapDataLocal::getSetting (DapBaseDataLocal::SETTING_ROUEXC_MODE).toBool();

  s_items[s_rouExcIndex].m_textMain =
      !includedMode
      ? "Routing Exceptions"
      : "Inclusion in Routing";

  emit dataChanged (
    index (s_rouExcIndex, 0),
    index (s_rouExcIndex, columnCount (index (s_rouExcIndex, 3))));
}

void DapQmlModelSettings::slotSetDaysLeft (QString a_days)
{
  if (s_daysLabelIndex == -1)
    return;

  if (a_days == "$")
    a_days  = DapDataLocal::instance()->serialKeyData()->daysLeftString();

  s_daysLeftString = (a_days.startsWith("-")) ? "expired" : a_days;

  s_items[s_daysLabelIndex].m_textSub = s_daysLeftString;

  emit dataChanged (
    index (s_daysLabelIndex, 0),
    index (s_daysLabelIndex, columnCount (index (s_daysLabelIndex, 3))));
}

void DapQmlModelSettings::slotResetDaysLeft()
{
  if (s_daysLabelIndex == -1)
    return;

  s_items[s_daysLabelIndex].m_textSub.clear();

  emit dataChanged (
    index (s_daysLabelIndex, 0),
    index (s_daysLabelIndex, columnCount (index (s_daysLabelIndex, 3))));
}

void DapQmlModelSettings::slotCountryChange()
{
  if (s_countryIndex == -1)
    return;

  s_items[s_countryIndex].m_textSub = getCurrentCountryCode();

  emit dataChanged (
    index (s_countryIndex, 0),
    index (s_countryIndex, columnCount (index (s_countryIndex, 3))));
}

void DapQmlModelSettings::slotRetranslate()
{
  slotUpdateLabels (true);
  slotSetDaysLeft ("$");
  emit languageChanged();
}

QString DapQmlModelSettings::getCurrentCountryCode() const
{
  QString base_location = DapDataLocal::instance()->getSetting (DapBaseDataLocal::COUNTRY_NAME).toString();
  return DapAbstractServerList::countryMap().value (base_location, QString()); // DapServersData::m_countryMap.value (base_location, QString());
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
