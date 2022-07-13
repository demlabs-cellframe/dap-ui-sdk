/* INCLUDES */
#include "dapqmlmodelsettings.h"
#include "helper/languagectl.h"

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
static QList<DapQmlModelSettingsItem> s_items;
static qint32 s_daysLabelIndex     = -1;
static qint32 s_versionLabelIndex  = -1;

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

void DapQmlModelSettings::exec (int index)
{
  if (index < 0 || index >= s_items.size())
    return;

  auto cbv  = s_items.at (index).get ("callback");
  auto cb   = reinterpret_cast<ItemCB> (cbv.toULongLong());
  cb();
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

  auto field  = s_fieldIdMap.key (FieldId (role));
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

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelSettings::slotUpdateLabels()
{
  s_items =
  {
#ifndef BRAND_RISEVPN

    DapQmlModelSettingsItem{SI_SPACER,     "", "", "1",                                                                   []{} },
    DapQmlModelSettingsItem{SI_TITLE,      tr ("Settings"), "", "settings_icon",                                          []{} },

    DapQmlModelSettingsItem{SI_BUTTONRED,  tr ("Get new licence key"), " ", "settings_icon ic_renew",                     [] { emit __inst->sigLicenceGet(); } },
    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Reset licence key"), "", "settings_icon ic_key",                          [] { emit __inst->sigLicenceReset(); } },
    DapQmlModelSettingsItem{SI_LINK,       tr ("Language"), "", "settings_icon ic_language",                              [] { emit __inst->sigLanguage(); } },
#ifndef DISABLE_THEMES
    DapQmlModelSettingsItem{SI_LINK,       tr ("Color theme"), "", "settings_icon ic_theme",                              [] { emit __inst->sigColorTheme(); } },
#endif // DISABLE_THEMES

    DapQmlModelSettingsItem{SI_TITLE,      tr ("Support"), "", "settings_icon",                                           []{} },

    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Send bug report"), "", "settings_icon ic_send-report",                    [] { emit __inst->sigBugSend(); } },
    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Telegram support bot"), "", "settings_icon ic_bot",                       [] { emit __inst->sigTelegramBot(); } },

    DapQmlModelSettingsItem{SI_TITLE,      tr ("Information"), "", "settings_icon",                                       []{} },

    DapQmlModelSettingsItem{SI_LINK,       tr ("Bug reports"), "", "settings_icon ic_information_bug-report",             [] { emit __inst->sigBugReport(); } },
    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Serial key history on this device"), "", "settings_icon ic_key-history",  [] { emit __inst->sigLicenceHistory(); } },
#ifndef DISABLE_TERMSOFUSE_AND_PRIVACYPOLICY
    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Terms of use"), "", "settings_icon ic_terms_policy",                      [] { emit __inst->sigTermsOfUse(); } },
    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Privacy policy"), "", "settings_icon ic_terms_policy",                    [] { emit __inst->sigPrivacyPolicy(); } },
#endif // DISABLE_TERMSOFUSE_AND_PRIVACYPOLICY
    DapQmlModelSettingsItem{SI_BUTTONGRAY, tr ("Version"), "@version", "settings_icon ic_version",                        [] { emit __inst->sigVersion(); } },

    DapQmlModelSettingsItem{SI_TITLE,      "", "", "settings_icon",                                                       []{} },
    DapQmlModelSettingsItem{SI_TITLE,      "", "", "settings_icon",                                                       []{} },

#else // BRAND_RISEVPN

    DapQmlModelSettingsItem{SI_SPACER,     "", "", "1",                                                                   []{} },
    DapQmlModelSettingsItem{SI_TITLE,      tr ("Settings"), "", "settings_icon",                                          []{} },

    DapQmlModelSettingsItem{SI_BUTTONRED,  tr ("Get new licence key"), " ", "settings_icon ic_renew",                     [] { emit __inst->sigSerialGet(); } },
    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Reset licence key"), "", "settings_icon ic_key",                          [] { emit __inst->sigSerialReset(); } },
    DapQmlModelSettingsItem{SI_LINK,       tr ("Language"), "", "settings_icon ic_language",                              [] { emit __inst->sigLanguage(); } },
    DapQmlModelSettingsItem{SI_LINK,       tr ("Manage servers"), "", "settings_icon ic_language",                        [] { emit __inst->sigManageServers(); } },
    DapQmlModelSettingsItem{SI_LINK,       tr ("Cryptography"), "", "settings_icon ic_language",                          [] { emit __inst->sigCryptography(); } },
#ifndef DISABLE_THEMES
    DapQmlModelSettingsItem{SI_LINK,       tr ("Dark theme"), "", "settings_icon ic_theme",                               [] { emit __inst->sigDarkTheme(); } },
#endif // DISABLE_THEMES

    DapQmlModelSettingsItem{SI_TITLE,      tr ("Support"), "", "settings_icon",                                           []{} },

    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Send bug report"), "", "settings_icon ic_send-report",                    [] { emit __inst->sigBugSend(); } },
    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Telegram support bot"), "", "settings_icon ic_bot",                       [] { emit __inst->sigTelegramBot(); } },

    DapQmlModelSettingsItem{SI_TITLE,      tr ("Information"), "", "settings_icon",                                       []{} },

    DapQmlModelSettingsItem{SI_LINK,       tr ("Bug reports"), "", "settings_icon ic_information_bug-report",             [] { emit __inst->sigBugReport(); } },
    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Serial key history on this device"), "", "settings_icon ic_key-history",  [] { emit __inst->sigLicenceHistory(); } },
#ifndef DISABLE_TERMSOFUSE_AND_PRIVACYPOLICY
    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Terms of use"), "", "settings_icon ic_terms_policy",                      [] { emit __inst->sigTermsOfUse(); } },
    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Privacy policy"), "", "settings_icon ic_terms_policy",                    [] { emit __inst->sigPrivacyPolicy(); } },
#endif // DISABLE_TERMSOFUSE_AND_PRIVACYPOLICY
    DapQmlModelSettingsItem{SI_BUTTONGRAY, tr ("Version"), "@version", "settings_icon ic_version",                        [] { emit __inst->sigVersion(); } },

    DapQmlModelSettingsItem{SI_TITLE,      "", "", "settings_icon",                                                       []{} },
    DapQmlModelSettingsItem{SI_TITLE,      "", "", "settings_icon",                                                       []{} },

#endif // BRAND_RISEVPN
  };

  /* find indexes */
  qint32 index = 0;
  for (auto i = s_items.cbegin(), e = s_items.cend(); i != e; i++, index++)
    {
      if (i->m_sid == SI_BUTTONRED)
        s_daysLabelIndex    = index;

      if (i->m_sid == SI_BUTTONGRAY)
        s_versionLabelIndex = index;
    }

  /* set version */
  if (s_versionLabelIndex != -1)
    {
      auto version  = QString ("%1 %2")
          .arg (DAP_VERSION, __DATE__);
      s_items[s_versionLabelIndex].m_textSub  = version;
    }
}

void DapQmlModelSettings::slotSetDaysLeft (QString a_days)
{
  beginResetModel();
  s_items[s_daysLabelIndex].m_textSub = (a_days.startsWith("-")) ? "expired" : a_days;
  endResetModel();

  emit dataChanged (
    index (s_daysLabelIndex, 0),
    index (s_daysLabelIndex, columnCount()));
}

void DapQmlModelSettings::slotResetDaysLeft()
{
  beginResetModel();
  s_items[s_daysLabelIndex].m_textSub.clear();
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

/********************************************
 * ModelSettingsItem
 *******************************************/

DapQmlModelSettingsItem::DapQmlModelSettingsItem()
  : QObject()
  , m_sid (DapQmlModelSettings::SI_TITLE)
  , m_cb ([]{})
{

}

DapQmlModelSettingsItem::DapQmlModelSettingsItem (const DapQmlModelSettingsItem &src)
  : QObject (src.parent())
{
  m_sid       = src.m_sid;
  m_textMain  = src.m_textMain;
  m_textSub   = src.m_textSub;
  m_icon      = src.m_icon;
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
  m_cb        = std::move (src.m_cb);
  setParent (src.parent());
}

DapQmlModelSettingsItem::DapQmlModelSettingsItem (
  const DapQmlModelSettings::StyleId a_sid,
  const QString a_textMain,
  const QString a_textSub,
  const QString a_icon,
  const DapQmlModelSettings::ItemCB a_callback)
{
  m_sid       = a_sid;
  m_textMain  = a_textMain;
  m_textSub   = a_textSub;
  m_icon      = a_icon;
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
  m_cb        = std::move (src.m_cb);
  setParent (src.parent());
  return *this;
}

/*-----------------------------------------*/
