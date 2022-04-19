/* INCLUDES */
#include "dapqmlmodelsettings.h"

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

/* LINKS */
static void defaultCb() {}

/* settings */
static void cbLicenceGet();
static void cbLicenceReset();
static void cbLanguage();
static void cbColorTheme();

/* support */
static void cbBugSend();
static void cbTelegramBot();

/* info */
static void cbBugReport();
static void cbLicenceHistory();
static void cbTermsOfUse();
static void cbPrivacyPolicy();
static void cbVersion();

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
//    DapQmlModelSettingsItem{DapQmlModelSettings::StyleId(0),     "0", "", "1", defaultCb},
//    DapQmlModelSettingsItem{DapQmlModelSettings::StyleId(1),     "1", "", "1", defaultCb},
//    DapQmlModelSettingsItem{DapQmlModelSettings::StyleId(2),     "2", "", "1", defaultCb},
//    DapQmlModelSettingsItem{DapQmlModelSettings::StyleId(3),     "3", "", "1", defaultCb},
//    DapQmlModelSettingsItem{DapQmlModelSettings::StyleId(4),     "4", "", "1", defaultCb},

    DapQmlModelSettingsItem{SI_SPACER,     "", "", "1", defaultCb},
    DapQmlModelSettingsItem{SI_TITLE/*TOP*/,   tr ("Settings"), "", "settings_icon", defaultCb},
//    DapQmlModelSettingsItem{SI_SPACER,     "", "", "2", defaultCb},

    DapQmlModelSettingsItem{SI_BUTTONRED,  tr ("Get new licence key"), /*"265 days left"*/" ", "settings_icon ic_renew", cbLicenceGet},
    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Reset licence key"), "", "settings_icon ic_key", cbLicenceReset},
 // DapQmlModelSettingsItem{SI_LINK,       tr ("Language"), "", "settings_icon ic_language", cbLanguage},
    DapQmlModelSettingsItem{SI_LINK,       tr ("Color theme"), "", "settings_icon ic_theme", cbColorTheme},

    DapQmlModelSettingsItem{SI_TITLE,      tr ("Support"), "", "settings_icon", defaultCb},

    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Send a bug report"), "", "settings_icon ic_send-report", cbBugSend},
    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Telegram support bot"), "", "settings_icon ic_bot", cbTelegramBot},

    DapQmlModelSettingsItem{SI_TITLE,      tr ("Information"), "", "settings_icon", defaultCb},

    DapQmlModelSettingsItem{SI_LINK,       tr ("Bug Reports"), "", "settings_icon ic_information_bug-report", cbBugReport},
    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Serial key history on this device"), "", "settings_icon ic_key-history", cbLicenceHistory},
    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Terms of use"), "", "settings_icon ic_terms_policy", cbTermsOfUse},
    DapQmlModelSettingsItem{SI_BUTTON,     tr ("Privacy policy"), "", "settings_icon ic_terms_policy", cbPrivacyPolicy},
    DapQmlModelSettingsItem{SI_BUTTONGRAY, tr ("Version"), "@version", "settings_icon ic_version", cbVersion},
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
    s_items[s_versionLabelIndex].m_textSub  = DAP_VERSION;
}

void DapQmlModelSettings::slotSetDaysLeft (QString a_days)
{
  beginResetModel();
  s_items[s_daysLabelIndex].m_textSub = a_days;
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

/********************************************
 * STATIC FUNCTIONS
 *******************************************/

/* settings */
void cbLicenceGet()     { emit __inst->sigLicenceGet(); }
void cbLicenceReset()   { emit __inst->sigLicenceReset(); }
void cbLanguage()       { emit __inst->sigLanguage(); }
void cbColorTheme()     { emit __inst->sigColorTheme(); }

/* support */
void cbBugSend()        { emit __inst->sigBugSend(); }
void cbTelegramBot()    { emit __inst->sigTelegramBot(); }

/* info */
void cbBugReport()      { emit __inst->sigBugReport(); }
void cbLicenceHistory() { emit __inst->sigLicenceHistory(); }
void cbTermsOfUse()     { emit __inst->sigTermsOfUse(); }
void cbPrivacyPolicy()  { emit __inst->sigPrivacyPolicy(); }
void cbVersion()        { emit __inst->sigVersion(); }

/********************************************
 * ModelSettingsItem
 *******************************************/

DapQmlModelSettingsItem::DapQmlModelSettingsItem()
  : QObject()
  , m_sid (DapQmlModelSettings::SI_TITLE)
  , m_cb (defaultCb)
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
